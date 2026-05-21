#pragma once
#include <string>
class filter {
public:
	filter() = default;
	filter(const filter&) = default;
	filter& operator=(const filter&) = default;
	filter(filter&&) = default;
	filter& operator=(filter&&) = default;
	
	virtual ~filter() = default;
	virtual bool match(const std::string& ip) const = 0;
	virtual std::string get_type() const = 0;
};