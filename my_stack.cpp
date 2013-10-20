#include "my_stack.h"

/*
 * This class is a stack data structure with exxtended features.
 * It keeps track of each element's confidence score and allows lements to be
 * kicked to the end of the stack permenantly when they have been deemed to be
 * inhibiting the solving process.
 */

my_stack::my_stack() : data(), rank() {

}

void my_stack::add(const m_list &to_add) {

	data.push_back(to_add);
	rank.push_back(data.size() - 1);
}

void my_stack::replace(const std::vector<m_list> &newy) {

	clear();
	
	for (unsigned i = 0; i < newy.size(); i++)
		add(newy[i]);
}

m_list my_stack::get(int index) {

	if (!valid_index(index))
		return m_list();

	return data[rank[index]];
}

unsigned int my_stack::size() {
	return data.size();
}

void my_stack::clear() {

	data.clear();
	rank.clear();
}

bool my_stack::valid_index(int index) {

	if (index < 0) {
		std::cout << "invalid index " << index << "/" << 
		data.size() << std::endl;
		return false;
	}
	
	if (index >= data.size()) {
	
		std::cout << "invalid index " << index << "/" << 
		data.size() << std::endl;
		return false;
	}

	return true;
}

unsigned int my_stack::size(int index) {

	if (!valid_index(index))
		return 0;
	
	return data[index].size();
}

std::string my_stack::pop(int index) {

	if (!valid_index(index))
		return "";
	
	return data[rank[index]].pop();
}

void my_stack::add(int index, const std::string &word, int conf) {

	if (!valid_index(index))
		return;
	
	data[rank[index]].add(word, conf);
}

void my_stack::replace(int index, const m_list &new_stack) {

	if (!valid_index(index))
		return;
	
	data[rank[index]] = new_stack;
}

void my_stack::clean(int index) {

	if (!valid_index(index))
		return;
	
	data[rank[index]].clean();
}

std::vector<m_list> my_stack::to_m_list() {

	return data;
}

int my_stack::logical_index(int index) {

	if (!valid_index(index))
		return 0;
	
	return rank[index];
}

void my_stack::kick_back(int index) {

	int target_index = rank[index];
	
	for (unsigned i = index + 1; i < rank.size(); i++)
		rank[i - 1] = rank[i];
	
	rank[rank.size() - 1] = target_index;
}
