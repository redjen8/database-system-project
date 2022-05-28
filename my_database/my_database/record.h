#pragma once
#include <vector>
#include <string>

#define MAX_COLUMN_LEN 50

typedef struct location_meta_data
{
	int offset;
	int length;
}location_meta_data;

// �÷� �̸� ��� column_name, column_type�� true�̸� ���� ���� �÷�, false�� ���� ���� Į��
typedef struct column_info
{
	std::vector<std::string> column_name;
	std::vector<bool> column_type;
}column_info;

class Record
{
private:
	int null_bitmap;
	std::vector<std::string> fixed_len_column;
	std::vector<std::string> var_len_column;
	std::vector<location_meta_data> var_len_column_loc;
public:
	Record(std::vector<std::string> input, column_info column_meta);
	int get_record_size();
	void print_record();
	int get_null_bitmap();
	std::vector<std::string>* get_fixed_column_list();
	std::vector<std::string>* get_var_column_list();
	const char* print_to_disk();
};