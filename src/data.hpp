#include <string>
#include <vector>

typedef struct {
	std::string pkgname;
	std::string pkgver;
	std::string pkgrel;
	std::string pkgdesc;
	std::string url;
	
	std::vector<std::string> arch;
	std::vector<std::string> license;
	std::vector<std::string> depends;
	std::vector<std::string> makedepends;
	std::vector<std::string> optdepends;
	std::vector<std::string> source;
	std::vector<std::string> raw_sources;
	std::vector<std::string> sha256sums;
} Package;
