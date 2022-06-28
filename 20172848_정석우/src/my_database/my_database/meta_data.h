#pragma once
#include <vector>
#include <string>

// 컬럼 이름 목록 column_name, column_type이 true이면 가변 길이 컬럼, false면 고정 길이 칼럼
typedef struct column_info
{
	std::vector<std::string> column_name;
	std::vector<bool> column_type;
	std::vector<int> fixed_column_length;
	int primary_key_index;
}column_info;

typedef struct block_store_loc
{
	std::string file_name;
	int start_loc;
	int end_loc;
}block_store_loc;

typedef struct table_meta_data
{
	std::string table_name;
	std::vector<block_store_loc> block_location;
	std::vector<std::string> table_column_list;
	std::vector<int> fixed_column_length;
	int variable_column_cnt;
	int fixed_column_cnt;
	int pk_column_idx;
}table_meta_data;
