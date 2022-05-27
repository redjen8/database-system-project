#include "record.h"
#include "slotted_page.h"
#include <iostream>

int main()
{
	std::vector<std::string> fixed_column;
	std::vector<std::string> variable_column;
	fixed_column.push_back("00122");
	fixed_column.push_back("student");
	variable_column.push_back("Jeong Seok Woo");
	variable_column.push_back("Computer Science");
	Record new_record = Record(&fixed_column, &variable_column, column_meta{ 2,4 });
	new_record.print_record();
	std::cout << "record size : " << new_record.get_record_size() << std::endl;

	SlottedPage my_page = SlottedPage();
	my_page.add_record(new_record);
	//my_page.print_slotted_page();
	//my_page.get_record_list();
	//my_page.write_page_on_disk();
	return 0;
}