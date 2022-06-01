#include "record.h"
#include "slotted_page.h"
#include <iostream>
#include <fstream>
#include "jsoncpp/json/json.h"

int main()
{
	const char* meta_data_file_name = "meta_data.json";

	{
		Json::Value table1;
		table1["table_name"] = "student";
		table1["table_column_list"].append("student_id");
		table1["table_column_list"].append("grade");
		table1["table_column_list"].append("name");
		table1["table_column_list"].append("major");
		table1["variable_column_cnt"] = 2;
		table1["fixed_column_cnt"] = 2;

		Json::Value file1;
		file1["file_name"] = "data.db";
		file1["start_loc"] = 4096;
		file1["end_loc"] = 49152;
		table1["block_location"].append(file1);

		Json::Value file2;
		file2["file_name"] = "data.db";
		file2["start_loc"] = 0;
		file2["end_loc"] = 4096;
		table1["block_location"].append(file2);

		table1["next_insert_location"]["file_name"] = "data.db";
		table1["next_insert_location"]["start_loc"] = 0;
		table1["next_insert_location"]["end_loc"] = 4096;

		Json::Value table_meta_data;
		table_meta_data["table_meta_data"].append(table1);

		table_meta_data["FILE_MAX_BLOCK_NUM"] = 1024;
		table_meta_data["NEXT_BLOCK_LOC"]["file_name"] = "data.db";
		table_meta_data["NEXT_BLOCK_LOC"]["loc"] = 0;

		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "None";
		builder["indentation"] = "    ";

		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		std::ofstream outputFileStream("meta_data.json");
		writer->write(table_meta_data, &outputFileStream);
	}
	{
		std::fstream json_file;
		json_file.open("meta_data.json", std::ios::binary | std::ios::in | std::ios::out);
		std::string str;
		if (json_file.is_open())
		{
			while (!json_file.eof())
			{
				std::string tmp;
				std::getline(json_file, tmp);
				str.append(tmp);
			}
		}
		json_file.close();
		Json::Reader reader;
		Json::Value value;
		if (!reader.parse(str, value))
		{
			std::cerr << "Failed to parse json : " << reader.getFormattedErrorMessages() << std::endl;
		}
		std::cout << value << std::endl;
	}
	return 0;
q}

int main2()
{
	std::vector<std::string> column_name = { "student_id", "grade", "name", "major" };
	std::vector<bool> column_type = { false, false, true, true };
	std::vector<int> fixed_column_length = {5, 1};
	column_info column_meta = column_info{column_name, column_type, fixed_column_length};

	std::vector<std::string> input;
	input.push_back("00122");
	input.push_back("4");
	input.push_back("Jeong Seok Woo");
	input.push_back("Computer Science");
	Record record1 = Record(input, column_meta);

	std::vector<std::string> input2;
	input2.push_back("00123");
	input2.push_back("2");
	input2.push_back("");
	input2.push_back("Mathmatics");
	Record record2 = Record(input2, column_meta);

	std::vector<std::string> input0;
	input0.push_back("01549");
	input0.push_back("5");
	input0.push_back("Professor");
	input0.push_back("Defense");
	Record record0 = Record(input0, column_meta);

	SlottedPage my_page = SlottedPage("data.db", column_meta, 0);
	my_page.add_record(record1);
	my_page.add_record(record2);
	my_page.add_record(record0);
	//my_page.get_record_list();
	my_page.write_page_on_disk();

	column_name = { "student_id", "grade", "name", "major", "gender" };
	column_type = { false, false, true, true, false };
	fixed_column_length = { 5, 1, 1 };
	column_info column_meta2 = column_info{ column_name, column_type, fixed_column_length };

	std::vector<std::string> input3;
	input3.push_back("09157");
	input3.push_back("3");
	input3.push_back("Lee Boum Young");
	input3.push_back("Architect");
	input3.push_back("M");
	Record record3 = Record(input3, column_meta2);

	std::vector<std::string> input4;
	input4.push_back("09161");
	input4.push_back("4");
	input4.push_back("Park Joo Young");
	input4.push_back("Forward");
	input4.push_back("F");
	Record record4 = Record(input4, column_meta2);

	SlottedPage my_page2 = SlottedPage("data.db", column_meta2, 1);
	my_page2.add_record(record3);
	my_page2.add_record(record4);

	SlottedPage test = SlottedPage("data.db", column_meta, 0, 4096);
	test.print_slotted_page();
	my_page2.write_page_on_disk();

	SlottedPage test2 = SlottedPage("data.db", column_meta2, 4096, 8192);
	test2.print_slotted_page();

	test = SlottedPage("data.db", column_meta, 0, 4096);
	test.print_slotted_page();
	return 0;
}