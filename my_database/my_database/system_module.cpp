#include "system_module.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "jsoncpp/json/json.h"
#include "table.h"
#include "slotted_page.h"

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

	//json 파일 파싱하여 Table 데이터 리스트화
	//Table 데이터는 바로 read 하지 않고 나중에 메모리로 올림
	Json::Reader reader;
	Json::Value read_data;
	if (!reader.parse(str, read_data))
	{
		std::cerr << "Failed to parse json : " << reader.getFormattedErrorMessages() << std::endl;
	}
	system_meta_json = read_data;

	Json::Value table_meta = read_data["table_meta_data"];
	int cnt = 0;
	for (auto i = table_meta.begin(); i != table_meta.end(); i++)
	{
		std::string table_name = (*i)["table_name"].asCString();
		Json::Value block_location_json = (*i)["block_location"];
		std::vector<block_store_loc> block_location;
		for (auto it = block_location_json.begin(); it != block_location_json.end(); it++)
		{
			std::string file_name = (*it)["file_name"].asCString();
			int start_loc = (*it)["start_loc"].asInt();
			int end_loc = (*it)["end_loc"].asInt();
			block_store_loc each_block_info = block_store_loc{file_name, start_loc, end_loc};
			block_location.push_back(each_block_info);
		}
		std::vector<std::string> table_column_list;
		for (auto it = (*i)["table_column_list"].begin(); it != (*i)["table_column_list"].end(); it++)
		{
			table_column_list.push_back((*it).asCString());
		}

		std::vector<int> fixed_column_length; 
		for (auto it = (*i)["fixed_column_length"].begin(); it != (*i)["fixed_column_length"].end(); it++)
		{
			fixed_column_length.push_back((*it).asInt());
		}
		int variable_column_cnt = (*i)["variable_column_cnt"].asInt();
		int fixed_column_cnt = (*i)["fixed_column_cnt"].asInt();
		int pk_column_idx = (*i)["pk_column_idx"].asInt();
		table_meta_data tmd = table_meta_data{table_name, block_location, table_column_list, fixed_column_length, variable_column_cnt, fixed_column_cnt, pk_column_idx};
		Table each_table = Table(tmd);
		table_list.push_back(each_table);
		table_name_index_map.insert({ table_name, cnt });
		cnt++;
	}
}

int SystemModule::insert_new_table(Table new_table)
{
	{
		table_meta_data table_meta = new_table.get_table_meta();
		Json::Value write_data = convert_meta_to_json(table_meta);
		system_meta_json["table_meta_data"].append(write_data);

		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "None";
		builder["indentation"] = "    ";

		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		std::ofstream outputFileStream(META_DATA_FILE_NAME);
		writer->write(system_meta_json, &outputFileStream);
	}
	return 0;
}

int SystemModule::insert_new_record(int table_idx)
{
	Table target_table = table_list[table_idx];
	//target_table.load_from_file_location();
	std::vector<std::string> column_name_list = target_table.get_table_meta().table_column_list;
	std::vector<std::string> new_column_value;
	for (int i = 0; i < column_name_list.size(); i++)
	{
		std::string input;
		while (true)
		{
			std::cout << "Insert value for column name : " << column_name_list[i];
			std::cin >> input;
			if (i < target_table.get_table_meta().fixed_column_cnt)
			{
				if (input.length() > target_table.get_table_meta().fixed_column_length[i])
				{
					std::cout << "Column Length Exceeded. Please input less than " << target_table.get_table_meta().fixed_column_length[i] << std::endl;
				}
				else break;
			}
			else break;
		}
		new_column_value.push_back(input);
	}
	Record new_record = Record(new_column_value, target_table.get_column_meta());
	std::vector<block_store_loc> target_block_location = target_table.get_table_meta().block_location;
	bool insert_flag = false;
	std::string update_file_name = DEFAULT_DB_FILE;
	for (int i = 0; i < target_block_location.size(); i++)
	{
		std::string file_name = target_block_location[i].file_name;
		update_file_name = file_name;
		int start = target_block_location[i].start_loc;
		int end = target_block_location[i].end_loc;
		SlottedPage page_read_from_disk = SlottedPage(file_name, target_table.get_column_meta(), start, end);
		//기존 page 내에 record 삽입 가능한 경우
		if (page_read_from_disk.is_able_insert())
		{
			page_read_from_disk.add_record(new_record);
			insert_flag = true;
			page_read_from_disk.write_page_on_disk();
			break;
		}
	}
	// 모든 block location이 차서 기존 block에 insert가 불가능한 경우, 새 block 할당 및 system catalog 업데이트
	if (!insert_flag)
	{
		int current_file_size = std::filesystem::file_size(update_file_name);
		
		if (current_file_size < FILE_MAX_BLOCK_NUM * PAGE_SIZE)
		{
			// 기존 파일에 block만 추가하는 경우
			int current_file_block_idx = current_file_size / PAGE_SIZE;
			SlottedPage new_page_to_disk = SlottedPage(update_file_name, target_table.get_column_meta(), current_file_block_idx + 1);
			new_page_to_disk.add_record(new_record);
			new_page_to_disk.write_page_on_disk();

			std::string table_name = target_table.get_table_meta().table_name;
			std::vector<block_store_loc> table_block_loc_list = target_table.get_table_meta().block_location;

			for (int i = 0; i < table_block_loc_list.size(); i++)
			{

			}

			for (auto it = system_meta_json["table_meta_data"].begin(); it != system_meta_json["table_meta_data"].end(); it++)
			{
				
			}
		}
		else
		{
			// 기존 파일이 꽉 차서 새 파일을 써야하는 경우
			std::string new_file_name;
			SlottedPage new_page_to_disk = SlottedPage(new_file_name, target_table.get_column_meta(), 0);
			new_page_to_disk.add_record(new_record);
			new_page_to_disk.write_page_on_disk();
		}
	}
	return 0;
}

