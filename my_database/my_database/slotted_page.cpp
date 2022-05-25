#include "slotted_page.h"
#include "record.h"
#include <vector>
#include <iostream>

SlottedPage::SlottedPage(page_meta_data input_meta)
{
	meta_data.page_size = input_meta.page_size;
	meta_data.entry_size = input_meta.entry_size;
	meta_data.free_space_end_addr = input_meta.free_space_end_addr;
}

Record* SlottedPage::get_record_list()
{
	return NULL;
}

void SlottedPage::print_slotted_page()
{
	std::cout << "page size : " << meta_data.page_size << std::endl;
	std::cout << "entry size : " << meta_data.entry_size << std::endl;
	std::cout << "free space end address : " << meta_data.free_space_end_addr << std::endl;
	int cnt = 0;
	for (std::vector<record_location>::const_iterator i = record_ptr_arr.begin(); i != record_ptr_arr.end(); i++)
	{
		std::cout << "record " << cnt << "byte : " << (*i).byte << "offset : " << (*i).offset << std::endl;
		cnt++;
	}
}

int SlottedPage::write_page_on_disk()
{
	return 0;
}
