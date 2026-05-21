#include <string>
#include <memory>
#include "filter.h"
#include "factory_filter.h"
#include "and_filter.h"
#include <stdexcept>
//inline for visibility in the main
inline std::unique_ptr<filter> create_filter(const std::initializer_list<std::initializer_list<std::string>>& list) { 
	static factory_filter factory; //to avoid creating it every time you call a function. and create object class step two
	if (list.size() == 0) {
		throw std::runtime_error("empty filter list");
	}
	if (list.size() == 1) { //if one, then we immediately create a filter through the factory
		const auto& filt = *list.begin();
		if (filt.size() != 4 || *filt.begin() != "type" || *(filt.begin() + 2) != "value") {
			throw std::runtime_error("incorrect type filter");
		}
		std::string type = *(filt.begin() + 1);
		std::string value = *(filt.begin() + 3);
		return factory.create(type, value); //step three for one filter
	}
	auto and_filter_ptr = std::make_unique<and_filter>();
	for (const auto& f : list) {
		if (f.size() != 4 || *f.begin() != "type" || *(f.begin() + 2) != "value") {
			throw std::runtime_error("incorrect type filter");
		}
		std::string type = *(f.begin() + 1);
		std::string value = *(f.begin() + 3);
		and_filter_ptr->add_filter(factory.create(type, value)); //for many filters
	}
	return and_filter_ptr;
}