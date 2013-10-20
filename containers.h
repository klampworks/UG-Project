/*
 * containers.h
 *
 *  Created on: Dec 23, 2012
 */

#ifndef CONTAINERS_H_
#define CONTAINERS_H_

#include <vector>
#include <string>
#include "clue.h"
#include <boost/regex.hpp>


class m_list {
public:
	m_list();
	void add(const m_list &list);
	void print();
	void print(int rows);
	int size() const;
	std::string get(unsigned int index);
	void clear();
	static void create_filter(std::string &input);

	static m_list filter(const m_list &input, std::string filter);
	int is_in_list(std::string word);
	bool empty();
	std::string pop();
	int peek_i() ;
	std::string peek_s() const;
	void clean();
	std::vector<std::string> get_all();
	void add_vector(const std::vector<std::string> &input, int conf);

	static void print_vector(std::vector<std::string> input);

	int get_conf(int index) const;
	std::string get_rank(int index) const;
	void add(std::string word, int confidence);
	
private:
	void order(int index);
	std::vector<int> rank;
	std::vector<int> confidence_score;
	std::vector<std::string> m_vector;
	std::string tabby(const std::string &word, int tab_len);
};

class solution {
public:
	//solution();
	solution(int number=5);
	bool add_solution(const std::vector<std::string> &sol, int sol_length);
	bool add_solution(const std::vector<std::string> &sol);
	bool add_solution(const std::vector<m_list> &stack);
	void arrange_rank();
	std::vector<std::string> get(int my_rank) const;
	std::vector<std::vector<std::string> > get_all() const;
	int calculate_length(const std::vector<std::string> &sol) const;
	int get_len(int my_rank) const;
	void merge(const solution &sol);
	int get_number() const;
	void print(int index) const;
	void print_all() const;

	std::vector<std::string> *s_list;
	int counter;

private:
	int *length;
	int *rank;
	int number;
	bool null;
};

class clue_list {
public:
	clue_list();
	clue_list(const std::vector<clue> &clue_list);
	clue get(int index) const;
	clue get_clue(unsigned int index) const;
	clue get_clue(int clue_number, int across) const;
	int get_clue_index(int clue_number, int across) const;
	void add_clue(const clue &clue_c);
	void add_clue(const std::string &text, const std::string &solution, 
		const std::string &length_p, int start_x, int start_y, int across, int number);
	void add_solution(int index, int across, const std::string &text);
	void add_complete_soluton(const std::vector<m_list> &stack);

	void print_best_solution() const;
	void print_real_solutions() const;
	void print_all_solutions() const;
	void print_clues() const;
	void merge_solution(const solution &solut);

	solution get_solution() const;
	int get_solution_length() const;
	unsigned size() const;

	std::vector<clue> get_list() const;
	void replace_list(const std::vector<clue> &re);

private:
	std::vector<clue> cl;
	solution sol;
};

#endif /* CONTAINERS_H_ */
