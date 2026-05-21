#pragma once
#include <stdexcept>
#include <sstream>
#include "filter.h"
#include "ip_to_uint32.h"
#include "filter_type.h"
class subnet_filter : public filter {
	std::uint32_t subnet_ip;
	std::uint32_t subnet_mask;

public:
	subnet_filter(const std::string& CIDR) { //val from factory
		size_t slash = CIDR.find('/');
		if (slash == std::string::npos) {
			throw std::runtime_error("invalid format");
		}
		std::string ip = CIDR.substr(0, slash);
		if (ip.empty()) throw std::runtime_error("empty ip");
		if (slash + 1 >= CIDR.size()) throw std::runtime_error("miss mask");
		int mask = 0;
		try {
			mask = std::stoi(CIDR.substr(slash + 1));
		}
		catch (...) {
			throw std::runtime_error("mask is not a number");
		}
		if (mask < 0 || mask > 32) {
			throw std::runtime_error("innvalid mask");
		}
		try {
			subnet_ip = ip_to_uint32(ip);
		}
		catch (const std::exception& e) {
			throw std::runtime_error(e.what());
		}
		if (mask == 0) {
			subnet_mask = 0;
		}
		else if (mask == 32) {
			subnet_mask = 0xFFFFFFFF;
		}
		else {
			subnet_mask = ~((1U << (32 - mask)) - 1);
		}
		
	}
	bool match(const std::string& ip) const override {
		try {
			std::uint32_t ip_int = ip_to_uint32(ip);
			return (ip_int & subnet_mask) == (subnet_ip & subnet_mask);
		}
		catch (...) {
			return false;
		}
		
	}
	std::string get_type() const {
		return to_string(filter_type::subnet);
	}
};