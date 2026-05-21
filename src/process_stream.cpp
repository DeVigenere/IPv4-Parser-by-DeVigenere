#include "process_stream.h"
#include <string>
#include <sstream>
#include <fstream>
#include "filter.h"
#include <vector>


void process_stream(const std::string& input_file, const std::string& output_file, filter& filt) {
	std::ifstream in_file(input_file);
	if (!in_file.is_open()) {
		throw std::runtime_error("cannot open input file");
	}
	std::ofstream out_file(output_file);
	const size_t BUFFER_SIZE = 1000;
	std::vector<std::string> buffer;
	buffer.reserve(BUFFER_SIZE);

	std::string line;
	while (std::getline(in_file, line)) {
		if (line.empty()) continue;

		size_t stick = line.find('-');
		if (stick == std::string::npos) continue;
		std::string ip = line.substr(0, stick);
		if (filt.match(ip)) {
			buffer.push_back(line);
			if (buffer.size() >= BUFFER_SIZE) {
				for (const auto& i : buffer) {
					out_file << i << std::endl;
				}
				buffer.clear();
			}
		}
	}
	for (const auto& i : buffer) {
		out_file << i << std::endl;
	}
}