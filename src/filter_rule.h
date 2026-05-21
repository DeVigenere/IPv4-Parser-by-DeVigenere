#pragma once
#include <string>
#include "filter_type.h"

struct filter_rule{
	filter_type type;
	std::string value;
};

inline filter_rule subnet(const std::string& CIDR) { //when we call function we need to enter filter-data
	return { filter_type::subnet, CIDR };
}

inline filter_rule range(const std::string& ip) { //when we call function we need to enter filter-data
	return { filter_type::range, ip };
}
// next we can add more filters!!