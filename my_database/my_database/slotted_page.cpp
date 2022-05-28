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
	meta_data.free_space_end_addr = PAGE_SIZE;
}

Record* SlottedPage::get_record_list()
{
	return NULL;
}

void SlottedPage::print_slotted_page()
{
	std::cout << "page size : " << meta_data.page_size << std::endl;
	std::cout << "entry size : " << meta_data.entry_size << std::endl;
	std::cout << "free space end address : " << meta_data.free_space_end_addr << std::endl << std::endl;
	int cnt = 0;
	for (int i = 0 ; i != record_arr.size(); i++)
	{
		std::cout << "record " << cnt << " :: size = " << record_arr[i].get_record_size() << std::endl;
		record_arr[i].print_record();
		cnt++;
	}
}

int SlottedPage::write_page_on_disk()
{
	std::fstream fout;
	fout.open("data.db", std::ios::binary | std::ios::out);
	// page meta data 파일 쓰기
	fout.write("{", 1);
	fout.write(std::to_string(meta_data.page_size).c_str(), std::to_string(meta_data.page_size).length());
	fout.write(",", 1);
	fout.write(std::to_string(meta_data.entry_size).c_str(), std::to_string(meta_data.entry_size).length());
	fout.write(",", 1);
	fout.write(std::to_string(meta_data.free_space_end_addr).c_str(), std::to_string(meta_data.free_space_end_addr).length());
	fout.write("}", 1);

	int record_start_idx = meta_data.page_size;
	for (int i = 0; i < meta_data.entry_size; i++)
	{
		fout.write("(", 1);
		fout.write(std::to_string(record_ptr_arr[i].offset).c_str(), std::to_string(record_ptr_arr[i].offset).length());
		fout.write(",", 1);
		fout.write(std::to_string(record_ptr_arr[i].length).c_str(), std::to_string(record_ptr_arr[i].length).length());
		fout.write(",", 1);
		std::string is_deleted;
		if (record_ptr_arr[i].is_deleted) is_deleted = "1";
		else is_deleted = "0";
		fout.write(is_deleted.c_str(), is_deleted.length());
		fout.write(")", 1);
	}

	//std::string null_bitmap = std::bitset<32>(0x000000E1).to_string();
	//fout.write(null_bitmap.c_str(), null_bitmap.length());
	fout.seekg(meta_data.page_size);
	for (int i = 0; i < meta_data.entry_size; i++)
	{
		if (record_ptr_arr[i].is_deleted) continue;
		const char* write_buffer = record_arr[i].print_to_disk();
		int buffer_offset = record_ptr_arr[i].offset;
		int buffer_length = record_ptr_arr[i].length;
		
		fout.write(write_buffer, sizeof(write_buffer));
		fout.seekg(-buffer_length);
	}
	fout.close();
	return 0;
}

int SlottedPage::add_record(Record tRecord)
{
	int new_record_offset = meta_data.free_space_end_addr;
	int new_record_length = tRecord.get_record_size();
	bool new_record_isDeleted = false;
	record_arr.push_back(tRecord);
	record_ptr_arr.push_back(record_meta_data{new_record_offset, new_record_length, new_record_isDeleted});
	meta_data.free_space_end_addr -= new_record_length;
	meta_data.entry_size++;
	return 0;
}

int SlottedPage::read_from_disk()
{
	return 0;
}