
#ifndef STD_UTIL_HPP
#define STD_UTIL_HPP

#include <string>
#include <iostream>

namespace std {
	inline std::wstring to_wstring(const std::string &string) {
		return std::wstring(string.begin(), string.end());
	}
	
	inline std::string to_string(const std::wstring &string) {
		return std::string(string.begin(), string.end());
	}
}

inline std::istream &GetLine(std::istream &stream, std::string &line) {
	std::getline(stream, line);
	if(line.size() && line[line.size()-1] == 13)
		line.resize(line.size() - 1);
	return stream;
}

inline std::string GetCoreName(const std::string &name) {
	std::string ret(name);
	int64_t s1 = ret.rfind("/");
	int64_t s2 = ret.rfind("\\");
	int64_t slash = std::max( (s1!=(int64_t)std::string::npos?s1:-1),
			(s2!=(int64_t)std::string::npos?s2:-1) );
	if(slash != -1)
		ret = ret.substr(slash+1);
	int64_t dot = ret.rfind(".");
	if(dot != (int64_t)std::string::npos)
		ret = ret.substr(0,dot);
	return ret;
}

inline std::string GetFileWithPathWithoutExtension(const std::string &name) {
	int64_t dot = name.rfind(".");
	if(dot == (int64_t)std::string::npos)
		return name;
	int64_t dot2 = name.rfind("/");
	int64_t dot3 = name.rfind("\\");
	if(dot2 != (int64_t)std::string::npos && dot2 > dot)
		return name;
	if(dot3 != (int64_t)std::string::npos && dot3 > dot)
		return name;
	return name.substr(0, dot);
}

inline std::string GetExtension(const std::string &name) {
	int64_t dot = name.rfind(".");
	if(dot == (int64_t)std::string::npos)
		return "";
	int64_t dot2 = name.rfind("/");
	int64_t dot3 = name.rfind("\\");
	if(dot2 != (int64_t)std::string::npos && dot2 > dot)
		return "";
	if(dot3 != (int64_t)std::string::npos && dot3 > dot)
		return "";
	return std::string(name.c_str() + dot + 1);
}

inline std::string GetPathWithoutSlash(const std::string &name) {
	int64_t dot = name.rfind("/");
	int64_t dot2 = name.rfind("\\");
	if(dot < dot2)
		dot = dot2;
	if(dot == (int64_t)std::string::npos)
		return name;
	return name.substr(0, dot);
}

inline std::string operator+(const char *cstring, const std::string &string) {
	return std::string(cstring) + string;
}

#endif
