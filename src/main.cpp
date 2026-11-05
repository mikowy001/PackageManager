#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <getopt.h>

#include "package.hpp"
#include "misc.hpp"
#include "https_request.hpp"

Data DATA;

int main(int argc, char *argv[]) {
	handle_arguments(&argc, &argv);
    
	std::vector<Package> packages = { Package() };
	
	packages[0].print_info();
	

	if(DATA.url_from_arg == ""){
		std::cout << "WARNING: You did not give an url as an argument so curl will not be run." << std::endl;		
	} else {
		https_request request;
		request.perform(DATA.url_from_arg);
	}
    return 0;
}
