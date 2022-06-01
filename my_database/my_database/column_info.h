#pragma once
// 컬럼 이름 목록 column_name, column_type이 true이면 가변 길이 컬럼, false면 고정 길이 칼럼
typedef struct column_info
{
	std::vector<std::string> column_name;
	std::vector<bool> column_type;
	std::vector<int> fixed_column_length;
	int primary_key_index;
}column_info;