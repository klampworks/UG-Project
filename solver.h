/*
 * solver.h
 *
 *  Created on: Nov 26, 2012
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "my_stack.h"
#include "websearch.h"
#include "collision_map.h"
#include "basic_dict.h"

class solver {
	public:

		bool file_exists(const std::string &file);
		std::vector<clue> read_clues(const std::string &file);
		bool prepare_stack(bool dict, bool thes, bool web_wiki, 
			bool web_goog, bool web_clue);

		bool prepare_clue_list(const std::string &file);

		m_list lookup_clue(const clue &clue_c, bool dict, bool thes, 
			bool web_wiki, bool web_goog, bool web_clue);

		m_list crop_words(const m_list &words, int number);

		bool cl_ready();
		clue_list get_cl();
		void branch_order();
		void create_grid();

		collision_map cm;
		clue_list cl;

		my_stack master_stack;
		std::vector<m_list> solver_stack;
		int THRESHOLD_CANDIDATES;
		
		int cl_size();
		bool cl_is_across(int index);
		std::string cl_get_clue_text(int index);
		
		m_list get_master_stack_m(int index);
		int get_grid_size();
		
	private:
		bool seek_string(const std::string &tmp, std::string &tmp_2, 
			const std::string &match);

		void find_grid_size(const std::vector<clue> &input);
		int grid_size;
		websearch ws;
		basic_dict bd;
};

#endif /* SOLVER_H_ */
