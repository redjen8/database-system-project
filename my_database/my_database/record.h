#pragma once
#include <vector>
#include <string>

#define MAX_COLUMN_LEN 50

typedef struct column_meta
{
	int num_fixed;
	int num_variable;
}column_meta;

class Record
{
private:
	int null_bitmap;
	std::vector<std::string>* fixed_len_column;
	std::vector<std::string>* var_len_column;
public:
	Record(std::vector<std::string>* input_fixed, std::vector<std::string>* input_variable, column_meta column_info);
	void print_record();
};