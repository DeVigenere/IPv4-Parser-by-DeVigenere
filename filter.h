#pragma once

class filter {
	subnet* sf;
	range* rf;
public:
	filter() : sf(nullptr), rf(nullptr) {};
	filter(std::string address, int masked) : sf(new subnet()), rf(nullptr) {
		sf->ip = address;
		sf->mask = masked;
	}
	filter(std::uint32_t lower, std::uint32_t upper) : sf(nullptr), rf(new range()) {
		rf->lower_bound = lower;
		rf->upper_bound = upper;
	}
	filter(std::string address, int masked, std::uint32_t lower, std::uint32_t upper)
		: sf(new subnet()), rf(new range()) {
		rf->lower_bound = lower;
		rf->upper_bound = upper;
		sf->ip = address;
		sf->mask = masked;
	}
	~filter() {
		if (sf) {
			delete sf;
		}
		if (rf) {
			delete rf;
		}
	}
	bool sf_exist() {
		return sf != nullptr ? true : false;
	}
	bool rf_exist() {
		return rf != nullptr ? true : false;
	}
	std::uint32_t get_lb() {
		return rf ? rf->lower_bound : 0;
	}
	std::uint32_t get_ub() {
		return rf ? rf->upper_bound : 0;
	}
	std::string get_ip_subnet() {
		return sf ? sf->ip : " ";
	}
	int get_mask() {
		return sf ? sf->mask : 0;
	}
};