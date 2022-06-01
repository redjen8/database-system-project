#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <format>
#include <bitset>
#include "record.h"
#include "byte_convert.h"
#include "meta_data.h"

Record::Record()
{
	null_bitmap = 0xFFFFFFFF;
}

Record::Record(unsigned char* byte_arr, int arr_length, column_info column_meta)
{
	null_bitmap = 0x00000000;
	fixed_len_column.clear();
	var_len_column.clear();
	var_len_column_loc.clear();
	record_byte_arr.clear();
	byte_arr_size = 0;

	print_byte_arr(byte_arr, arr_length);
	int byte_cursor = 0;
	unsigned int column_length = column_meta.column_type.size();
	// null bitmap 추출
	unsigned char* null_bitmap_byte = new unsigned char[4];
	for (int i = 0; i < 4; i++)
	{
		null_bitmap_byte[i] = byte_arr[byte_cursor];
		byte_cursor++;
	}
	null_bitmap = byte_arr_to_int(null_bitmap_byte);
	
	std::vector<bool> is_column_null_arr;
	int epbit = 0x00000001;
	for (int i = 0; i < column_length; i++)
	{
		if (epbit & null_bitmap) is_column_null_arr.push_back(true);
		else is_column_null_arr.push_back(false);
		epbit *= 2;
	}

	int column_number = 0;
	//고정 길이 칼럼 추출
	for (; column_number < column_meta.fixed_column_length.size(); column_number++)
	{
		if (is_column_null_arr[column_number]) continue;
		int byte_cnt = column_meta.fixed_column_length[column_number];
		unsigned char* fixed_column_byte = new unsigned char[byte_cnt+1];
		for (int j = 0; j < column_meta.fixed_column_length[column_number]; j++)
		{
			fixed_column_byte[j] = byte_arr[byte_cursor];
			byte_cursor++;
		}
		fixed_column_byte[byte_cnt] = '\0';
		std::string fixed_len_column_string = static_cast<std::string>(reinterpret_cast<const char*>(fixed_column_byte));
		fixed_len_column.push_back(make_pair(fixed_len_column_string, fixed_len_column_string.length()));
	}

	//가변 길이 컬럼 추출
	while (column_number < column_length)
	{
		if (is_column_null_arr[column_number])
		{
			column_number++;
			continue;
		}
		unsigned char* var_column_offset = new unsigned char[4];
		for (int i = 0; i < 4; i++)
		{
			var_column_offset[i] = byte_arr[byte_cursor];
			byte_cursor++;
		}
		int offset = byte_arr_to_int(var_column_offset);
		unsigned char* var_column_length = new unsigned char[4];
		for (int i = 0; i < 4; i++)
		{
			var_column_length[i] = byte_arr[byte_cursor];
			byte_cursor++;
		}
		int length = byte_arr_to_int(var_column_length);
		var_len_column_loc.push_back(location_meta_data{offset, length});
		unsigned char* column_string = new unsigned char[length+1];
		for (int i = 0; i < length; i++)
		{
			column_string[i] = byte_arr[offset + i];
		}
		column_string[length] = '\0';
		var_len_column.push_back(static_cast<std::string>(reinterpret_cast<const char*>(column_string)));
		column_number++;
	}
	byte_arr_size = get_record_size();
}

Record::Record(std::vector<std::string> input, column_info column_meta) {
	int null_bitmap_mask = 0x00000000;
	int epbit = 1;
	int fixed_column_cnt = 0;
	int var_column_start_location = 4; // null bitmap 이후부터 시작
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
			fixed_column_cnt++;
		}
		null_bitmap_mask += epbit;
		epbit *= 2;
	}
	var_column_start_location += var_len_column.size() * 8;
	null_bitmap = ~null_bitmap_mask;
	for (int i = 0; i < var_len_column.size(); i++)
	{
		var_len_column_loc.push_back(location_meta_data{ var_column_start_location, (int)var_len_column[i].length()});
		var_column_start_location += var_len_column[i].length();
	}
	record_byte_arr = to_byte_vector();
	byte_arr_size = record_byte_arr.size();
}

void Record::print_record()
{
	std::cout << "null bitmap : " << std::format("{:x}", null_bitmap) << std::endl;
	std::cout << "fixed_len_column : " << std::endl;
	int epbit = 1;
	for (unsigned int i = 0; i < fixed_len_column.size(); i++)
	{
		if (null_bitmap & epbit) std::cout << "NULL" << std::endl;
		std::cout << fixed_len_column[i].first << std::endl;
		epbit *= 2;
	}
	std::cout << "var_len_column : " << std::endl;
	for (unsigned int i = 0; i < var_len_column.size(); i++)
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
	for (unsigned int i = 0; i < fixed_len_column.size(); i++)
	{
		record_length += fixed_len_column[i].second;
	}
	for (unsigned int i = 0; i < var_len_column.size(); i++)
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
	for (unsigned int i = 0; i < fixed_len_column.size(); i++)
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
	for (unsigned int i = 0; i < fixed_len_column.size(); i++)
	{
		result.append(std::format("{0:>4}", fixed_len_column[i].first).c_str());
	}
	for (unsigned int i = 0; i < var_len_column_loc.size(); i++)
	{
		result.append(std::format("{0:>4}", std::to_string(var_len_column_loc[i].offset)));
		result.append(std::format("{0:>4}", std::to_string(var_len_column_loc[i].length)));
	}
	for (unsigned int i = 0; i < var_len_column.size(); i++)
	{
		result.append(var_len_column[i]);
	}
	return result;
}

std::vector<unsigned char> Record::to_byte_vector()
{
	int record_size = get_record_size();
	std::vector<unsigned char> record_byte_arr;
	std::vector<unsigned char> null_bitmap_byte = int_to_byte(null_bitmap);
	for (int i = 0; i < 4; i++)
	{
		record_byte_arr.push_back(null_bitmap_byte[i]);
	}
	for (unsigned int i = 0; i < fixed_len_column.size(); i++)
	{
		if (fixed_len_column[i].first.length() > fixed_len_column[i].second)
		{
			//고정 길이 컬럼 문자열이 컬럼 최대 문자열 길이보다 길 경우 예외 처리
		}

		for (unsigned int j = 0; j < fixed_len_column[i].second; j++)
		{
			if (fixed_len_column[i].first.length() < fixed_len_column[i].second)
			{
				int diff = fixed_len_column[i].second - fixed_len_column[i].first.length();
				if (j < diff)
				{
					record_byte_arr.push_back(0);
				}
				else
				{
					record_byte_arr.push_back(fixed_len_column[i].first.at(j-diff));
				}
				continue;
			}
			record_byte_arr.push_back(fixed_len_column[i].first.at(j));
		}
	}
	for (unsigned int i = 0; i < var_len_column.size(); i++)
	{
		std::vector<unsigned char> offset_byte_arr = int_to_byte(var_len_column_loc[i].offset);
		std::vector<unsigned char> length_byte_arr = int_to_byte(var_len_column_loc[i].length);
		for (int j = 0; j < 4; j++)
		{
			record_byte_arr.push_back(offset_byte_arr[j]);
		}
		for (int j = 0; j < 4; j++)
		{
			record_byte_arr.push_back(length_byte_arr[j]);
		}
	}
	for (unsigned int i = 0; i < var_len_column.size(); i++)
	{
		const char* each_column = var_len_column[i].c_str();
		for (int j = 0; j < var_len_column[i].size(); j++)
		{
			record_byte_arr.push_back(each_column[j]);
		}
	}
	return record_byte_arr;
}

