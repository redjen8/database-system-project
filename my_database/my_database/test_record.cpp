#include "record.h"
#include "slotted_page.h"
#include <iostream>

int main()
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