#include <cstring>
#include <filesystem>
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
	

	
	
    return 0;
}
