/*
 * containers.cpp
 *
 *  Created on: Dec 23, 2012
 */

/*
 * Custon contains for  holdering the data structures used in the system and
 * methods for manipulating then.
 */

#include "containers.h"
#include <algorithm> //for std::find
#include <iostream>
#include <string>
#include <vector>
#include "clue.h"

/*
 * START m_list
 */

m_list::m_list() {}

//Add a vector of words all with the same confidence score (from the same source).
void m_list::add_vector(const std::vector<std::string> &input, int conf) {

	for(int i = 0; i < input.size(); i++)
		add(input[i],  conf);
}

//Get the confidence score for this word.
int m_list::get_conf(int index) const {

	//Check if in range.
	if (index < 0 || index >= rank.size()) {
	
		std::cout << "INVALID INDEX m_list::get_conf(int)" << std::endl;
		return -1;
	}

	return confidence_score[rank[index]];
}

std::string m_list::get_rank(int index) const {

	if (index < 0 || index >= rank.size()) {
	
		std::cout << "INVALID INDEX m_list::get_rank(int)" << std::endl;
		return "";
	}

	if (rank[index] >= m_vector.size())
		return "";

	return m_vector[rank[index]];
}

void m_list::order(int index) {

	if (index < 1)
		return;
	
	if (index >= rank.size())
		return;

	for (int i = index; i > 0; i--) {
	
		if (confidence_score[rank[i]] > confidence_score[rank[i - 1]]) {
		
			int tmp = rank[i];
			rank[i] = rank[i - 1];
			rank[i - 1] = tmp;
		}
	}
}

//Add a new word with a given confidence score to the list.
void m_list::add(std::string word, int confidence) {

	//Turn it to lower case.
	std::transform(word.begin(), word.end(), word.begin(), ::tolower);

	int r = -1;
	
	//Check to see if the word already exists.
	for (unsigned i = 0; i < rank.size(); i++) {
	
		if (m_vector[rank[i]].compare(word) == 0) {
		
			r = i;
			break;
		}
	}

	//If it doesn't exist add it.
	if (r == -1) {
		
		m_vector.push_back(word);
		confidence_score.push_back(confidence);
		rank.push_back(m_vector.size() - 1);
		
		order(m_vector.size() - 1);
	} else {
	
		//If it does exist just increase its confidence score.
		confidence_score[r] += confidence;
		order(r);
	}
}


//Merge an existing list with this one.
void m_list::add(const m_list &list) {

	for (unsigned int i = 0; i < list.size(); i++)
		add(list.get_rank(i), list.get_conf(i));
}

int m_list::is_in_list(std::string word) {
	//return true or false based on whether a given word already exists in the list or not

	std::transform( word.begin(), word.end(), word.begin(), ::tolower );
	
	std::vector<std::string>::iterator i = std::find(m_vector.begin(), m_vector.end(), word );
	if ((i != m_vector.end()) ) {
		return std::distance(m_vector.begin(), i);
	}

	return -1;
}

//Create a number of tabs to appropriatly format the given word.
std::string m_list::tabby(const std::string &word, int tab_len) {

	int tabs_i;
	int rows = 4;

	//Work out how many tabs we need...
	if (word.length() % tab_len == 0)
		tabs_i = rows - (1 + (word.length() / tab_len));
	else
		 tabs_i = rows - ((word.length() + tab_len - 1) / tab_len);

	std::string tabs_s;

	//Create a string containing the right number of tabs.
	for (unsigned int i = 0; i < tabs_i; i++)
		tabs_s += "\t";

	return tabs_s;
}


//Print the contents of the list in a formatted fashion.
void m_list::print(int rows) {

	for (std::vector<std::string>::size_type i = 0; i < m_vector.size(); i++) {

		if (i%rows == 0) {
			std::cout << std::endl << m_vector[i];
		} else {
			std::string tabs = tabby(m_vector[i - 1], 8);
			std::cout << tabs << m_vector[i];
		}
	}

	std::cout << std::endl;
}

//Simply print the contents of the list.
void m_list::print() {

	for (std::vector<std::string>::size_type i = 0; i < m_vector.size(); i++)
			std::cout << std::endl << m_vector[i];

	std::cout << std::endl;
}

//Get a word at a specified index.
std::string m_list::get(unsigned index) {

	if (index >= 0 && index < m_vector.size())
		return m_vector[index];

	return "";
}

//Return a vector repreentation of the list in an unordered fashion.
std::vector<std::string> m_list::get_all() {
	return m_vector;
}

int m_list::size() const {
	return rank.size();
}

bool m_list::empty() {
	return rank.empty();
}

