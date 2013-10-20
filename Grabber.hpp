#ifndef GRABBER_HPP
#define GRABBER_HPP

#include <string>
#include <unistd.h>

class Grabber {

	public:
		std::string grab_to_string(const std::string &url);
		std::string grab(const std::string &url, int target);
		std::string grab(const std::string &url);

	private:
		static void * grab_thread(void *p);

};

class Grabber_wrapper {

	public:
		Grabber_wrapper();
		std::string grab_to_string(const std::string &url);
		std::string grab(const std::string &url, int target);
		static size_t write_data_string(char *ptr, size_t size, 
			size_t nmemb, std::string *stream);

};

typedef struct g_para {

	Grabber_wrapper gr;
	std::string url;
	bool finished;
	std::string result;

	g_para(const std::string &u) : gr() {
	
		url = u;
		finished = false;
		result = "";
	}
} g_para;


#endif
