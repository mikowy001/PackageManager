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
		Package package;
		std::cout << "Succesfuly downloaded package " << DATA.pkgname_from_arg << std::endl;
	}
	
	
    return 0;
}
