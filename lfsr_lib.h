#pragma once
#include <iostream>
#include <vector>

class lfsr_lib {
private:
	unsigned int degree = 0;
    unsigned int start_value = 0;
	unsigned int cur_value = 0;
	unsigned int period = 1;
	std::vector<bool> polynom;

public:
	lfsr_lib(std::vector<bool> src, int seed);
	lfsr_lib(const lfsr_lib& lfsr);
	~lfsr_lib();
	void shift();
	bool get_bit(int n = 0);
	int get_period();
	unsigned int get_start_val();
	unsigned int get_cur_val();
	unsigned int get_degree();

};
