#pragma once
#include "filter.h"
class and_filter : public filter {
	std::vector<std::unique_ptr<filter>> filters;
public:
	void add_filter(std::unique_ptr<filter> filter) { //add filters here
		filters.push_back(std::move(filter));
	}
	bool match(const std::string& ip) const override {
		for (const auto& i : filters) {
			if (!i->match(ip)) {
				return false;
			}
		}
		return true;
	}
	std::string get_type() const override { return "and-combination"; };
};