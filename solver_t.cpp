/*
 * solver_t.cpp
 *
 *  Created on: Dec 28, 2012
 */

/*
 * This is where the algorithmic logic resides.
 * Given a list of clues and candidiate words it will spawn threads and
 * comence the solving process.
 */

#include "solver_t.h"
#include "global.h"

//static feilds
bool solver_t::die = false;
bool solver_t::all_threads_done = false;
bool solver_t::forward_check_enabled = true;

std::string solver_t::puzzle_name;

solver_t::solver_t(clue_list *cl_p, my_stack *master_stack_p, collision_map *cm_p) {

	this->cl = cl_p;
	this->master_stack = master_stack_p;
	this->cm = cm_p;
	solver_t::die = false;
	solver_t::all_threads_done = false;
}

void solver_t::push_back(int index) {

	if (index >= master_stack->size())
		return;
	
	if (master_stack->size() != cl->size())
		return;

	std::vector<m_list> my_ms = master_stack->to_m_list();
	std::vector<clue> my_cl = cl->get_list();
	
	m_list tmp_m = master_stack->get(index);
	clue tmp_c = cl->get(index);
	
	for (unsigned i = index + 1; i < my_cl.size(); i++) {
	
		my_ms[i - 1] = my_ms[i];
		my_cl[i - 1] = my_cl[i];
	}
	
	my_ms[my_ms.size() - 1] = tmp_m;
	my_cl[my_cl.size() - 1] = tmp_c;
	
	master_stack->replace(my_ms);
	cl->replace_list(my_cl);
}


//Split the workload evenly between multiple threads.
void solver_t::split_master_stack(int *split_index, int *split_value, int *number_of_threads) {

	int largest_index(-1);
	int largest_value(-1);

	for (int i = 0; i < master_stack->size() ; i++) {

		if (master_stack->size(i) >= *number_of_threads) {
			*split_index = i;
			*split_value = master_stack->size(i) / *number_of_threads;
			break;
		}

		if (master_stack->size(i) > largest_value) {

			largest_value = master_stack->size(i);
			largest_index = i;
		}
	}

	if (*split_index == -1) {

		//all stacks have fewer elements than threads
		//the simplest way to solve this issue is to use fewer threads

		*number_of_threads = largest_value;
		*split_index = largest_index;
		*split_value = largest_value / *number_of_threads;
	}
}

std::vector<thread_param *> solver_t::solve(int number_of_threads, bool forward_check) {

	return solve(number_of_threads, forward_check, solution());
}

std::vector<thread_param *> solver_t::solve(int number_of_threads, bool forward_check, solution sol) {

	solver_t::forward_check_enabled = forward_check;

	number_of_threads = 2;
	
	//each individual thread needs to know this....
	solver_t::no_threads = number_of_threads;

	pthread_t threads[number_of_threads];
	std::vector<thread_param *> thread_data;
	
	for (int i = 0; i < number_of_threads; i++) {
		thread_data.push_back(0);
	}
	
	//which index of master_stack are we splitting into threads?
	int split_index = -1;

	//how many words per thread?
	int split_value = -1;
	
	push_back(2);
	
	if (sol.get_number() > 0) {
		
		int sol_size = sol.get_len( 0 );
		push_back(sol_size);
	}

	for (int i = 0; i < master_stack->size(); i++) {
		master_stack->add(i, cl->get(i).get_solution(), 0);
	}
	
	split_master_stack(&split_index, &split_value, &number_of_threads);

	std::vector<int> remaining_threads;
	my_stack my_master_stack;

	for (int i = 0; i < number_of_threads; i++) {

		for (unsigned j = 0; j < master_stack->size(); j++) {

			if (j != split_index) {

				my_master_stack.add(master_stack->get(j));
			} else {

				my_master_stack.add(m_list());
				
				for (int k = 0; k < split_value; k++) {
					
					int conf = master_stack->get(j).peek_i();
					std::string word = master_stack->pop(j);
					my_master_stack.add(j, word, conf);
				}
			}
		}

		thread_data[i] = new thread_param(cm, cl, master_stack->size());

		thread_data[i]->master_stack = my_master_stack;

		my_master_stack.clear();
		pthread_create(&threads[i], NULL, solver_thread, thread_data[i]);

		remaining_threads.push_back(i);
	}

	int finished_length = master_stack->size();

	checker_param *p = new checker_param(cl, thread_data, 
						remaining_threads, finished_length);

	pthread_t checker_t;
	pthread_create(&checker_t, NULL, checker_thread, p);

	return thread_data;
}

//This thread polls the other threads in a loop to check if and when they finish.
void * solver_t::checker_thread(void *para) {

	checker_param *p = (checker_param*)para;
	
	int start_time = std::time(NULL);
	int finish_time = 0;
	
while(!p->remaining_threads.empty()) {

	sleep(1);

	global::get_window()->update();

	for (int i = 0; i < p->remaining_threads.size(); i++) {

		if (!p->thread_data[p->remaining_threads[i]]->finished) {
			continue;
		}
		
		if (!solver_t::die) { 

			std::cout << "Thread " << p->remaining_threads[i] << " finished" << std::endl;

			p->cl->merge_solution( p->thread_data[p->remaining_threads[i]]->t_sol );

			p->thread_data[p->remaining_threads[i]]->t_sol.print_all();
	
			

			if (p->thread_data[p->remaining_threads[i]]->t_sol.get_len(0) == p->finished_length) {
			//solution_found!
			}
			
		}

		 p->remaining_threads.erase(p->remaining_threads.begin()+i);
	}
}
	
	solver_t::all_threads_done = true;
	return '\0';
}

