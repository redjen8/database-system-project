#include "system_module.h"
#include <vector>
#include <iostream>
#include <fstream>

SystemModule::SystemModule()
{
	std::fstream meta_file;
	meta_file.open(META_DATA_FILE_NAME, std::ios::binary | std::ios::in);
	std::string str;
	std::cout << "Reading meta data file.." << std::endl;
	if (meta_file.is_open())
	{
		while (!meta_file.eof())
		{
			std::string tmp;
			std::getline(meta_file, tmp);
			str.append(tmp);
		}
	}
	meta_file.close();

	//json ���� �Ľ��Ͽ� Table ������ ����Ʈȭ
	//Table �����ʹ� �ٷ� read

}

int SystemModule::insert_new_table(Table new_table)
{
	return 0;
}

int SystemModule::insert_new_record()
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

int SystemModule::get_table_every_data(int table_idx)
{
	return 0;
}