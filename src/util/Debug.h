
//	This file is part of The Drwalin Engine project
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <sstream>
#include <string>

#ifdef ENGINE_CPP
	int UseDebugVaraibleStorage = 0;
#else
	extern "C" int UseDebugVaraibleStorage;
#endif

namespace Debug {
	inline void UseDebug(bool enable) {
		UseDebugVaraibleStorage = enable ? 1 : 0;
	}
	inline void SwitchDebugState() {
		UseDebugVaraibleStorage = UseDebugVaraibleStorage ? 0 : 1;
	}
	inline bool GetDebug() {
		return UseDebugVaraibleStorage;
	}
	void PushMessage(const std::string& str);
}

template <class T >
inline std::string MethodName(const std::string& prettyFunction) {
	std::string ret = prettyFunction;
	size_t index = 0;
	
	while(true) {
		index = ret.find("::__cxx11");
		if(index == std::string::npos)
			break;
		ret.replace(index, 9, "");
	}
	
	index = ret.find(" [");
	if(index != std::string::npos)
		ret.resize(index);
	
	return ret;
}
#define __METHOD_NAME__ (MethodName<int>(__PRETTY_FUNCTION__))

#define DEBUG(x) if(UseDebugVaraibleStorage) { \
	std::cerr << " " << __METHOD_NAME__ << "(" << (x) << ") " << \
			__FILE__ << ":" << __LINE__ << "\n"; \
	std::cerr.flush(); \
}

#define MESSAGE(x) { \
	std::stringstream ss; \
	ss << " " << __METHOD_NAME__ << "(" << (x) << ") " << __FILE__ << ":" << \
			__LINE__ << "\n"; \
	Debug::PushMessage(ss.str()); \
	std::cerr << ss.str(); \
	std::cerr.flush();\
}

#endif