//Remove last element and return a copy of it.
std::string m_list::pop() {

	if (rank.size() == 0) {
		std::cout << "rank.size() == 0!" << std::endl;
		return "";
	}

	std::string ret = get_rank( 0 );

	for (unsigned i = 1; i < rank.size(); i++)
		rank [i-1] = rank [i];

	//get rid of duplicate on end
	rank.pop_back();

	return ret;
}

void m_list::clear() {
	m_vector.clear();
	rank.clear();
	confidence_score.clear();
}


//Turn our custom filter format into a legit regex.
void m_list::create_filter(std::string &input) {

	boost::regex re;
	std::string regex = "[\\\?_]";

	re.assign(regex, boost::regex_constants::icase);

	input = boost::regex_replace(input, re, ".", 
			boost::match_default | boost::format_sed);

	std::transform(input.begin(), input.end(), input.begin(), ::tolower);
}



//Given an m_list and a filter return a new m_list containing only words that
//comply with filter.
m_list m_list::filter(const m_list &input, std::string filter) {

	//Convert our filter into a real regex.
	m_list::create_filter(filter);
	m_list return_value;

	boost::regex re;
	re.assign(filter, boost::regex_constants::icase);

	for (unsigned i = 0; i < input.size(); i++) {

		if (boost::regex_match(input.get_rank(i), re))
			return_value.add(input.get_rank(i), input.get_conf(i));
	}
	return return_value;
}

int m_list::peek_i() {
	return get_conf(0);
}

std::string m_list::peek_s() const {
	return get_rank(0);
}

void m_list::clean() {

	//Hold onto the originals.
	std::vector<int> tmp_rank = rank;
	std::vector<int> tmp_conf = confidence_score;
	std::vector<std::string> tmp_m_vector = m_vector;
	
	//Clear the member fields.
	rank.clear();
	confidence_score.clear();
	m_vector.clear();
	
	//For all the temps...
	for (unsigned i = 0; i < tmp_rank.size(); i++) {
		
		//Only readd and good ones.
		if (!tmp_m_vector[tmp_rank[i]].empty())
			add(tmp_m_vector[tmp_rank[i]], tmp_conf[tmp_rank[i]]);
	} 
}

/*
 * END m_list
 */

/*
 * START solution
 */

solution::solution(int number) {

	this->number = number;

	length = new int[number];
	s_list =  new std::vector<std::string>[number];

	rank = new int[number];

	for (int i = 0; i < number; i++) {
		rank[i] = i;
		length[i] = 0;
	}

	counter = 0;
}

bool solution::add_solution(const std::vector<m_list> &stack) {
	
	std::vector<std::string> whole_solution;
	int soltion_length = 0;

	for (unsigned int i = 0; i < stack.size(); i++) {

		std::string tmp_s = "";

		if (stack[i].size() > 0)
			//avoid annoying error messages
			tmp_s = stack[i].peek_s();

		if (!tmp_s.empty())
			soltion_length++;

		whole_solution.push_back(tmp_s);
	}

	return add_solution(whole_solution);
}

bool solution::add_solution(const std::vector<std::string> &sol) {

	int sol_length = calculate_length(sol);

	if (sol_length > 0)
		return add_solution(sol, sol_length);
	else
		return false;
}

int solution::calculate_length(const std::vector<std::string> &sol) const {

	int length = 0;

	for (unsigned int i = 0; i < sol.size(); i++) {

		if (sol[i].compare("") != 0)
			length++;
	}

	return length;
}

bool solution::add_solution(const std::vector<std::string> &sol, int sol_length) {

	null = true;

	static int sdhf = -1;

	if (counter < number) {

		if (length[counter] != 0) {
			counter++;
			return add_solution(sol, sol_length);
		}

		s_list[rank[number - 1]] = sol;
		length[rank[number - 1]] = sol_length;
		counter++;

	} else {

		number = number;

		int lowest_index = rank[number-1];
		int len_low_ind = length[lowest_index];

		if (sol_length > len_low_ind) {

			s_list[lowest_index] = sol;
			length[lowest_index] = sol_length;
		} else {
			//not good enough for the list
			return false;
		}
	}

	arrange_rank();
	return true;
}

void solution::arrange_rank() {

	int current_index = number - 1;

	for (int i = number - 2; i >= 0; i--) {
		int rank_current_index = rank[current_index];
		int length_rci = length[rank[current_index]];
		int ranki = rank[i];
		int length_ranki = length[rank[i]];

		if (length[rank[current_index]] > length[rank[i]]) {

			int tmp = rank[i];
			rank[i] = rank[current_index];
			rank[current_index] = tmp;
			current_index = i;
		} else {
			return;
		}
	}
}

std::vector<std::string> solution::get(int my_rank) const {
	return s_list[rank[my_rank]];
}


