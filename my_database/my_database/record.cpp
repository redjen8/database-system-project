#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <bitset>
#include "record.h"


Record::Record(std::vector<std::string>* input_fixed, std::vector<std::string>* input_variable, column_meta column_info) {
	null_bitmap = 0x00000000;
	int epbit = 1;
	for (int i = 0; i < column_info.num_fixed + column_info.num_variable; i++)
	{	
		// slotted page로부터 column 정보를 받아 null_bitmap 초기화 필요
		if (true)
		{
			null_bitmap += epbit;
		}
		epbit *= 2;
	}
 	null_bitmap = ~null_bitmap;
	fixed_len_column = input_fixed;
	var_len_column = input_variable;
}

void Record::print_record()
{
	std::cout << "null bitmap : " << std::bitset<32>(null_bitmap) << std::endl;
	std::cout << "fixed_len_column : " << std::endl;
	for (std::vector<std::string>::const_iterator i = (*fixed_len_column).begin(); i != (*fixed_len_column).end(); i++)
	{
		std::cout << *i << std::endl;
	}
	std::cout << "var_len_column : " << std::endl;
	for (std::vector<std::string>::const_iterator i = (*var_len_column).begin(); i != (*var_len_column).end(); i++)
	{
		std::cout << *i << std::endl;
	}
}