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
	input2.push_back("HiHi");
	input2.push_back("Mathmatics");
	Record record2 = Record(input2, column_meta);

	SlottedPage my_page = SlottedPage("data.db", 0);
	my_page.add_record(record1);
	my_page.add_record(record2);
	//my_page.print_slotted_page();
	//my_page.get_record_list();
	my_page.write_page_on_disk();
	
	
	SlottedPage my_page2 = SlottedPage("data.db", column_meta, 0, 4096);
	my_page2.print_slotted_page();

	return 0;
}