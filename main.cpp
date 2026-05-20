#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <fstream>
#include "subnet.h"
#include "range.h"
#include "filter.h"
#include <vector>


filter create_filter(const std::initializer_list<std::string>& List) { //function only for one filter
	if (List.size() != 4) throw std::runtime_error ("not correct size");
	for (const auto& i : List) {
		if (i == "subnet") {
			std::string temp = *(List.begin() + 3);
			size_t slash = temp.find('/');
			std::string temp_ip = temp.substr(0, slash);
			int temp_mask = std::stoi(temp.substr(slash + 1));
			return filter(temp_ip, temp_mask);
		}
		else if (i == "range") {
			std::string temp = *(List.begin() + 3);
			size_t stick = temp.find('-');
			
			std::string temp_lb = temp.substr(0, stick), temp_ub = temp.substr(stick + 1);
			std::uint32_t lower = 0, upper = 0;
			std::stringstream ss_ub(temp_ub), ss_lb(temp_lb);
			int octet, octet2;
			for (auto i = 0; i < 4; ++i) {
				ss_lb >> octet;
				ss_ub >> octet2;
				if (ss_lb.peek() == '.') ss_lb.ignore();
				if (ss_ub.peek() == '.') ss_ub.ignore();
				lower = (lower << 8) | (octet & 0xFF);
				upper = (upper << 8) | (octet2 & 0xFF);
			}
			return filter(lower, upper);
		}
	}
	throw std::runtime_error("unknown filter type");
}

filter create_filter(const std::initializer_list<std::initializer_list<std::string>>& BigList) { //function for two filters
	std::string sub_ip;
	int mask = 0;
	std::uint32_t r_lower = 0, r_upper = 0;
	bool sub_ready = false, range_ready = false;
	
	for (const auto& i : BigList) {
		if (i.size() != 4) throw std::runtime_error("invalid size!");
		auto iterator = i.begin();
		std::string type = *iterator++;
		std::string filt = *iterator++;
		std::string val_text = *iterator++;
		std::string value = *iterator++;
		if (type != "type") throw std::runtime_error("invalid filter enter!");
		if (filt == "subnet") {
			size_t slash = value.find('/');
			sub_ip = value.substr(0, slash);
			mask = std::stoi(value.substr(slash + 1));
			sub_ready = true;
		}
		else if (filt == "range") {
			size_t stick = value.find('-');
			std::string temp_lb = value.substr(0, stick), temp_ub = value.substr(stick + 1);
			std::uint32_t lower = 0, upper = 0;
			std::stringstream ss_ub(temp_ub), ss_lb(temp_lb);
			int octet, octet2;
			for (auto i = 0; i < 4; ++i) {
				ss_lb >> octet;
				ss_ub >> octet2;
				if (ss_lb.peek() == '.') ss_lb.ignore();
				if (ss_ub.peek() == '.') ss_ub.ignore();
				lower = (lower << 8) | (octet & 0xFF);
				upper = (upper << 8) | (octet2 & 0xFF);
			}
			r_lower = lower;
			r_upper = upper;
			range_ready = true;
		} 
	}
	if (range_ready && sub_ready) {
		return filter(sub_ip, mask, r_lower, r_upper);
	}
	else {
		throw std::runtime_error("invalid enter filter");
	}
}

bool check_range(const std::string& ip, filter& filt) { //function for check ip-address by range
	std::stringstream ss(ip);
	std::uint32_t ip_32 = 0;
	int octet;
	for (auto i = 0; i < 4; ++i) {
		ss >> octet;
		if (ss.peek() == '.') ss.ignore();
		ip_32 = (ip_32 << 8) | (octet & 0xFF);
	}
	if (ip_32 >= filt.get_lb() && ip_32 <= filt.get_ub()) {
		return true;
	}
	else {
		return false;
	}
}

std::uint32_t ip_to_uint32(const std::string& ip) { //function for convert std::string to std::uint32_t 
	std::stringstream ss(ip);
	std::uint32_t result = 0;
	int octet;
	for (int i = 0; i < 4; ++i) {
		ss >> octet;
		if (ss.peek() == '.') ss.ignore();
		result = (result << 8) | (octet & 0xFF);
	}
	return result;
}

bool check_subnet(const std::string& ip, filter& filt) { //function for check ip-address by mask
	std::uint32_t ip_int = ip_to_uint32(ip);
	std::uint32_t subnet_int = ip_to_uint32(filt.get_ip_subnet());
	int mask = filt.get_mask();

	if (mask < 0 || mask > 32) return false;

	uint32_t mask_bits = (mask == 32) ? 0xFFFFFFFF : ~((1 << (32 - mask)) - 1);

	return (ip_int & mask_bits) == (subnet_int & mask_bits);
}

void process_stream(const std::string& input_file, const std::string& output_file,filter& filt) { //function for read and write correct ip
	std::string line;
	std::ifstream in_file(input_file);
	if (!in_file.is_open()) {
		std::cerr << "cannot open file!";
		return;
	}
	std::ofstream out_file(output_file);
	const size_t BUFFER_SIZE = 1000;
	std::vector<std::string> buffer;
	buffer.reserve(BUFFER_SIZE);

	while (std::getline(in_file, line)) {
		if (line.empty()) {
			continue;
		}
		size_t stick = line.find('-');
		std::string ip = line.substr(0, stick);
		bool correct = false;
		if (filt.rf_exist() && filt.sf_exist()) { //both filter
			correct = check_range(ip, filt) && check_subnet(ip, filt);
		}
		else if (filt.rf_exist()) { //range filter
			correct = check_range(ip, filt);
		}
		else if (filt.sf_exist()) {//subnet filter
			correct = check_subnet(ip, filt);
		}
		if (correct) {
			buffer.push_back(line); //buffering
			if (buffer.size() >= BUFFER_SIZE) {
				for (const auto& buffered_line : buffer) {
					out_file << buffered_line << std::endl;
				}
				buffer.clear();
			}
		}
	}
	for (const auto& buffered_line : buffer) {
		out_file << buffered_line << std::endl;
	}
}

int main() {
	auto filter = create_filter({ "type", "range", "value", "10.0.0.1-10.0.0.255" });

	process_stream("test.txt", "output.txt", filter);
}