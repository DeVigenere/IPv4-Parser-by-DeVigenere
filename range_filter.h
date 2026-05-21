#pragma once
#include <stdexcept>
#include <sstream>
#include "filter.h"
class range_filter : public filter {
	std::uint32_t lower_bound;
	std::uint32_t upper_bound;

	static std::uint32_t ip_to_uint32(const std::string& ip) {
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
public:
	range_filter(const std::string& ip) { //val from factory
		size_t stick = ip.find('-');
		if (stick == std::string::npos) {
			throw std::runtime_error("invalid format");
		}
		std::string lb = ip.substr(0, stick);
		std::string ub = ip.substr(stick + 1);
		lower_bound = ip_to_uint32(lb);
		upper_bound = ip_to_uint32(ub);
		if (lower_bound > upper_bound) {
			throw std::runtime_error("incorrect bounds");
		}
	}
	bool match(const std::string& ip) const override {
		std::uint32_t ip_int = ip_to_uint32(ip);
		return ip_int >= lower_bound && ip_int <= upper_bound;
	}
	std::string get_type() const override { return "range"; };

};