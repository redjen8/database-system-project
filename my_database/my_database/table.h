#pragma once
#include <vector>
#include "record.h"
#include "meta_data.h"

typedef struct record_store_loc
{
	std::string file_name;
	int start_loc;
	int end_loc;
}record_store_loc;


class Table
{
private:
	column_info column_meta;
	table_meta_data table_meta;
	std::vector<Record> record_list;
	std::vector<record_store_loc> record_loc_list;
public:
	Table(table_meta_data table);
	Table(column_info column, std::vector<record_store_loc> record_loc_list);
	int load_from_file_location();
	int insert_new_record(std::string file_name, int start, int end);
	Record search_by_id(std::string search_key);
	std::vector<Record> get_record_list();
	table_meta_data get_table_meta();
	column_info get_column_meta();
	int insert_new_record_loc(record_store_loc new_location);
};