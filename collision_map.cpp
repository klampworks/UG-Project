/*
 * collision_map.cpp
 *
 *  Created on: Nov 28, 2012
 */

#include <vector>
#include "clue.h"
#include "collision_map.h"
#include "containers.h"
#include <iostream>

collision_map::collision_map() {}

//Create a collision map from a clue list. This map will cache which 
//clue intersects with whcih and on which indecies to provide quick 
//lookup later on in the solving process.
collision_map::collision_map(const clue_list &clues) {

	int i,j;
	int s = clues.size();

	for (i = 0; i < s; i++) {
		for (j = 0; j < s; j++) {

			if (i == j) {
				//a clue cannot intersect with itself
				continue;
			}

			if (clues.get(i).get_across() == clues.get(j).get_across()) {
				//clues cannot intersect unless one is down and one is across
				continue;
			}

			clue clue_across;
			clue clue_down;

			if (clues.get(i).get_across() == 1) {
				clue_across = clues.get(i);
				clue_down = clues.get(j);
			} else {
				clue_across = clues.get(j);
				clue_down = clues.get(i);
			}

			if (clue_across.get_start_y() >= clue_down.get_start_y() && clue_across.get_start_y() < (clue_down.get_start_y() + clue_down.get_length())) {
				//Y intersect
			} else {
				continue;
			}

			if (clue_down.get_start_x() >= clue_across.get_start_x() && clue_down.get_start_x() < clue_across.get_start_x() + clue_across.get_length()) {
				//X intersect
			} else {
				continue;
			}

			list_one.push_back(clue_down.get_number());
			list_two.push_back(clue_across.get_number());
			letter_position_one.push_back(clue_down.get_start_x());
			letter_position_two.push_back(clue_across.get_start_y());

		}
	}
}

//Given two clues, do they intersect?
bool collision_map::check(const clue &clue_one, const clue &clue_two) {

	clue clue_across;
	clue clue_down;

	if (clue_one.get_across() == 1) {
		clue_across = clue_one;
		clue_down = clue_two;
	} else {
		clue_across = clue_two;
		clue_down = clue_one;
	}

	for (unsigned int i = 0; i < list_one.size(); i++) {
		if (clue_down.get_number() == list_one[i]) {

			if (clue_across.get_number() == list_two[i]) {
				return true;
			}
		}
	}

	return false;
}

void collision_map::print() {
	for (unsigned int i = 0; i < list_one.size(); i++) {
		std::cout << "List_one " << i << " = " << list_one[i];
		std::cout << " List_two " << i << " = " << list_two[i] << std::endl;;
	}
}

