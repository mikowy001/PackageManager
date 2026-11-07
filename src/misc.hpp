#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>

typedef struct {
	std::string pkgname_from_arg;
	bool debug;
	std::filesystem::path path_to_binary;
} Data;
extern Data DATA;


std::string trim_string(const std::string& string);

void panic(std::string info);

void debug(std::string info);

size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* userp);

void handle_arguments(int* argc, char*** argv);

bool create_safe_directory(const std::filesystem::path& dir_path);

namespace Color {
    inline constexpr std::string_view RESET     = "\033[0m";
    inline constexpr std::string_view RED       = "\033[31m";
    inline constexpr std::string_view BOLD_RED  = "\033[1;31m";
    inline constexpr std::string_view YELLOW    = "\033[33m";
    inline constexpr std::string_view GREEN     = "\033[32m";
	inline constexpr std::string_view CYAN		= "\x1b[36m";
}

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
