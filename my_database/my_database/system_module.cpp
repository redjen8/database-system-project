#include "system_module.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "jsoncpp/json/json.h"
#include "table.h"

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
	//Table 데이터는 바로 read
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