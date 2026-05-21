#pragma once
#include <string>

struct filter_rule{
	std::string type;
	std::string value;
};

inline filter_rule subnet(const std::string& CIDR) { //when we call function we need to enter filter-data
	return { "subnet", CIDR };
}

inline filter_rule range(const std::string& ip) { //when we call function we need to enter filter-data
	return { "range", ip };
}
// next we can add more filters!!