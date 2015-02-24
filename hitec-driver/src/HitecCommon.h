#ifndef HITEC_COMMON_H_
#define HITEC_COMMON_H_

#include <exception>
#include <string>

struct HitecConfiguration {

	std::string uart_port;
	unsigned int uart_speed;

};

class HitecUnsupportedCommandException: public std::exception {

};

class HitecSerialException: public std::exception {

};


#endif
