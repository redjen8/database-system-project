#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <bitset>
#include "record.h"

Record::Record(std::vector<std::string> input, column_info column_meta) {
	int null_bitmap_mask = 0x00000000;
	int epbit = 1;
	for (int i = 0; i < column_meta.column_type.size(); i++)
	{
		if (input[i].compare("") == 0 || input[i].length() == 0)
		{
			// column has null values
			epbit *= 2;
			continue;
		}
		if (column_meta.column_type[i])
		{
			// if column has variable length
			var_len_column.push_back(input[i]);
		}
		else
		{
			fixed_len_column.push_back(input[i]);
		}
		null_bitmap_mask += epbit;
		epbit *= 2;
	}
	null_bitmap = ~null_bitmap_mask;
}

void Record::print_record()
{
	std::cout << "null bitmap : " << std::bitset<32>(null_bitmap) << std::endl;
	std::cout << "fixed_len_column : " << std::endl;
	int epbit = 1;
	for (int i = 0; i < fixed_len_column.size(); i++)
	{
		if (null_bitmap & epbit) std::cout << "NULL" << std::endl;
		std::cout << fixed_len_column[i] << std::endl;
		epbit *= 2;
	}
	std::cout << "var_len_column : " << std::endl;
	for (int i = 0; i < var_len_column.size(); i++)
	{
		if (null_bitmap & epbit) std::cout << "NULL" << std::endl;
		std::cout << var_len_column[i] << std::endl;
		epbit *= 2;
	}
	std::cout << std::endl;
}

int Record::get_record_size()
{
	int record_length = 0;
	for (int i = 0; i < fixed_len_column.size(); i++)
	{
		record_length += fixed_len_column[i].size();
	}
	for (int i = 0; i < var_len_column.size(); i++)
	{
		record_length += var_len_column[i].size();
	}
	record_length += sizeof(location_meta_data) * var_len_column.size();
	record_length += sizeof(null_bitmap);
	return record_length;
}

int Record::get_null_bitmap()
{
	return null_bitmap;
}

std::vector<std::string>* Record::get_fixed_column_list()
{
	return &fixed_len_column;
}

std::vector<std::string>* Record::get_var_column_list()
{
	return &var_len_column;
}

const char* Record::print_to_disk()
{
	std::string result = std::to_string(null_bitmap);
	result.append("");
	return "";
}