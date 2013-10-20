/*
 * collision_map.h
 *
 *  Created on: Nov 27, 2012
 */


#ifndef COLLISION_MAP_H_
#define COLLISION_MAP_H_

#include <vector>
#include "clue.h"
#include "containers.h"

class collision_map {

public:
	collision_map();
	collision_map(const clue_list &clues);
	bool check(const clue &clue_one, const clue &clue_two);


	void print();

	//make private
private:
	std::vector<int> list_one;
	std::vector<int> list_two;
	std::vector<int> letter_position_one;
	std::vector<int> letter_position_two;



};

#endif /* COLLISION_MAP_H_ */
