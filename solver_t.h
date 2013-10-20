/*
 * solver_t.h
 *
 *  Created on: Dec 28, 2012
 */

#ifndef SOLVER_T_H_
#define SOLVER_T_H_

#include <vector>
#include "containers.h"
#include "collision_map.h"
#include "my_stack.h"


typedef struct thread_param {

	clue_list *cl;
	collision_map *cm;
	my_stack master_stack;
	my_stack solver_stack;
	solution t_sol;
	int index;
	int real_index;
	bool finished;
	int index_guard;

	thread_param(collision_map *cm_p, clue_list *cl_p, int size) {
		cm = cm_p;
		cl = cl_p;

		index = 0;
		real_index = 0;
		index_guard = 0; 
		finished = false;
		t_sol = solution(7);

		for (unsigned int j = 0; j < size; j++) {
			solver_stack.add( m_list() );
		}
	}

}thread_param;


typedef struct checker_param {

	clue_list *cl;
	std::vector<thread_param*> thread_data;
	std::vector<int> remaining_threads;
	int finished_length;
	
	checker_param(clue_list *cl_p, std::vector<thread_param*> thread_data_p, std::vector<int> remaining_threads_p, int finished_length_p) {
		cl = cl_p;
		thread_data = thread_data_p;
		remaining_threads = remaining_threads_p;
		finished_length = finished_length_p;
	}

} checker_param;

class solver_t {

public:
	solver_t(clue_list *cl_p, my_stack *master_stack_p, collision_map *cm_p);
	std::vector<thread_param *> solve(int number_of_threads, bool forward_check);
	std::vector<thread_param *> solve(int number_of_threads, bool forward_check, solution sol);
	
	static void set_thread_pause(bool pause);
	static void set_thread_sleep(int miliseconds);
	static void set_next_word();
	static void set_next_clue();

	~solver_t();
	
	static std::string puzzle_name;


private:

	static int no_threads;
	static bool forward_check_enabled;
	static bool die;
	static bool all_threads_done;

	static void * checker_thread(void *para);

	void split_master_stack(int *split_index, int *split_value, int *number_of_threads);
	static void * solver_thread(void *parameters);
	static void thread_go_back(thread_param *para);
	static void thread_validate_candidates(thread_param *para);

	static std::vector<int> find_intersecotrs(thread_param *para, int start_index, int end_index);
	static void forward_check(thread_param *para);
	static char* create_filter(char *filter, int size);
	static bool find_blacklist( std::vector<int> &blacklist, int sought, int &last_index );
	 clue_list *cl;
	my_stack *master_stack;
	collision_map *cm;
	
	static bool thread_pause;
	static int thread_sleep;

	static bool next_word;
	static bool next_clue;
	
	void push_back(int index);
};

#endif /* SOLVER_T_H_ */
