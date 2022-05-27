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
		// slotted page�κ��� column ������ �޾� null_bitmap �ʱ�ȭ �ʿ�
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

int Record::get_record_size()
{
	int record_length = 0;
	for (int i = 0; i < (*fixed_len_column).size(); i++)
	{
		record_length += (*fixed_len_column)[i].size();
	}
	for (int i = 0; i < (*var_len_column).size(); i++)
	{
		record_length += (*var_len_column)[i].size();
	}
	record_length += sizeof(column_meta);
	return record_length;
}