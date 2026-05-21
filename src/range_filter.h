#pragma once
#include "ip_to_uint32.h"
#include <stdexcept>
#include <sstream>
#include "filter.h"
#include "filter_type.h"

class range_filter : public filter {
	std::uint32_t lower_bound;
	std::uint32_t upper_bound;

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
		try {
			std::uint32_t ip_int = ip_to_uint32(ip);
			return ip_int >= lower_bound && ip_int <= upper_bound;
		}
		catch (...) {
			return false;
		}

	}
	std::string get_type() const {
		return to_string(filter_type::range);
	}
		
};