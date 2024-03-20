#include "Log.cpp"

using namespace std;

string runCommand(const char *cmd) {
	Log::log("cmd exec " + string(cmd));

	std::array<char, 128> buffer;
	std::string result;
	#ifdef _WIN32
		std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen((std::string(cmd) + " 2>&1").c_str(), "r"), _pclose);
	#else
		std::unique_ptr<FILE, decltype(&pclose)> pipe(popen((std::string(cmd) + " 2>&1").c_str(), "r"), pclose);
	#endif
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}