#include "Log.cpp"

using namespace std;

std::string runCommand(const char *cmd) {
	std::array<char, 128> buffer;
	std::string result;
	int exitCode;
	try {
		#ifdef _WIN32
			std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen((std::string(cmd) + " 2>&1").c_str(), "r"), _pclose);
		#else
			std::unique_ptr<FILE, decltype(&pclose)> pipe(popen((std::string(cmd) + " 2>&1").c_str(), "r"), pclose);
		#endif
		if (!pipe) {
			return "popen() failed!";
		}
		while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
			result += buffer.data();
		}
		#ifdef _WIN32
			exitCode = _pclose(pipe.release());
		#else
			exitCode = pclose(pipe.release());
		#endif
		if (exitCode != 0) {
			Log::log("Command failed with exit code " + std::to_string(exitCode));
			return "error";
		}
	} catch (const std::exception& e) {
		Log::log("Error running command: " + std::string(e.what()));
		return "error";
	}
	return result;
}