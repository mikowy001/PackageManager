#include "package.hpp"
#include "misc.hpp"
#include "https_request.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;


Package::Package(){
	
	//aka compiled/PackageManager -> ../../
	fs::path pkgs_dir = DATA.path_to_binary.parent_path().parent_path();
	
	if(DATA.debug)
		debug("pkgs_dir from Package() constructor: " + pkgs_dir.string());
	

	fs::path this_pkg_dir = pkgs_dir/DATA.pkgname_from_arg;
	
	if(DATA.debug)
		std::cout << "DEBUG: this_pkg_dir from Package() constructor: " << this_pkg_dir << std::endl;

	if (fs::exists(this_pkg_dir) && fs::is_directory(this_pkg_dir)){
		if(DATA.debug)
			std::cout << "DEBUG: package directory exists." << std::endl;
		path_to_pkgbuild = this_pkg_dir/"PKGBUILD";
	} else {
		
		create_safe_directory(this_pkg_dir);
		path_to_pkgbuild = this_pkg_dir/"PKGBUILD";
		//panic(" " + this_pkg_dir.string() + " directory doesn't exist and unable to create it");
	}

	
	//PKGBUILD curling from netwwwoork
	https_request request;

	//request.perform();

    std::ifstream file(path_to_pkgbuild);
    if(!file.is_open()){
        std::cerr << "ERROR: Could not open file '" << path_to_pkgbuild << "'\n";
        panic("1");
    }

	std::string line = "";
    std::string current_key = "";
    bool in_array = false;
    
    while(std::getline(file, line)) {
        size_t start_pos = line.find_first_not_of(" \t");
        
        if(start_pos == std::string::npos || line[start_pos] == '#')
             continue;
        
        std::string trimmed_line = line.substr(start_pos);
        
        if(!in_array) {
            for (const auto& field : package_fields){
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
                                add_array_item(current_key, cleaned);
                            }
                            in_array = false;
                        }
                    } else {
                        set_scalar_field(current_key, trim_string(val));
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
                    add_array_item(current_key, item);
                }
            }
        }
    }
}
Package::~Package(){}

	
void Package::print_info(){
	std::cout << "\nInfo about package: " << this->pkgname;
	std::cout << "\npackage version: " << this->pkgver;
	std::cout << "\npackage rel: " << this->pkgrel;
	std::cout << "\npackage description: " << this->pkgdesc;
	for(auto& thing : arch) std::cout << "\narchitecture: " << thing;
	for(auto& thing : license) std::cout << "\nlicense: " << thing;
	for(auto& thing : depends) std::cout << "\ndependencies: " << thing;
	for(auto& thing : makedepends) std::cout << "\nmake dependencies: " << thing;
	for(auto& thing : optdepends) std::cout << "\nopt dependencies: " << thing;
	for(auto& thing : source) std::cout << "\nsources:: " << thing;
	for(auto& thing : raw_sources) std::cout << "\nraw_sources:: " << thing;
	for(auto& thing : sha256sums) std::cout << "\nsha256sums: " << thing << std::endl;
}	



void Package::set_scalar_field(const std::string& key, const std::string& val) {
    if (key == "pkgname") pkgname = val;
    else if (key == "pkgver") pkgver = val;
    else if (key == "pkgrel") pkgrel = val;
    else if (key == "pkgdesc") pkgdesc = val;
    else if (key == "url") url = val;
}

void Package::add_array_item(const std::string& key, const std::string& item) {
    if (key == "arch") arch.push_back(item);
    else if (key == "license") license.push_back(item);
    else if (key == "depends") depends.push_back(item);
    else if (key == "makedepends") makedepends.push_back(item);
    else if (key == "optdepends") optdepends.push_back(item);
    else if (key == "source") source.push_back(item);
    else if (key == "sha256sums") sha256sums.push_back(item);
}
