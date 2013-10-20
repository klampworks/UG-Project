/*
 * clue.cpp
 *
 *  Created on: Nov 27, 2012
 */

#include <string>
#include "clue.h"
#include <boost/regex.hpp>
#include <sstream>

#include <iostream>

/*
 * This class reprsents a clue, its clue text, number of letters expected,
 * whether it is down or across and its coordinates in the grid.
 */

clue::clue() {}

clue::clue(const std::string &text, const std::string &length_p) {

	this->text = text;
	this->length = find_real_length(length_p);
	
	this->length_p = length_p;
	
	this->solution = "";
	this->start_x = 0;
	this->start_y = 0;
	this->across = 0;
	this->number = 0;
}

clue::clue(const std::string &text, const std::string &solution, 
	const std::string &length_p, int start_x, int start_y, int across, 
	int number) {

		this->text = text;
		this->solution = solution;
		this->length = find_real_length(length_p);
		this->length_p = length_p;
		this->start_x = start_x;
		this->start_y = start_y;
		this->across = across;
		this->number = number;
}

std::string clue::get_text() const {
	return text;
}

int clue::get_length() const {
	return length;
}

std::string clue::get_length_p() const {
	return length_p;
}

int clue::get_start_x() const {
	return start_x;
}

int clue::get_start_y() const {
	return start_y;
}

std::string clue::get_solution() const {
	return solution;
}

int clue::get_across() const {
	return across;
}

std::string clue::get_across_s() const {

	if (across)
		return "across";
	else
		return "down";
}

int clue::get_number() const {
	return number;
}

std::string clue::get_number_s() const {

	std::stringstream ss;
	ss << number;
	return ss.str();
}

int clue::find_clue(int index, int across, const std::vector<clue> &cl) {

	for (int i = 0; i < cl.size(); i++) {

		if (cl[i].get_number() == index && cl[i].get_across() == across)
			return i;
	}

	return -1;
}


int clue::find_real_length(const std::string &len) const {

	std::stringstream ss;
	ss.str(len);
	std::string tmp;
	std::vector<std::string> parts;
	
	while (std::getline(ss, tmp, ','))
		parts.push_back(tmp);
	
	if (parts.size() == 0)
		parts.push_back(len);
	
	std::vector<std::string> parts_2;
	
	for (unsigned i = 0; i < parts.size(); i++) {
	
		ss.str(parts[i]);
		ss.clear();
		
		while (std::getline(ss, tmp, '-'))
			parts_2.push_back(tmp);
	}
	
	//parts_2 will now have individual elemants sans comma and dash.
	int total = 0;
	
	for (int i = 0; i < parts_2.size(); i++)
		total  += atoi(parts_2[i].c_str());
	
	return total;
}

std::string clue::get_info() const {

	return std::string("Clue " + get_number_s() + " " + get_across_s() + 
				" " + get_length_p() + " letters ");
}
