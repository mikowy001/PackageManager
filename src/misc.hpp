#include <string>
#include <vector>

typedef struct {
	std::string url_from_arg;
	bool debug;
} Data;
extern Data DATA;


std::string trim_string(const std::string& string);

void panic(std::string info);

size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* userp);

void handle_arguments(int* argc, char*** argv);

const std::vector<std::string> package_fields = {
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
