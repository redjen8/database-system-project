#include "record.h"
#include "slotted_page.h"

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

	SlottedPage my_page = SlottedPage(page_meta_data{ PAGE_SIZE, 0, 0 });
	my_page.print_slotted_page();
	my_page.get_record_list();
	return 0;
}