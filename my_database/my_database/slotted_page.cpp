#include "slotted_page.h"
#include "record.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <bitset>

SlottedPage::SlottedPage()
{
	meta_data.page_size = PAGE_SIZE;
	meta_data.entry_size = 0;
	meta_data.free_space_end_addr = -1;
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
	std::ofstream fout;
	fout.open("data.db", std::ios::binary | std::ios::out);
	// page meta data 파일 쓰기
	fout.write("{", 1);
	fout.write(std::to_string(meta_data.page_size).c_str(), std::to_string(meta_data.page_size).length());
	fout.write(",", 1);
	fout.write(std::to_string(meta_data.entry_size).c_str(), std::to_string(meta_data.entry_size).length());
	fout.write(",", 1);
	fout.write(std::to_string(meta_data.free_space_end_addr).c_str(), std::to_string(meta_data.free_space_end_addr).length());
	fout.write("}", 1);

	std::string null_bitmap = std::bitset<32>(0x000000E1).to_string();
	fout.write(null_bitmap.c_str(), null_bitmap.length());

	fout.close();
	return 0;
}

int SlottedPage::add_record(Record tRecord)
{
	std::cout << sizeof(tRecord) << std::endl;
	std::cout << sizeof(tRecord.var_len_column) << std::endl;
	std::cout << sizeof(tRecord.fixed_len_column) << std::endl;
	return 0;
}
