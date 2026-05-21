#pragma once
#include <string>
class filter {
public:
	virtual ~filter() = default;
	virtual bool match(const std::string& ip) const = 0;
	virtual std::string get_type() const = 0;
};