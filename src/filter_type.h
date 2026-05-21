#pragma once
enum class filter_type {
	subnet,
	range,
	and_comb
};
//here we can add a new filters!
inline std::string to_string(filter_type type) {
	switch (type) {
	case filter_type::subnet: return "subnet";
	case filter_type::range: return "range";
	case filter_type::and_comb: return "and_combination";
	default: return "unknown";
	}
}