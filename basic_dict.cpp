/*
 * basic_dict.c
 *
 *  Created on: Nov 12, 2012
 */

#include "containers.h"
#include "basic_dict.h"
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <algorithm> //for tolower

#include "global.h"

std::string path_prefix;

basic_dict::basic_dict() {
	path_prefix = "files/";
	thes_file = "mthesaur.txt";
}

basic_dict::basic_dict(const std::string &dict_prefix) {
	path_prefix = dict_prefix;
	thes_file = "mthesaur.txt";
}

m_list basic_dict::lookup(const std::string &word, int length) {

	//check that path_prefix+word exists as a file

	//if yes open and read line by line

	//each line is a word

	m_list return_value;

	std::string filename;
	filename = this->path_prefix+word;
	std::ifstream ifs(filename.c_str());


	//check if file exists, return empty vector if not
	if (!ifs.is_open())
		return return_value;

	std::string temp;

	while( getline(ifs, temp)) {
		//
		//check file
		if (length > 0 && temp.length() != length)
			continue;

		return_value.add(temp, global::dict_conf);
	}

	return return_value;
}

m_list basic_dict::prepare_clue(const std::string &clue , int conf) {

	m_list clue_components = basic_dict::split(clue, conf);

	m_list extra_clue_words;

	for (int i = 0; i < clue_components.size(); i++) {
		extra_clue_words.add(basic_dict::alter_clue_word(clue_components.get(i), conf));
	}
	clue_components.add(extra_clue_words);

	return clue_components;
}

m_list basic_dict::lookup_clue(const std::string &clue, int length) {

	m_list clue_components = basic_dict::prepare_clue(clue, global::dict_conf );

	m_list tmp_list;

	for (unsigned i = 0; i < clue_components.size(); i++)
		tmp_list.add(lookup(clue_components.get(i), length));

	return tmp_list;
}

m_list basic_dict::split(const std::string &input, int conf) {

	int start = 0;
	int end = 0;

	const char DELIMITER = ' ';

	m_list return_value;

	for(;;) {
		end = input.find(DELIMITER, start);

		if (end == std::string::npos) {

			std::string tmp_str = basic_dict::clean(input.substr(start));

			if (tmp_str.length() > 0)
				return_value.add(tmp_str, conf);

			break;
		} else {
			std::string tmp_str = basic_dict::clean(input.substr(start, end-start));

			if (tmp_str.length() > 0)
				return_value.add(tmp_str, conf);
		}

		start = end + 1;
	}

	return return_value;
}

std::string basic_dict::clean(const std::string &input) {
	//remove unwanted characters
	//and convert to lower case

	boost::regex re;
	std::string regex = "\\W";

	re.assign(regex, boost::regex_constants::icase);

	std::string tmp = boost::regex_replace(input, re, "", boost::match_default | boost::format_sed);
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
	return tmp;
}

m_list basic_dict::alter_clue_word(const std::string &clue_word, int conf) {
	/*
	 * remove
	 * -s
	 * -ing
	 * -ed
	 * -ly
	 */

	m_list return_value;

	boost::regex re;
	std::string regex;

	//remove -ing
	regex = "ing$";
	re.assign(regex, boost::regex_constants::icase);
	return_value.add(boost::regex_replace( clue_word, re, "", boost::match_default | boost::format_sed ), conf );

	//remove -s
	regex = "s$";
	re.assign(regex, boost::regex_constants::icase);
	return_value.add(boost::regex_replace( clue_word, re, "", boost::match_default | boost::format_sed ), conf );

	//remove -ed
	regex = "ed$";
	re.assign(regex, boost::regex_constants::icase);
	return_value.add(boost::regex_replace( clue_word, re, "", boost::match_default | boost::format_sed ), conf );

	//remove -ly
	regex = "ly$";
	re.assign(regex, boost::regex_constants::icase);
	return_value.add(boost::regex_replace( clue_word, re, "", boost::match_default | boost::format_sed ), conf );

	return return_value;

}

m_list basic_dict::lookup_clue_thesaurus(const std::string &word, int length) {

	m_list clue_components = basic_dict::prepare_clue(word, global::thes_conf);

	m_list return_value;

	for (unsigned i = 0; i < clue_components.size(); i++)
		return_value.add(thes_lookup(clue_components.get(i), length));

	return return_value;
}


m_list basic_dict::thes_lookup(const std::string &word, int length) {

	m_list return_value;
	std::string return_temp;

	std::ifstream ifs( thes_file.c_str() );
	std::string temp;
	boost::regex re;

	std::string regex = "(^" + word +",.*)";
	re.assign(regex, boost::regex_constants::icase);

	while(getline(ifs, temp)) {
		
		if (boost::regex_match(temp, re))
			return_temp.append(temp);
	}

	return_value.add(thes_split(return_temp, length));

	return return_value;
}

m_list basic_dict::thes_split(const std::string &input, int length) {

	int start = 0;
	int end = 0;

	const char DELIMITER = ',';

	m_list return_value;
	for(;;) {

		end = input.find(DELIMITER, start);

		if (end == std::string::npos) {

			if (length == 0 || input.length() - start - 1 == length) {
				return_value.add( input.substr(start), global::thes_conf);
			}

			break;
		} else if (end == 0) {
			//comma at start, chop it off
			//input = input.substr(1);
		} else {

			if (length == 0 || end-start == length) {
				std::string tmp = input.substr(start, end-start);
				return_value.add(tmp, global::thes_conf);
			}
		}

		start = end + 1;
	}

	return return_value;
}
