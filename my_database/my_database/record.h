#pragma once
#include <vector>
#include <string>
#include "meta_data.h"

#define MAX_COLUMN_LEN 50

typedef struct location_meta_data
{
	int offset;
	int length;
}location_meta_data;


class Record
{
private:
	unsigned int null_bitmap;
	std::vector<std::pair<std::string, int>> fixed_len_column;
	std::vector<std::string> var_len_column;
	std::vector<location_meta_data> var_len_column_loc;
	std::vector<unsigned char> record_byte_arr;
	int byte_arr_size;
public:
	Record();
	Record(unsigned char* byte_arr, int arr_length, column_info column_meta);
	Record(std::vector<std::string> input, column_info column_meta);
	int get_record_size();
	void print_record();
	int get_null_bitmap();
	std::vector<std::string> get_fixed_column_list();
	std::vector<std::string> get_var_column_list();
	std::string to_string();
	std::vector<unsigned char> to_byte_vector();
};