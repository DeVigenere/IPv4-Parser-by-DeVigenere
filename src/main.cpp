#include "create_filter.h"
#include "process_stream.h"

//step by step description

int main() {
	auto filter = create_filter( {range("10.0.0.1-10.0.0.255") }); //first of all create_filter. step one
	process_stream("test.txt", "output.txt", *filter);
}