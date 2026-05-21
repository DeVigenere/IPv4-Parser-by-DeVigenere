#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include "subnet_filter.h"
#include "range_filter.h"

class factory_filter {
	std::unordered_map<std::string, std::function<std::unique_ptr<filter>(const std::string&)>> creators;
public:
	factory_filter() { //constructor. create just two types, but if we need more - we can add here
		creators["subnet"] = [](const std::string& val) { 
			return std::make_unique<subnet_filter>(val);//call when we push value. and here we call constructor step four
			};
		creators["range"] = [](const std::string& val) {
			return std::make_unique<range_filter>(val);//call when we push value. and here we call constructor step four
			};
	}

	std::unique_ptr<filter> create(const std::string& type, const std::string& value) { 
		auto iterator = creators.find(type); //here we find type
		if (iterator == creators.end()) {
			throw std::runtime_error("unknown filter");
		}
		return iterator->second(value);//and here we add to type value
	}

};