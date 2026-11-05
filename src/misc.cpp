#include "misc.hpp"
#include <getopt.h>
#include <iostream>

std::string trim_string(const std::string& string) {
    size_t first = string.find_first_not_of(" \t\r\n\\");
    if (first == std::string::npos) return "";
    
    size_t last = string.find_last_not_of(" \t\r\n\\)");
    return string.substr(first, (last - first + 1));
}

void panic(std::string info) {
	if(info == "1") exit(1);
	std::cerr << "ERROR: critical error occured.\nReason:" << info << std::endl;
	exit(1);
}

void handle_arguments(int* argc, char*** argv) {
	struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"verbose", no_argument, NULL, 'v'},
		{"url", required_argument, NULL, 'u'},
		{ NULL, 0, NULL, 0 }
	};
	
	int opt = 0;
	while((opt = getopt_long(*argc, *argv, "hvu:", long_options, NULL)) != -1){
		switch(opt){
			case 'v':
				DATA.debug = true;
				std::cout << "INFO: verbose mode is active." << std::endl;
				break;
			case 'u':
				DATA.url_from_arg = optarg;
				break;
			case 'h':
				const char *help =
					"Usage:\n"
					"  PackageManager [options]\n\n"

					"Options:\n"
					"  -h, --help\n"
					"      Shows this help message and exits.\n\n"

					"  -v --verbose\n"
					"      Runs PackageManager in verbose mode\n\n"

					"  -u, --url <link>\n"
					"      TEMP: query the PKGBUILD from <link>\n";

				std::cout << help << std::endl;
				break;
		}
	}
}

size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}
