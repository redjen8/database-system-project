#pragma once
#include <vector>
#include <string>
#include "table.h"
#include "jsoncpp/json/json.h"

#define FILE_MAX_BLOCK_NUM 1024
#define META_DATA_FILE_NAME "meta_data.json"
#define DEFAULT_DB_FILE "data.db"

class SystemModule
{
private:
	std::vector<Table> table_list;
	block_store_loc next_block_ptr;
	Json::Value system_meta_json;
public:
	std::map<std::string, int> table_name_index_map;
	SystemModule(); // 메타 데이터 로드 및 시스템 초기 구동
	int insert_new_table(Table new_table); // 빈 테이블 생성 및 메타 데이터 업데이트
	int insert_new_record(int table_idx); // 테이블에 레코드 삽입
	int search_by_pk(int table_idx, std::string key); 
	int get_table_column_list(int table_idx);
	int get_table_every_data(int table_idx);
	table_meta_data convert_json_to_meta(Json::Value data);
	Json::Value convert_meta_to_json(table_meta_data meta);
};