//Thread function.
void * solver_t::solver_thread(void *parameters) {

	//Extract the environment from the parameters pointer.
	thread_param *para = (thread_param *)parameters;

	static int thread = 0;

	//While out index pointer is in range of the master_stack boundaries.
	while (para->real_index < para->master_stack.size() && para->real_index > -1) {
	
		//Check if the thread has been cancelled.
		if (solver_t::die) {
			std::cout << "Thread " << thread << " exiting!" << std::endl;
			para->finished = true;
			pthread_exit('\0');
		}
	
		usleep(solver_t::thread_sleep);
	
		while (solver_t::thread_pause) {
		
			static int check = 0;
			check++;
		
			usleep(solver_t::thread_sleep);
			
			//wait for all threads to be alined to avoid starvation
			while (check < solver_t::no_threads);
			
			if (solver_t::next_word) {
				solver_t::next_word = false;
				break;
			}	
			
			if (solver_t::die) {
				std::cout << "Thread " << thread << 
				" exiting!" << std::endl;
				para->finished = true;
				pthread_exit('\0');
			}
		}

		//populate solver_stack with valid candidates from master_stack.
		thread_validate_candidates( para ); 
		
		if (solver_t::forward_check_enabled)
			forward_check(para);
		
		para->solver_stack.clean(para->index);

		//Have we hit a dead end?
		if (para->solver_stack.get(para->index).size() == 0) {

			thread_go_back(para);

			//-1 means we cannot backtrack anymore.
			if (para->index == -1) {
				para->finished = true;
				return '\0';
			}
		}

		para->index++;
		para->real_index = para->solver_stack.logical_index(para->index);
	}
	
	para->t_sol.add_solution(para->solver_stack.to_m_list());
	para->finished = true;

	return '\0';
}

void solver_t::thread_go_back(thread_param *para) {

	//#######################################################################
	// Recursive backtracking function
	// Checks if current stack has members
	// If not it backtracks to a point where there are members to be used or
	// Dies because dead end, solution cannot be found
	//#######################################################################
	
	if (para->solver_stack.get(para->index).empty()) {

		//we do not want to backtrack back beyond a certain point
		if (para->index == para->index_guard) {
	
			para->t_sol.add_solution(para->solver_stack.to_m_list());
			
			para->index = -1;
			
			return;
		}

		//keep backtracking until a stack with members has been found (or until deadend)
		para->index--;
		para->real_index = para->solver_stack.logical_index(para->index);
		
		return thread_go_back(para);
	} else {
		//stack with members found! Pop the last result because that didn't work out
		para->t_sol.add_solution(para->solver_stack.to_m_list());

		std::string popped = para->solver_stack.pop(para->index);
		
		std::cout << "Chucking " << popped << std::endl;

		//If the last member was just popped we need to backtrack to find another full stack...
		if (para->solver_stack.get(para->index).empty()) {
			return thread_go_back( para );
		}

		return;
	}

	return;
}

std::vector<int> solver_t::find_intersecotrs(thread_param *para, 
	int start_index, int end_index) {

	collision_map cm((*para->cl));

	std::vector<int> intersectors;

	for (int i = start_index; i < end_index; i++) {
	
		int current_index = para->solver_stack.logical_index(i);
	
		//can't intersect if both are down or across
		if (para->cl->get(current_index).get_across() == 
			para->cl->get(para->real_index).get_across()) {
			continue;
		}

		//check if they intersect
		if (!cm.check(para->cl->get(para->real_index), 
			para->cl->get(current_index))) {

			continue;
		}

		//they intersect!
		intersectors.push_back(i);
	}
	
	return intersectors;
}

//Validate candidate words to make sure they fit in with candidates from other stacks.
void solver_t::thread_validate_candidates(thread_param *para) {

	collision_map cm((*para->cl));

	std::vector<int> intersectors;
	
	intersectors = find_intersecotrs(para, 0, para->index);

	if (intersectors.empty()) {
		//There are no intersectors, accept the stack as is.
		para->solver_stack.replace(para->index, para->master_stack.get(para->real_index));
		return;
	}

	clue current_clue = para->cl->get(para->real_index);
	char *filter = create_filter(filter, current_clue.get_length());
	
	for (unsigned int i = 0; i < intersectors.size(); i++) {

		std::string tmp_s = para->solver_stack.get(intersectors[i]).peek_s();

		if (tmp_s.empty())
			continue;

		clue sect = para->cl->get(para->solver_stack.logical_index(intersectors[i]));

		//Create a function pointer.
		int(clue::*fn)()const;
		int(clue::*fn2)()const;

		//Depending on whether the current clue is across or down
		//assign get_start_y or get_start_x respectivly.
		fn = current_clue.get_across()? &clue::get_start_y : &clue::get_start_x;
		fn2 = current_clue.get_across()? &clue::get_start_x : &clue::get_start_y;

		//Find the grid coordinate offset of the clue intersection square.
		std::string tmp_sub = tmp_s.substr((&current_clue->*fn)() - (&sect->*fn)(), 1);
		
		//Derive the char for this offset.
		char tmp_c = tmp_sub.c_str()[0];

		//Derive a second offset for the intersecting clue and set the char.
		filter[(&sect->*fn2)() - (&current_clue->*fn2)()] = tmp_c;
	}

	//check that words mesh etc

	para->solver_stack.replace(para->real_index, m_list::filter(para->master_stack.get(para->real_index), filter));
	
}

