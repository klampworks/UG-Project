/*
 * clue.h
 *
 *  Created on: Nov 27, 2012
 */

#ifndef CLUE_H_
#define CLUE_H_

#include <string>
#include <vector>

class clue {

public:
	clue();
	clue(const std::string &text, const std::string &length_p);
	clue(const std::string &text, const std::string &solution, 
		const std::string &length_p, int start_x, int start_y, 
		int across, int number);

	std::string get_text() const;
	int get_length() const;
	std::string get_length_p() const;
	int get_start_x() const;
	int get_start_y() const;
	std::string get_solution() const;
	int get_across() const;
	int get_number() const;
	static int find_clue(int index, int across, const std::vector<clue> &cl);
	std::string get_across_s() const;
	std::string get_number_s() const;

	//returns clue number, across and length
	std::string get_info() const;


private:
	std::string text;
	int length;
	std::string length_p;
	int start_x;
	int start_y;
	std::string solution;
	int across;
	int number;

	int find_real_length(const std::string &len) const;

};
#endif /* CLUE_H_ */
