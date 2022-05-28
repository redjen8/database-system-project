#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <format>
#include <bitset>
#include "record.h"

Record::Record(std::vector<std::string> input, column_info column_meta) {
	int null_bitmap_mask = 0x00000000;
	int epbit = 1;
	int fixed_column_cnt = 0;
	int var_column_start_location = 8; // null bitmap 이후부터 시작
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
			fixed_len_column.push_back(make_pair(input[i], column_meta.fixed_column_length[fixed_column_cnt]));
			var_column_start_location += column_meta.fixed_column_length[fixed_column_cnt];
		}
		null_bitmap_mask += epbit;
		epbit *= 2;
	}
	null_bitmap = ~null_bitmap_mask;
	for (int i = 0; i < var_len_column.size(); i++)
	{
		var_len_column_loc.push_back(location_meta_data{var_column_start_location, 8});
		var_column_start_location += 8;
	}
}

void Record::print_record()
{
	std::cout << "null bitmap : " << std::format("{:x}", null_bitmap) << std::endl;
	std::cout << "fixed_len_column : " << std::endl;
	int epbit = 1;
	for (int i = 0; i < fixed_len_column.size(); i++)
	{
		if (null_bitmap & epbit) std::cout << "NULL" << std::endl;
		std::cout << fixed_len_column[i].first << std::endl;
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
		record_length += fixed_len_column[i].second;
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

std::vector<std::string> Record::get_fixed_column_list()
{
	std::vector<std::string> result;
	for (int i = 0; i < fixed_len_column.size(); i++)
	{
		result.push_back(fixed_len_column[i].first);
	}
	return result;
}

std::vector<std::string> Record::get_var_column_list()
{
	return var_len_column;
}

std::string Record::to_string()
{
	std::string result = std::format("{:x}", null_bitmap);
	for (int i = 0; i < fixed_len_column.size(); i++)
	{
		result.append(std::format("{0:>4}", fixed_len_column[i].first).c_str());
	}
	for (int i = 0; i < var_len_column_loc.size(); i++)
	{
		result.append(std::to_string(var_len_column_loc[i].offset));
		result.append(std::to_string(var_len_column_loc[i].length));
	}
	for (int i = 0; i < var_len_column.size(); i++)
	{
		result.append(var_len_column[i]);
	}
	return result;
}