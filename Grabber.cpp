#include "Grabber.hpp"
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>

std::string Grabber::grab_to_string(const std::string &url) {
	return grab(url);
}
		
std::string Grabber::grab(const std::string &url, int target) {
	return grab(url);
}
		
//Starts the curl grabbing process in a separate thread.
//That way we can timeout requests that take too long.
std::string Grabber::grab(const std::string &url) {

	g_para para(url);
	
	pthread_t mio;
	pthread_create(&mio, NULL, grab_thread, &para);
	
	int count = 0;
	int threshold = 60;
	
	while((!para.finished) && (count++ < threshold)) {
		sleep(1);
	}

	pthread_cancel(mio);

	return para.result;
}


void * Grabber::grab_thread(void *p) {

	g_para *q = (g_para*)p;

	q->result = q->gr.grab_to_string(q->url);
	
	q->finished = true;
	
	return '\0';
}


CURL *curl = NULL;

Grabber_wrapper::Grabber_wrapper() {

	//initialise curl
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_USERAGENT, 
		"Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.2.23)");
}

std::string Grabber_wrapper::grab_to_string(const std::string &url) {
	return grab(url, 0);
}

std::string Grabber_wrapper::grab(const std::string &url, int target) {

	std::string mi;

	if (!curl) {
		std::cout << "Curl not initialised." << std::endl;
		return "";
	}

			
	CURLcode res;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mi);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_string);

	res = curl_easy_perform(curl);

	return mi;
}

size_t Grabber_wrapper::write_data_string(char *ptr, size_t size, size_t nmemb, 
	std::string *stream) {

	int result = 0;

	if (stream != NULL) {

	    // Append the data to the buffer
	    stream->append(ptr, size * nmemb);
	    
	    result = size * nmemb;
	}

	return result;
}
