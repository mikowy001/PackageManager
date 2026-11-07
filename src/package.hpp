#include <filesystem>
#include <vector>
#include <string>

class Package {
public:
	Package();
	~Package();

	void print_info();
	void set_scalar_field(const std::string& key, const std::string& val);
	void add_array_item(const std::string& key, const std::string& item);

private:
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
	

	std::filesystem::path path_to_pkgbuild;
};
