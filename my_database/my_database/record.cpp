#include <vector>
#include <string>
#include <cstring>
#include "record.h"

class record
{
private:
	int null_bitmap;
	std::vector<const char*> fixed_len_column;
	std::vector<std::string>* var_len_column;

public:
	record(std::vector<std::string> input_fixed, std::vector<std::string> input_variable) {
		for (int i = 0; i < input_fixed.size(); i++) {
			const char* each_column = input_fixed.at(i).substr(0, MAX_COLUMN_LEN).c_str();
			fixed_len_column.push_back(each_column);
		}
		var_len_column = &input_variable;
	}
};