#pragma once
#include <sstream>

inline static std::uint32_t ip_to_uint32(const std::string& ip) {  //convert function
	std::stringstream ss(ip);
	std::uint32_t result = 0;
	int octet;
	for (int i = 0; i < 4; ++i) {
		if (!(ss >> octet)) {
			throw std::runtime_error("not enough octets");
		}
		if (octet < 0 || octet > 255) {
			throw std::runtime_error("octet out of range");
		}
		result = (result << 8) | (octet & 0xFF);
		if (i < 3 && ss.peek() == '.') ss.ignore();
		else if (i < 3) {
			throw std::runtime_error("miss dotr");
		}
	}

	if (ss >> octet) {
		throw std::runtime_error("more symbols than we need");
	}

	return result;
}