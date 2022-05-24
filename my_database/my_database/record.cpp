#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <bitset>
#include "record.h"

typedef struct column_meta
{
	int num_fixed;
	int num_variable;
}column_meta;

class record
{
private:
	int null_bitmap;
	std::vector<std::string>* fixed_len_column;
	std::vector<std::string>* var_len_column;

public:
	record(std::vector<std::string>* input_fixed, std::vector<std::string>* input_variable, column_meta column_info) {
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

	void print_record()
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
};

int main()
{
	std::vector<std::string> fixed_column;
	std::vector<std::string> variable_column;
	fixed_column.push_back("00122");
	fixed_column.push_back("student");
	variable_column.push_back("Jeong Seok Woo");
	variable_column.push_back("Computer Science");
	record new_record = record(&fixed_column, &variable_column, column_meta{ 2,4 });
	new_record.print_record();
	return 0;
}