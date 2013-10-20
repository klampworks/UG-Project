/*
 * basic_dict.h
 *
 *  Created on: Nov 12, 2012
 */
#ifndef BASIC_DICT_H_
#define BASIC_DICT_H_

#include <vector>
#include <string>
#include "containers.h"


class basic_dict {

public:
	basic_dict();
	basic_dict(const std::string &dict_prefix);
	m_list lookup(const std::string &word, int length);
	static m_list split(const std::string &input, int conf );
	m_list lookup_clue(const std::string &clue, int length);
	static m_list prepare_clue(const std::string &clue, int conf );
	static std::string clean(const std::string &input);

	m_list lookup_clue_extra(const std::string &clue, int length);

	m_list find_plural_words(const std::string &word, int length);
	m_list find_past_tense_words(const std::string &word, int length);
	m_list find_present_tense_words(const std::string &word, int length);

	m_list lookup_clue_thesaurus(const std::string &word, int length);
	m_list thes_lookup(const std::string &word, int length);
	m_list thes_split(const std::string &input, int length);

private:
	std::string path_prefix;
	std::string thes_file;

	static m_list alter_clue_word(const std::string &clue_word, int conf );

};

#endif /* BASIC_DICT_H_ */
