#pragma once
#include <vector>
#include "record.h"
#include "column_info.h"

typedef struct record_store_loc
{
	std::string file_name;
	int start_loc;
	int end_loc;
};

class Table
{
private:
	column_info column_meta;
	std::vector<Record> record_list;
	std::vector<record_store_loc> record_loc_list;
public:
	Table();
	Table(column_info column, std::vector<record_store_loc> record_loc_list);
	int load_from_file_location();
	int insert_new_record(std::string file_name, int start, int end);
	Record search_by_id(std::string search_key);
};