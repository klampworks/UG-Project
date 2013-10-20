/*
 * solver.cpp
 *
 *  Created on: Nov 26, 2012
 */

#include "solver.h"
#include "global.h"
#include <fstream>

m_list solver::lookup_clue(const clue &clue_c, bool dict, bool thes, 
	bool web_wiki, bool web_goog, bool web_clue) {

	m_list return_value;

	if (dict)
		return_value.add(bd.lookup_clue(clue_c.get_text(), clue_c.get_length()));

	if (thes)
		return_value.add(bd.lookup_clue_thesaurus(clue_c.get_text(), clue_c.get_length()));

	if (web_wiki)
		return_value.add(ws.lookup_wikilist(clue_c.get_text(), clue_c.get_length()));
	
	if (web_goog)
		return_value.add(ws.lookup_google(clue_c.get_text(), clue_c.get_length()));
	
	if (web_clue)
		return_value.add(ws.lookup_cluedb(clue_c.get_text(), clue_c.get_length()));

	return return_value;
}


bool solver::prepare_stack(bool dict, bool thes, bool web_wiki, 
	bool web_goog, bool web_clue) {

	if (cl.size() < 1)
		return false;

	master_stack.clear();
	solver_stack.clear();

	m_list tmp_m_list;
	m_list empty_m_list;

	for (int i = 0; i < cl.size(); i++) {

		tmp_m_list.clear();
		tmp_m_list.add(lookup_clue(cl.get(i), dict, thes, web_wiki, web_goog, web_clue));
		
		tmp_m_list=(crop_words(tmp_m_list, 100));
		
		master_stack.add(tmp_m_list);
		solver_stack.push_back(empty_m_list);
	}

	return true;
}

bool solver::prepare_clue_list(const std::string &file) {

	cl = clue_list(read_clues(file));

	if (cl.size() == 0) {
		return false;
	}

	return true;
}


//Given a file of the correct format, parsed clues into a vector of clue objects.
std::vector<clue> solver::read_clues(const std::string &file) {

	std::vector<clue> return_value;

	if (!file_exists(file)) {
		return return_value;
	}

	std::ifstream ifs(file.c_str());
	std::string tmp, tmp_2;
	
	int clue_number;
	std::string clue_length;
	std::string clue_text;
	std::string clue_solution;
	
	int start_x, start_y;
	int across = -1;
	
	int count = 0;
	/*
		0 = number
		1 = down or across
		2 = letters
		3 = x
		4 = y
		5 = text
		6 = solution
	*/
	
	while (std::getline(ifs, tmp)) {
	
		if (tmp.empty())
			continue;
		
		switch (count) {
		
		case 0:
			//clue number
			if (!seek_string(tmp, tmp_2, std::string("number")))
				continue;
			
			clue_number = atoi(tmp_2.c_str());
			count = 1;
			break;
		case 1:
			//down or across
			if (tmp.compare("down") == 0)
				across = 0;
			else if (tmp.compare("across") == 0)
				across = 1;
			else
				continue;
			
			count = 2;
			break;
		case 2:
			//length
			if (!seek_string(tmp, tmp_2, std::string("letters")))
				continue;
			
			clue_length = tmp_2;
			count = 3;
			break;
		case 3:
			//start_x
			if (!seek_string(tmp, tmp_2, std::string("x")))
				continue;
			
			start_x = atoi(tmp_2.c_str());
			count = 4;
			break;			
		case 4:
			//start_y
			if (!seek_string(tmp, tmp_2, std::string("y")))
				continue;
			
			start_y = atoi(tmp_2.c_str());
			count = 5;
			break;
		case 5:
			//text
			clue_text = tmp;
			count = 6;
			break;
		case 6:
			//solution
			clue_solution = tmp;
			
			std::transform(clue_solution.begin(), 
					clue_solution.end(), 
					clue_solution.begin(), 
					::tolower);

			//create clue
			count = 0;
			
			return_value.push_back(clue(
				clue_text, //clue text 
				clue_solution,
				clue_length, //length (string) i.e. 3,2
				start_x,
				start_y,
				across, //across
				clue_number //number?
			));
			
			break;
		} 
	}

	find_grid_size(return_value);
	return return_value;
}

int solver::get_grid_size() {
	return grid_size;
}

void solver::find_grid_size(const std::vector<clue> &input) {

	int sdilhhflgkjsdfdkj = 0;
	int x, y;

	for (int i = 0; i < input.size(); i++) {
	
		if (input[i].get_across()) {
		
			x = input[i].get_start_x() + input[i].get_length();
			y = input[i].get_start_y();
		} else {
		
			x = input[i].get_start_x();
			y = input[i].get_start_y() + input[i].get_length();
		}
	
		if (x > sdilhhflgkjsdfdkj)
			sdilhhflgkjsdfdkj = x;
		
		if ( y > sdilhhflgkjsdfdkj )
			sdilhhflgkjsdfdkj = y;
	}
	
	grid_size = sdilhhflgkjsdfdkj;
}

