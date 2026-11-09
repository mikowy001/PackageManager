#include "misc.hpp"
#include "https_request.hpp"
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <filesystem>

std::string trim_string(const std::string& string) {
    size_t first = string.find_first_not_of(" \t\r\n\\");
    if (first == std::string::npos) return "";
    
    size_t last = string.find_last_not_of(" \t\r\n\\)");
    return string.substr(first, (last - first + 1));
}

void panic(std::string info) {
	if(info == "1") exit(1);
	std::cerr << 
		Color::BOLD_RED << "ERROR:" << Color::RESET 
		<< " Critical error occured.\n" <<
		Color::BOLD_RED << "ERROR:" << Color::RESET << " Reason:" << info << std::endl;
	exit(1);
}

void debug(std::string info) {
	std::clog <<
		Color::CYAN << "DEBUG:" << Color::RESET
		<< info << std::endl;
}

void warning(std::string info) {
	std::cout << Color::ORANGE << "WARNING: " << Color::RESET
	<< info << std::endl;
}

void handle_arguments(int* argc, char*** argv) {
	struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"verbose", no_argument, NULL, 'v'},
		{"query", required_argument, NULL, 'q'},
		{ NULL, 0, NULL, 0 }
	};
	
	int opt = 0;
	while((opt = getopt_long(*argc, *argv, "hvq:", long_options, NULL)) != -1){
		switch(opt){
			case 'h': {
				const char *help =
					"Usage:\n"
					"  PackageManager [options]\n\n"

					"Options:\n"
					"  -h, --help\n"
					"      Shows this help message and exits.\n\n"

					"  -v --verbose\n"
					"      Runs PackageManager in verbose mode\n\n"

					"  -q, --query <package name>\n"
					"      TEMP: query the PKGBUILD by name\n";

				std::cout << help << std::endl;
				exit(0);
			}
			case 'v':
				DATA.debug = true;
				std::cout << Color::GREEN << "INFO:" << Color::RESET <<
					" Verbose mode is active." << std::endl;
				break;
			case 'q':
				DATA.pkgname_from_arg = optarg;
				break;

			default:
				break;
		}
	}

	std::filesystem::path program_path = std::filesystem::absolute(**argv);
	DATA.path_to_binary = program_path;
}


bool create_safe_directory(const std::filesystem::path& dir_path){
	std::error_code erc;

	bool created = std::filesystem::create_directories(dir_path, erc);

	if(erc){
		panic(
			"Cannot create directory in " + dir_path.string() + 
			" Reason: " + erc.message() + "\n");
		return false;
	}
	if(created){
		if(DATA.debug) 
			debug("Successfully created a directory in: " + dir_path.string());
	} else {
		if(std::filesystem::is_directory(dir_path)){
			if(DATA.debug)
				debug("Directory already exists while trying to create " + dir_path.string() + " directory.");
			return false;
		} else {
			debug("Path " + dir_path.string() + " already exists but is a file.");
			return false;
		}
	}
	return true;
}

bool save_string_to_file(const std::string& content, const std::filesystem::path& path){
	std::ofstream file(path);
	if(!file.is_open())
		return false;
	file << content;
	file.close();
	if(!file){
		panic("Error occured while writing/closing a file at " + path.string());
	}
	if(DATA.debug)
		debug("Created and wrote to file " + path.string());
	return true;
}


size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

void download_file_http(const std::string& url, const std::filesystem::path& path){
	https_request request;
	std::string response = request.perform(url);
	save_string_to_file(response, path);
}
