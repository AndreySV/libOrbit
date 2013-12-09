#ifndef _STOD_H_
#define _STOD_H_

#include <stdexcept>
#include <sstream>

namespace std {

	template<class T>
	T stoType(const std::string &valueAsString) {
		istringstream totalSString( valueAsString );
		T valueAsNumber;
		// maybe use some manipulators
		totalSString >> valueAsNumber;
		if(!totalSString)
			throw std::runtime_error("Error converting");
		return valueAsNumber;
	}


	inline double stod(const std::string& valueAsString) {
		return stoType<double>(valueAsString);
	}
	

	inline long   stol(const std::string& valueAsString) {
		return stoType<long>(valueAsString);
	}
	
}



#endif
