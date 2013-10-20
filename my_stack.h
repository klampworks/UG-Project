#ifndef MY_STACK_H
#define MY_STACK_H

#include "containers.h"
#include <vector>

class my_stack {

	public:
		my_stack();
		void add(const m_list &to_add);
		m_list get(int index);
		unsigned int size();
		unsigned int size(int index);
		void clear();
		std::string pop(int index);
		void add(int index, const std::string &word, int conf);
		void replace(int index, const m_list &new_stack);
		void clean(int index);
		std::vector<m_list> to_m_list();
		int logical_index(int index);
		void kick_back(int index);
		void replace(const std::vector<m_list> &newy); 
		
	private:
		std::vector<m_list> data;
		std::vector<int> rank;
		bool valid_index(int index);
		
};

#endif
