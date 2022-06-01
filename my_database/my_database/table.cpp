#include "table.h"
#include "slotted_page.h"
#include "column_info.h"
#include "record.h"

Table::Table()
{

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

int Table::insert_new_record()
{
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
			//pk�� ���� ���� Į���� ���
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