std::vector<std::vector<std::string> > solution::get_all() const {

	std::vector<std::vector<std::string> > return_v;
	
	for (int i = 0; i < get_number(); i++ ) {

		std::vector<std::string> tmp;
		std::vector<std::string> s_list_cpy = get(i);
		
		for (int j = 0; j < s_list_cpy.size(); j++) {
		
			std::string tmp_str =  s_list_cpy[j];
			
			if (tmp_str.empty())
				continue;
			
			tmp.push_back(tmp_str);
		}

		return_v.push_back(tmp);
		tmp.clear();
	}

	return return_v;
}

int solution::get_len(int my_rank) const {


	if (my_rank >= number)
		return 0;

	return length[rank[my_rank]];
}

int solution::get_number() const {
	return counter;
}

void solution::merge(const solution &sol) {

	for (int i = 0; i < sol.get_number(); i++) {

		if (!add_solution(sol.get(0)))
			//if this was rejected then none of the 
			//subsequent ones will be accepted
			return;
	}
}

void solution::print(int index) const {

	std::vector<std::string> tmp = get(index);

	for (int i = 0; i < tmp.size(); i++) {
	
		if ( tmp[i].empty() ) {
			
			std::cout << "############\n" << std::endl;
			return;
		}
		
		std::cout << tmp[i] << std::endl;
	}
}

void solution::print_all() const {

	for (int i = 0; i < number; i++) {

		if (get_len(i) == 0)
			return;

		std::cout << "Solution " << i << std::endl;
		print(i);
	}
}

clue_list::clue_list() {
	//unused?
	sol = solution(3);
}


clue_list::clue_list(const std::vector<clue> &clue_list) {

	cl = clue_list;
	sol = solution(3);
}

clue clue_list::get(int index) const {
	return get_clue(index);
}

clue clue_list::get_clue(unsigned index) const {

	if (index < 0 || index >= cl.size()) {
		clue empty_clue;
		return empty_clue;
	}

	return cl[index];
}

int clue_list::get_clue_index(int clue_number, int across) const {

	for (unsigned int i = 0; i < cl.size(); i++) {

		if (cl[i].get_across() != across)
			continue;

		if (cl[i].get_number() != clue_number)
			continue;

		return i;
	}

	return -1;
}

clue clue_list::get_clue(int clue_number, int across) const {

	int index = get_clue_index(clue_number, across);

	if (index == -1) {
		clue empty_clue;
		return empty_clue;
	}

	return get_clue(index);
}

void clue_list::add_clue(const clue &clue_c) {
	cl.push_back(clue_c);
}

void clue_list::add_clue(const std::string &text, const std::string &solution, 
	const std::string &length_p, int start_x, int start_y, int across, int number) {

	clue new_clue(text, solution, length_p, start_x, start_y, across, number);
	add_clue(new_clue);
}

void clue_list::add_complete_soluton(const std::vector<m_list> &stack) {

	std::vector<std::string> whole_solution;
	int soltion_length = 0;

	for (unsigned int i = 0; i < stack.size(); i++) {

		std::string tmp_s = stack[i].peek_s();

		if (!tmp_s.empty())
			soltion_length++;

		whole_solution.push_back(tmp_s);
	}

	sol.add_solution(whole_solution);
}

void clue_list::merge_solution(const solution &solut) {
	sol.merge(sol);
}

solution clue_list::get_solution() const {
	return sol;
}

int clue_list::get_solution_length() const {
	return -1;
}

unsigned clue_list::size() const {
	return cl.size();
}

void clue_list::print_clues() const {

	std::cout << "Clues: " << std::endl;

	for (unsigned int i = 0; i < size(); i++) {
		std::cout << get(i).get_info() << get(i).get_text() << std::endl;
	}
}

void clue_list::print_real_solutions() const {

	std::cout << "Real solutions: " << std::endl;

	for (unsigned int i = 0; i < size(); i++) {
		std::cout << get(i).get_info() << get(i).get_solution() << std::endl;
	}
}

void clue_list::print_best_solution() const {

	std::cout << "Best solution: " << std::endl;

	for (unsigned int i = 0; i < size(); i++) {
		std::cout << get(i).get_info() << sol.get(0)[i] << std::endl;
	}
}

void clue_list::print_all_solutions() const {

	for (int j = 0; j < sol.counter; j++) {
		std::cout << "Solution " << j << " length = " << sol.get_len(j) << std::endl;
		for (unsigned int i = 0; i < size(); i++) {
			
			std::string tmp(get(i).get_info() + sol.get(j)[i]);
			
			if (tmp.empty())
				break;
		
			std::cout << "tmp" << tmp << std::endl;
		}
	}
}

//Returns a copy of the clue list,
std::vector<clue> clue_list::get_list() const {
	return cl;
}

//Replaces the object's clue list with a new one.
void clue_list::replace_list(const std::vector<clue> &re) {
	cl = re;
}

/*
 * END clue_list
 */
