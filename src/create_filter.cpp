#include "create_filter.h"
#include <string>
#include <memory>
#include "filter.h"
#include "factory_filter.h"
#include "and_filter.h"
#include <stdexcept>
#include "filter_type.h"

//inline for visibility in the main
std::unique_ptr<filter> create_filter(const std::vector<filter_rule>& list) {
	static factory_filter factory; //to avoid creating it every time you call a function. and create object class step two
	const size_t MAX_RULES = 20;
	if (list.size() > MAX_RULES) {
		throw std::runtime_error("too many filters");
	}
	if (list.empty()) {
		throw std::runtime_error("empty filter list");
	}
	if (list.size() == 1) { //if one, then we immediately create a filter through the factory
		const auto& filt = list[0];
		return factory.create(to_string(filt.type), filt.value); //step three for one filter
	}
	auto and_filter_ptr = std::make_unique<and_filter>();
	for (const auto& f : list) {
		and_filter_ptr->add_filter(factory.create(to_string(f.type), f.value)); //for many filters
	}
	return and_filter_ptr;
}