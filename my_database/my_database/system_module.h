#pragma once
#include <vector>
#include <string>
#include "table.h"

#define FILE_MAX_BLOCK_NUM 1024
#define META_DATA_FILE_NAME "meta_data.json"

typedef struct block_store_loc
{
	std::string file_name;
	int start_loc;
	int end_loc;
};

class SystemModule
{
private:
	std::vector<Table> table_list;
	block_store_loc next_block_ptr;
public:
	SystemModule(); // ��Ÿ ������ �ε� �� �ý��� �ʱ� ����
	int insert_new_table(Table new_table);
	int insert_new_record(block_store_loc next_loc);
	int search_by_pk(std::string key);
	int get_table_column_list(int table_idx);
	int get_table_every_data(int table_idx);
};