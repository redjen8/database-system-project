#include "slotted_page.h"
#include "record.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <bitset>

SlottedPage::SlottedPage(int page_idx)
{
	meta_data.entry_size = 0;
	meta_data.free_space_end_addr = PAGE_SIZE * (page_idx+1);
}

Record* SlottedPage::get_record_list()
{
	return NULL;
}

void SlottedPage::print_slotted_page()
{
	std::cout << "page size : " << PAGE_SIZE << std::endl;
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
	Record* exRecord = new Record();
	std::vector<unsigned char> entry_size_byte_arr = exRecord->int_to_byte(meta_data.entry_size);
	fout.write((char*)&entry_size_byte_arr[0], 4);
	std::vector<unsigned char> free_space_byte_arr = exRecord->int_to_byte(meta_data.free_space_end_addr);
	fout.write((char*)&free_space_byte_arr[0], 4);

 	int record_start_idx = PAGE_SIZE;
	for (int i = 0; i < meta_data.entry_size; i++)
	{
		std::vector<unsigned char> offset_byte_arr = exRecord->int_to_byte(record_ptr_arr[i].offset);
		std::vector<unsigned char> length_byte_arr = exRecord->int_to_byte(record_ptr_arr[i].length);
		unsigned char is_deleted_byte;
		if (record_ptr_arr[i].is_deleted) is_deleted_byte = 1;
		else is_deleted_byte = 0;
		fout.write((char*)&offset_byte_arr[0], 4);
		fout.write((char*)&length_byte_arr[0], 4);
		fout.write((char*)&is_deleted_byte, 1);
	}

	int current_seek = PAGE_SIZE * page_idx;
	for (int i = 0; i < meta_data.entry_size; i++)
	{
		if (record_ptr_arr[i].is_deleted) continue;
		std::vector<unsigned char> byte_arr = record_arr[i].to_byte_vector();
		int buffer_offset = record_ptr_arr[i].offset;
		int buffer_length = record_ptr_arr[i].length;
		
		current_seek -= buffer_length;
		fout.seekg(current_seek);
		fout.write((char*) & byte_arr[0], byte_arr.size());
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

int SlottedPage::read_from_disk(int page_start, int page_end)
{
	
	return 0;
}