#pragma once
#include <vector>
#include <string>
#include "table.h"
#include "jsoncpp/json/json.h"

#define FILE_MAX_BLOCK_NUM 1024
#define META_DATA_FILE_NAME "meta_data.json"

class SystemModule
{
private:
	std::vector<Table> table_list;
	Json::Value system_meta_json;
	std::string next_insert_file_name;
public:
	std::map<std::string, int> table_name_index_map;
	SystemModule(); // ��Ÿ ������ �ε� �� �ý��� �ʱ� ����
	int insert_new_table(Table new_table); // �� ���̺� ���� �� ��Ÿ ������ ������Ʈ
	int insert_new_record(int table_idx); // ���̺� ���ڵ� ����
	int search_by_pk(int table_idx, std::string key); 
	int get_table_column_list(int table_idx);
	int get_table_every_data(int table_idx);
	table_meta_data convert_json_to_meta(Json::Value data);
	Json::Value convert_meta_to_json(table_meta_data meta);
	int write_meta_data_to_file();
	void get_table_name_list();
};

