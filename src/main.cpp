#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>
#include <curl/easy.h>

#include "./data.hpp"

#define CURL_ON

void print_info(Package& pkg){
	std::cout << "\nInfo about package: " << pkg.pkgname;
	std::cout << "\npackage version: " << pkg.pkgver;
	std::cout << "\npackage rel: " << pkg.pkgrel;
	std::cout << "\npackage description: " << pkg.pkgdesc;
	for(auto& thing : pkg.arch) std::cout << "\narchitecture: " << thing;
	for(auto& thing : pkg.license) std::cout << "\nlicense: " << thing;
	for(auto& thing : pkg.depends) std::cout << "\ndependencies: " << thing;
	for(auto& thing : pkg.makedepends) std::cout << "\nmake dependencies: " << thing;
	for(auto& thing : pkg.optdepends) std::cout << "\nopt dependencies: " << thing;
	for(auto& thing : pkg.source) std::cout << "\nsources:: " << thing;
	for(auto& thing : pkg.raw_sources) std::cout << "\nraw_sources:: " << thing;
	for(auto& thing : pkg.sha256sums) std::cout << "\nsha256sums: " << thing << std::endl;
}	

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

const std::vector<std::string> fields = {
    "pkgname", 
    "pkgver", 
    "pkgrel", 
    "pkgdesc",
    "url", 
    "arch", 
    "license", 
    "depends",
    "makedepends", 
    "optdepends", 
    "source", 
    "sha256sums"
};

void set_scalar_field(Package& pkg, const std::string& key, const std::string& val) {
    if (key == "pkgname") pkg.pkgname = val;
    else if (key == "pkgver") pkg.pkgver = val;
    else if (key == "pkgrel") pkg.pkgrel = val;
    else if (key == "pkgdesc") pkg.pkgdesc = val;
    else if (key == "url") pkg.url = val;
}

void add_array_item(Package& pkg, const std::string& key, const std::string& item) {
    if (key == "arch") pkg.arch.push_back(item);
    else if (key == "license") pkg.license.push_back(item);
    else if (key == "depends") pkg.depends.push_back(item);
    else if (key == "makedepends") pkg.makedepends.push_back(item);
    else if (key == "optdepends") pkg.optdepends.push_back(item);
    else if (key == "source") pkg.source.push_back(item);
    else if (key == "sha256sums") pkg.sha256sums.push_back(item);
}

std::string trim_string(const std::string& string) {
    size_t first = string.find_first_not_of(" \t\r\n\\");
    if (first == std::string::npos) return "";
    
    size_t last = string.find_last_not_of(" \t\r\n\\)");
    return string.substr(first, (last - first + 1));
}

int main(int argc, char *argv[]) {
    const std::string filename = "PKGBUILD";
	

    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr << "ERROR: Could not open file '" << filename << "'\n";
        return 1;
    }
    
    Package package;

    std::string line = "";
    std::string current_key = "";
    bool in_array = false;
    
    while(std::getline(file, line)) {
        size_t start_pos = line.find_first_not_of(" \t");
        
        if(start_pos == std::string::npos || line[start_pos] == '#')
             continue;
        
        std::string trimmed_line = line.substr(start_pos);
        
        if(!in_array) {
            for (const auto& field : fields){
                std::string prefix = field + "=";

                if(trimmed_line.rfind(prefix, 0) == 0) {
                    current_key = field;
                    std::string val = trimmed_line.substr(prefix.length());

                    if(val.find('(') != std::string::npos) {
                        in_array = true;
                        

                        size_t open_paren = val.find('(');
                        size_t close_paren = val.rfind(')');
                        
                        if(close_paren != std::string::npos && close_paren > open_paren) {
                            std::string single_line_items = val.substr(open_paren + 1, close_paren - open_paren - 1);
                            std::string cleaned = trim_string(single_line_items);
                            if(!cleaned.empty()) {
                                add_array_item(package, current_key, cleaned);
                            }
                            in_array = false;
                        }
                    } else {
                        set_scalar_field(package, current_key, trim_string(val));
                        current_key = "";
                    }
                    break;
                }
            }
        } else {
            bool closing_found = false;
            if(trimmed_line.find(')') != std::string::npos){
                closing_found = true;
                in_array = false;
            }

            if(!(trimmed_line == ")" || trimmed_line == ")\r")) {
                std::string item = trim_string(trimmed_line);
                if(!item.empty()) {
                    add_array_item(package, current_key, item);
                }
            }
        }
    }
	
	

	print_info(package);
	
	if(argc < 2){
		std::cout << "WARNING: You did not give an url as an argument so curl will not be run." << std::endl;		
		

	} else {
		std::string url = argv[1];
		
		std::cout << "Performing curl at " << url << std::endl;

		CURL *curl;
		CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
  		if(result != CURLE_OK)
    		return (int)result;
		curl = curl_easy_init();
		if(curl){
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			std::string read_buffer;
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
			result = curl_easy_perform(curl);

			if(result != CURLE_OK){
				std::cerr << "ERROR: curl_perform() failed: " << curl_easy_strerror(result) << std::endl;
			} else {
				std::cout << "Response received:\n" << read_buffer << std::endl;
			}
			curl_easy_cleanup(curl);
		}
	}
    return 0;
}
