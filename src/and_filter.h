#pragma once
#include "filter.h"
#include <vector>
#include <memory>
#include "filter_type.h"

class and_filter : public filter {
	std::vector<std::unique_ptr<filter>> filters;
public:
	and_filter() = default;
	and_filter(const and_filter&) = delete;
	and_filter& operator=(const and_filter&) = delete;
	and_filter(and_filter&&) = default;
	and_filter& operator=(and_filter&&) = default;
	
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
	std::string get_type() const {
		return to_string(filter_type::and_comb);
	}
};