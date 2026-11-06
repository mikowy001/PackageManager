#include <string>

class https_request {
public:
	https_request();
	https_request(std::string url);
	~https_request();
	
	std::string get_result();

	std::string perform(const std::string& url);
private:
	std::string result;
	std::string url;
};