int SystemModule::search_by_pk(int table_idx, std::string key)
{
	Table target_table = table_list[table_idx];
	target_table.load_from_file_location();
	std::vector<Record> record_list = target_table.get_record_list();
	int pk_idx = target_table.get_table_meta().pk_column_idx;
	for (int i = 0; i < record_list.size(); i++)
	{
		std::string search_value;
		if (pk_idx > target_table.get_table_meta().fixed_column_cnt)
		{
			search_value = record_list[i].get_var_column_list().at(pk_idx);
		}
		else
		{
			search_value = record_list[i].get_fixed_column_list().at(pk_idx);
		}
		if (search_value.compare(key) == 0)
		{
			record_list[i].print_record();
			return 0;
		}
	}
	return 0;
}

int SystemModule::get_table_column_list(int table_idx)
{
	Table target_table = table_list[table_idx];
	std::vector<std::string> column_name_list = target_table.get_table_meta().table_column_list;
	for (int i = 0; i < column_name_list.size(); i++)
	{
		std::cout << "column " << std::to_string(i) << " : " << column_name_list[i] << std::endl;
	}
	return 0;
}

int SystemModule::get_table_every_data(int table_idx)
{
	Table target_table = table_list[table_idx];
	target_table.load_from_file_location();
	std::vector<Record> record_list = target_table.get_record_list();
	for (int i = 0; i < record_list.size(); i++)
	{
		record_list[i].print_record();
	}
	return 0;
}

table_meta_data SystemModule::convert_json_to_meta(Json::Value meta)
{
	std::string table_name = meta["table_name"].asCString();
	Json::Value block_location_json = meta["block_location"];
	std::vector<block_store_loc> block_location;
	for (auto it = block_location_json.begin(); it != block_location_json.end(); it++)
	{
		std::string file_name = (*it)["file_name"].asCString();
		int start_loc = (*it)["start_loc"].asInt();
		int end_loc = (*it)["end_loc"].asInt();
		block_store_loc each_block_info = block_store_loc{ file_name, start_loc, end_loc };
		block_location.push_back(each_block_info);
	}
	Json::Value next_insert_json = meta["next_insert_location"];
	std::vector<std::string> table_column_list;
	for (auto it = meta["table_column_list"].begin(); it != meta["table_column_list"].end(); it++)
	{
		table_column_list.push_back((*it).asCString());
	}

	std::vector<int> fixed_column_length;
	for (auto it = meta["fixed_column_length"].begin(); it != meta["fixed_column_length"].end(); it++)
	{
		fixed_column_length.push_back((*it).asInt());
	}
	int variable_column_cnt = meta["variable_column_cnt"].asInt();
	int fixed_column_cnt = meta["fixed_column_cnt"].asInt();
	int pk_column_idx = meta["pk_column_idx"].asInt();
	table_meta_data tmd = table_meta_data{ table_name, block_location, table_column_list, fixed_column_length, variable_column_cnt, fixed_column_cnt, pk_column_idx };
	return tmd;
}

Json::Value SystemModule::convert_meta_to_json(table_meta_data meta)
{
	Json::Value table_meta_json;
	table_meta_json["table_name"] = meta.table_name;
	for (int i = 0; i < meta.table_column_list.size(); i++)
	{
		table_meta_json["table_column_list"].append(meta.table_column_list[i]);
	}
	table_meta_json["variable_column_cnt"] = meta.fixed_column_cnt;
	table_meta_json["fixed_column_cnt"] = meta.variable_column_cnt;

	if (meta.block_location.size() > 0)
	{
		for (int i = 0; i < meta.block_location.size(); i++)
		{
			Json::Value block_location;
			block_location["file_name"] = meta.block_location[i].file_name;
			block_location["start_loc"] = meta.block_location[i].start_loc;
			block_location["end_loc"] = meta.block_location[i].end_loc;
			table_meta_json["block_location"].append(block_location);
		}
	}
	else
	{
		table_meta_json["block_location"] = Json::objectValue;
	}

	for (int i = 0; i < meta.fixed_column_length.size(); i++)
	{
		table_meta_json["fixed_column_length"].append(meta.fixed_column_length[i]);
	}

	table_meta_json["pk_column_idx"] = meta.pk_column_idx;
	return table_meta_json;
}