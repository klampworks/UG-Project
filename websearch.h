#ifndef WEBSEARCH_H
#define WEBSEARCH_H

#include "Grabber.hpp"
#include "containers.h"

class websearch {

	public:
	
		websearch();
		m_list lookup_wikilist(const std::string &clue, int length);
		m_list lookup_cluedb(const std::string &clue, int length);
		m_list lookup_google(const std::string &clue, int length);
		
	private:
	
		std::vector<std::string> google_search(const std::string &query);
		std::string extract_text(const std::string &page_source);
		
		bool check_404(const std::string &page_source);
		std::string fetch_page(const std::string &url);
		std::string fetch_wikilist(const std::string &word);
		std::string fetch_cluedb(const std::string &clue);

		m_list parse_wikilist(const std::string &page_source, int length);
		m_list parse_page (const std::string &value, 
			const boost::regex &expression, int conf) ;

		std::vector<std::string> parse_page_vector(const std::string &value, 
			const boost::regex &expression);
		
		std::vector<std::string> split_clue(const std::string &clue);
		std::string int_to_string(int value);
		Grabber g;
};

#endif