char* solver_t::create_filter(char *filter, int size) {

	//Find the actual number of bytes needed.
	unsigned filter_size = sizeof *filter * size;

	//Allocate the memory.
	filter = (char*)malloc(filter_size);

	//Set each character to underscore (wildcard character).
	memset(filter, '_', filter_size);

	//Don't forget the terminating NUL.
	filter[size] = '\0';

	//Return the new pointer.
	return filter;
}

//Look ahead to make sure the last assignment does not cause a dead end later on.
void solver_t::forward_check(thread_param *para) {

	m_list input_stack = para->solver_stack.get(para->index);
	
	std::vector<int> intersectors = find_intersecotrs(para, para->index, para->cl->size());
	
	std::vector<int> blacklist;
	
	char *filter = '\0';
	
	if (intersectors.empty())
		//Nothing to do.
		return;
	
	std::string tmp_s( "" );
	std::string word( "" );
	
	for (unsigned i = 0; i < input_stack.size(); i++) {
	
		word = "";
		word += input_stack.get( i );
	
		for (unsigned j = 0; j < intersectors.size(); j++ ) {

			if (filter) {
				free(filter);
				filter = '\0';
			}
				
			int tmp_s_index = intersectors[j];
			int tmp_s_index_real = para->solver_stack.logical_index(tmp_s_index);

			//Get the word on the top of the stack.
			tmp_s = para->master_stack.get(tmp_s_index_real).get(0);
			
			//Create a blank filter.
			filter = create_filter(filter, tmp_s.length());
			
			int intersector_is_across = para->cl->get(tmp_s_index_real).get_across();
			
			int intersector_y = para->cl->get(tmp_s_index_real).get_start_y();
			int word_y = para->cl->get(para->real_index).get_start_y();
			int string_index;
			
			int word_x = para->cl->get(para->real_index).get_start_x();
			int intersector_x = para->cl->get(tmp_s_index_real).get_start_x();
			int filter_index;
			
			if (intersector_is_across) {
				string_index = intersector_y - word_y;
 				filter_index = word_x - intersector_x;
			} else {				
 				string_index = intersector_x - word_x;
 				filter_index = word_y - intersector_y;
			}

			char tmp_c = word.c_str()[ string_index ];
			
			filter[filter_index] = tmp_c;

			//Apply the filter to the current stack.
			m_list hypothetical_stack = m_list::filter(para->master_stack.get(tmp_s_index_real), filter);
		
			//Check that this generated stack is not empty.
			if (hypothetical_stack.empty())
				//We can't use this index, blacklist it.
				blacklist.push_back(i);
		}
	}
		
	if (filter) {
		free(filter);
		filter = '\0';
	}
	
	m_list new_stack;
	
	int last_index = 0;

	for (unsigned i = 0; i < input_stack.size(); i++) {
	
		if (!find_blacklist(blacklist, i, last_index)) {
		
			//not in blacklist...
			new_stack.add(input_stack.get_rank(i), input_stack.get_conf(i));
		}
	}
	
	para->solver_stack.replace(para->index, new_stack);
	return;
}

bool solver_t::find_blacklist(std::vector<int> &blacklist, int sought, int &last_index) {

	for (int i = last_index; i < blacklist.size(); i++) {
	
		if (blacklist[i] == sought) {
			last_index = i;
			return true;
		
		} else if (blacklist[i] > sought) {
		
			//ordered list, all future ones are bigger too...
			last_index = i;
			return false;
		}
	}
	return false;
}

bool solver_t::thread_pause = false;
int solver_t::thread_sleep = 1;
bool solver_t::next_word;
bool solver_t::next_clue;
int solver_t::no_threads;
	
void solver_t::set_thread_pause(bool pause) {
	
	thread_pause = pause;
}

void solver_t::set_thread_sleep(int miliseconds) {
	
	if (miliseconds < 1) {
		return;
	}
	
	thread_sleep = miliseconds;
}

void solver_t::set_next_word() {
	
	next_word = true;
}

void solver_t::set_next_clue() {

	next_clue = true;
}

solver_t::~solver_t() {

	std::cout << "Cleaning up, waiting for threads to finish..." << std::endl;
	solver_t::die = true;

	while(!all_threads_done)
		usleep(solver_t::thread_sleep);
}
