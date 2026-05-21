#include "create_filter.cpp"
#include "process_stream.cpp"

//step by step description

int main() {
	auto filter = create_filter({ //first of all create_filter. step one
  {"type", "subnet", "value", "192.168.0.0/24"},
		});
	process_stream("test.txt", "output.txt", *filter);
}