#include <iostream>
#include <math.h>
#include "lfsr_lib.h"

lfsr_lib::lfsr_lib(std::vector<bool> src, int seed) {
	this->polynom = src;
	this->degree = (int)this->polynom.size();
	int dec = pow(2, (this->degree - 1));
	this->start_value = (seed % dec);
	this->cur_value = this->start_value;
}

lfsr_lib::lfsr_lib(const lfsr_lib& lfsr) {
	this->polynom = lfsr.polynom;
	this->degree = lfsr.degree;
	this->cur_value = lfsr.cur_value;
	this->start_value = lfsr.start_value;
	this->period = lfsr.period;
}

lfsr_lib::~lfsr_lib() {
	this->polynom.clear();
}

void lfsr_lib::shift() {
	if (this->polynom.empty())
		return;

	unsigned int bit = 0;
	for (unsigned int i = 0; i < this->degree; i++) {
		if (this->polynom[i])
            bit = bit ^ (this->cur_value >> (this->degree - i - 1));
			//bit = bit ^ (this->cur_value >> i);
	}
	bit &= 1u;

    this->cur_value = (this->cur_value >> 1) | (bit << (this->degree - 2));
	//this->cur_value = (this->cur_value << 1) | bit;
	//this->cur_value &= ~(1 << this->degree - 1);
	this->period++;
	if (this->cur_value == this->start_value)
		this->period = 1;
}

bool lfsr_lib::get_bit(int n) {
	//return ((this->cur_value >> (this->degree - 2)) & 1u);
	return ((this->cur_value >> n) & 1u);
}

int lfsr_lib::get_period() {
	return this->period;
}

unsigned int lfsr_lib::get_start_val() {
	return this->start_value;
}

unsigned int lfsr_lib::get_cur_val() {
	return this->cur_value;
}

unsigned int lfsr_lib::get_degree() {
	return this->degree - 1;
}
