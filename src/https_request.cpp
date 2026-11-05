#include "https_request.hpp"
#include "misc.hpp"

#include <string>
#include <iostream>
#include <curl/curl.h>
#include <curl/easy.h>
#define CURL_ON

https_request::https_request(){}

https_request::https_request(std::string url){
	this->url = url;
}

https_request::~https_request(){}

std::string https_request::perform(const std::string& url){
	this->url = url;
	
	if(DATA.debug)
		std::cout << "Performing curl at " << url << std::endl;

	CURL *curl;
	CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
	if(result != CURLE_OK)
		panic("https request failed. curl error code != CURLE_OK");
	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		std::string read_buffer;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
		result = curl_easy_perform(curl);

		if(result != CURLE_OK){
			std::cerr << "ERROR: curl_perform() failed: " << curl_easy_strerror(result) << std::endl;
		} else if (DATA.debug){
			std::cout << "Raw Curl response received:\n" << read_buffer << std::endl;
		}
		curl_easy_cleanup(curl);
		return read_buffer;
	}
	return "ERROR: error while performing network request.";
}
