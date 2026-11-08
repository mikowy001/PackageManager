#include <cstring>
#include <vector>
#include <getopt.h>

#include "package.hpp"
#include "misc.hpp"
#include "https_request.hpp"

Data DATA;

int main(int argc, char *argv[]) {
	handle_arguments(&argc, &argv);
    
	if(DATA.pkgname_from_arg != ""){ 
		std::vector<Package> packages = { Package() };
		packages[0].print_info();
	}

	
	
    return 0;
}