bool solver::seek_string(const std::string &tmp, std::string &tmp_2, 
	const std::string &match) {

	std::stringstream ss(tmp);
				
	if (!std::getline(ss, tmp_2, ' '))
		return false;
	
	if (tmp_2.compare(match) != 0)
		return false;

				
	if (!std::getline(ss, tmp_2))
		return false;
	
	//tmp_2 should now have the first word after the match word and a space
	return true;
}


void solver::branch_order() {

	collision_map cm(cl);
	std::vector<clue> my_cl = cl.get_list();
	std::vector<clue> new_cl;

	new_cl.push_back(my_cl[0]);
	my_cl.erase(my_cl.begin());

	int i = 0;

	while (1) {

		int j = new_cl.size() - 1;

		for (; j >= 0; j--) {

			if (new_cl[j].get_across() == my_cl[i].get_across())
				continue;

			if (!cm.check(new_cl[j], my_cl[i]))
				continue;

			break;
		}

		if (j > -1) {
			//intersector found at j

			new_cl.push_back(my_cl[i]);
			my_cl.erase(my_cl.begin() + i);
		} else {
			i++;
		}

		if (i >= my_cl.size())
			i = 0;

		if (my_cl.size() == 0)
			break;
	}
	
	cl.replace_list(new_cl);
}


bool solver::file_exists(const std::string &file) {

	std::ifstream ifs(file.c_str());

	//check if file exists, return empty vector if not
	if (!ifs.is_open()) {
		return false;
	}

	ifs.close();
	return true;
}

m_list solver::crop_words(const m_list &words, int number) {
	
	m_list new_list;

	for (int i = 0; i < number; i++) {
		new_list.add(words.get_rank(i), words.get_conf(i));
	}

	return new_list;
}


void solver::create_grid() {

	int size = grid_size;
	std::vector<std::vector<int> > grid;
	
	{
	std::vector<int> tmp;
	for (int i = 0; i < size; i++) {
		
		for (int j = 0; j < size; j++)
			tmp.push_back(0);	
		
		grid.push_back(tmp);
		tmp.clear();	
	}
	}
	
	std::vector<std::vector<int> > grid_2;

	for (int i = 0; i < cl.size(); i++) {

		clue c = cl.get(i);
		
		std::vector<int> *tmp_2 = new std::vector<int>;
		
		tmp_2->push_back(c.get_number());
		tmp_2->push_back(c.get_start_x());
		tmp_2->push_back(c.get_start_y());
		
		if (c.get_across()) {
		
			tmp_2->push_back(c.get_length()); //across
			tmp_2->push_back(1); //down
		
			for (int j = c.get_start_x(); j < c.get_start_x() + c.get_length(); j++) {

				if (j >= grid.size()) {
					std::cout << "ERROR j >= grid.size() " << 
					j << std::endl;
				}
				
				if (c.get_start_y() >= grid.size()) {
					std::cout << 
					"ERROR c.get_start_y() >= grid.size() " << 
					c.get_start_y() << std::endl;
				}
			
				grid[j][c.get_start_y()] = 1;
			}
		} else {
		
			tmp_2->push_back(1); //across
			tmp_2->push_back(c.get_length()); //down
		
			for (int j = c.get_start_y(); j < c.get_start_y() + c.get_length(); j++) {
			
				if (j >= grid.size()) {
					std::cout << "ERROR j >= grid.size() " << 
					j << std::endl;
					std::cout << "c.get_start_y() = " << 
					c.get_start_y() << std::endl;
					std::cout << "c.get_length() = " << 
					c.get_length() << std::endl;
					continue;
				}
				
				if (c.get_start_x() >= grid.size()) {
					std::cout << 
					"ERROR c.get_start_x() >= grid.size() " << 
					c.get_start_x() << std::endl;
				}
			
				grid[c.get_start_x()][j] = 1;
			}
		}
		
		grid_2.push_back( (*tmp_2) );
		delete tmp_2;
	}
	
	global::get_window()->set_grid(grid);
	global::get_window()->set_grid_2(grid_2);
}

int solver::cl_size() {
	return cl.size();
}

bool solver::cl_is_across(int index) {
	return cl.get_clue(index).get_across();
}

std::string solver::cl_get_clue_text(int index) {
	return cl.get_clue(index).get_info() + ": " + cl.get_clue(index).get_text();
}

m_list solver::get_master_stack_m( int index ) {
	return master_stack.get( index );
	
}
