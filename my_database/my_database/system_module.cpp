#include "system_module.h"
#include <vector>

SystemModule::SystemModule()
{

}

int SystemModule::insert_new_table(Table new_table)
{
	return 0;
}

int SystemModule::insert_new_record(block_store_loc next_loc)
{
	return 0;
}

int SystemModule::search_by_pk(std::string key)
{
	return 0;
}

int SystemModule::get_table_column_list(int table_idx)
{
	return 0;
}