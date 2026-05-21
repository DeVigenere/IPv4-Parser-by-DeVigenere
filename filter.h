#pragma once
class filter {
public:
	~filter() = default;
	virtual bool match(const std::string& ip) const = 0;
	virtual std::string get_type() const = 0;
};