#include "table.h"
#include "slotted_page.h"
#include "meta_data.h"
#include "record.h"

Table::Table(table_meta_data table)
{
	table_meta = table;
	column_meta.column_name = table_meta.table_column_list;
	int column_length = table_meta.variable_column_cnt + table_meta.fixed_column_cnt;
	std::vector<bool> column_type(column_length, true);
	for (int i = 0; i < table_meta.fixed_column_cnt; i++)
	{
		column_type[i] = false;
	}
	column_meta.column_type = column_type;
	column_meta.fixed_column_length = table_meta.fixed_column_length;
	column_meta.primary_key_index = table_meta.pk_column_idx;
	
	for (int i = 0; i < table_meta.block_location.size(); i++)
	{
		std::string file_name = table_meta.block_location[i].file_name;
		int start = table_meta.block_location[i].start_loc;
		int end = table_meta.block_location[i].end_loc;
		record_loc_list.push_back(record_store_loc{ file_name, start, end });
	}
	
}

Table::Table(column_info column, std::vector<record_store_loc> record_ptr_list)
{
	column_meta = column;
	record_loc_list = record_ptr_list;
	load_from_file_location();
}

int Table::load_from_file_location()
{
	int file_num = record_loc_list.size();
	for (int i = 0; i < file_num; i++)
	{
		SlottedPage read_page = SlottedPage(record_loc_list[i].file_name, column_meta, record_loc_list[i].start_loc, record_loc_list[i].end_loc);
		std::vector<Record> record_list_from_page = read_page.get_record_list();
		for (int j = 0; j < record_list_from_page.size(); j++)
		{
			record_list.push_back(record_list_from_page[j]);
		}
	}
	return 0;
}

int Table::insert_new_record(std::string file_name, int start, int end)
{
	SlottedPage read_page = SlottedPage(file_name, column_meta, start, end);
	return 0;
}

Record Table::search_by_id(std::string search_key)
{
	int pk_index = column_meta.primary_key_index;
	for (int i = 0; i < record_list.size(); i++)
	{
		int fix_len_column_num = record_list[i].get_fixed_column_list().size();
		int var_len_column_num = record_list[i].get_var_column_list().size();
		if (pk_index > fix_len_column_num)
		{
			//pk가 가변 길이 칼럼일 경우
			pk_index -= fix_len_column_num;
			if (record_list[i].get_var_column_list().at(pk_index) == search_key)
			{
				return record_list[i];
			}
		}
		else
		{
			if (record_list[i].get_fixed_column_list().at(pk_index) == search_key)
			{
				return record_list[i];
			}
		}
	}
	Record not_found = Record();
	return not_found;
}

std::vector<Record> Table::get_record_list()
{
	return record_list;
}

table_meta_data Table::get_table_meta()
{
	return table_meta;
}

column_info Table::get_column_meta()
{
	return column_meta;
}