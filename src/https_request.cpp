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

std::string https_request::get_result(){
	return result;
}

std::string https_request::perform(const std::string& url){
	this->url = url;
	result = "";
	
	if(DATA.debug)
		debug("Performing curl at " + url);

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

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "PackageManager/1.0");
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

		CURLcode code = curl_easy_perform(curl);

		long http_code = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

		curl_easy_cleanup(curl);

		if (code != CURLE_OK)
			panic(curl_easy_strerror(code));

		if (http_code != 200)
			panic("HTTP request failed. Status: " +
				  std::to_string(http_code));

		if (DATA.debug)
			debug("Raw Curl response received:\n" + read_buffer);
		
		curl_easy_cleanup(curl);
		return read_buffer;
		}
		panic("Error while performing https request via curl.");
	return "";
}
