#include "websearch.h"
#include <sstream>
#include "global.h"

/*
 * This is the websearch module.
 *
 * Given a clue it can look the clue up on
 * 	- Wikipedia
 * 	- Google
 * 	- Clue database
 *
 * These three websites are different enough to each warrent its own set of functions.
 */

websearch::websearch() : g() {

}

std::string websearch::extract_text(const std::string &page_source) {

	std::vector<std::string> parts = parse_page_vector(page_source, 
							boost::regex(">([^>]+)<"));

	std::string return_v("");

	for (unsigned  i = 0; i < parts.size(); i++)
		return_v += parts[i] + "\n";

	return return_v;
}

std::vector<std::string> websearch::google_search(const std::string &query) {

	std::vector<std::string> clue_parts = split_clue(query);
	
	if (clue_parts.size() == 0)
		return std::vector<std::string>();
	
	std::string url("");
	
	url += clue_parts[0];
	
	for (unsigned i = 1; i < clue_parts.size(); i++)
		url += "+" + clue_parts[i];
	
	url = "http://www.google.com/search?q=" + url;
	
	std::string page_source = fetch_page(url);

	std::vector<std::string> results = parse_page_vector(page_source, boost::regex("href=\"/url\\?q=((?!http://webcache.*)(?:[^\&]|&amp;[^s][^a])+)[^\"]+\""));
	
	return results;
}

m_list websearch::lookup_google(const std::string &clue, int length) {

	std::vector<std::string> leads = google_search(clue);
	std::string page_source("");
	
	boost::regex re("(?:^|\W)([a-zA-Z]{" + int_to_string(length) + "})(?:$|\W)");
	
	m_list return_v;
	
	for (unsigned i = 0; i < leads.size(); i++) {
	
		std::cout << "fetching " << leads[i] << std::endl;
		
		page_source = fetch_page(leads[i]);
		
		page_source = extract_text(page_source);
		
		return_v.add(parse_page(page_source, re, global::web_goog_conf));
	}
	
	return return_v;
}

m_list websearch::lookup_cluedb(const std::string &clue, int length) {
	
	std::string url = "";
	std::string page_source = "";
	
	std::vector<std::string> results = google_search(clue + " site%3Ahttp%3A%2F%2Fwww.crosswordgiant.com%2Fcrossword-clue%2F");
	m_list return_v;
	
	boost::regex re("http://www.crosswordgiant.com/crossword-answer[^\"]+\">([A-Z]{" + int_to_string(length) + "})</a>");
	
	for (unsigned i = 0; i < results.size(); i++) {

		url = results[i];
		
		std::cout << "fetching " << url << std::endl;
	
		page_source = fetch_page(url);

		return_v.add(parse_page(page_source, re, global::web_clue_conf));
	}
	
	return return_v;
}

std::vector<std::string> websearch::parse_page_vector(const std::string &value, 
						const boost::regex &expression) {

	std::vector<std::string> return_v;

	std::string::const_iterator start, end;
	start = value.begin();
	end = value.end();
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	
	while(regex_search(start, end, what, expression, flags)) {
		
		return_v.push_back(what[1]);
		start = what[0].second;
   }

	return return_v;
}

m_list websearch::parse_page(const std::string &value, 
				const boost::regex &expression, 
				int conf) {

	m_list return_v;
	
	return_v.add_vector(parse_page_vector(value, expression),conf);

	return return_v;
}

bool websearch::check_404(const std::string &page_source) {

	const std::string regex(".*Wikipedia does not have an article with this exact name.*");
	boost::regex re(regex);
	return (boost::regex_match(page_source, re));
}

m_list websearch::parse_wikilist(const std::string &page_source, int length) {

	if (check_404(page_source))
		return m_list();

	const std::string regex(">([a-zA-Z]{" + int_to_string(length) + "})</a>");

	boost::regex re(regex);
	return parse_page(page_source, re, global::web_wiki_conf);
}

m_list websearch::lookup_wikilist(const std::string &clue, int length) {

	std::vector<std::string> clue_words = split_clue(clue);
	m_list return_v;
	
	for (unsigned i = 0; i < clue_words.size(); i++) {
	
		const std::string url = std::string("http://en.wikipedia.org/wiki/List_of_") + clue_words[i];
		
		std::string page_source = fetch_page(url);
		
		return_v.add(parse_wikilist(page_source, length));
	}

	return return_v;
}

std::string websearch::fetch_page(const std::string &url) {
	return g.grab_to_string(url);
}

std::vector<std::string> websearch::split_clue(const std::string &clue) {

	std::stringstream ss(clue);
	std::string tmp;
	std::vector<std::string> return_v;
	
	while (std::getline(ss, tmp, ' ' ))
		return_v.push_back(tmp);
	
	return return_v;
}

std::string websearch::int_to_string(int value) {

	std::stringstream ss;
	std::string return_v = "";
	
	ss << value;
	return_v += ss.str();
	
	return return_v;
}
