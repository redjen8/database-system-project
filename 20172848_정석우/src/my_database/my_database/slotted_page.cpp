#include "slotted_page.h"
#include "record.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <bitset>
#include <format>
#include "byte_convert.h"

SlottedPage::SlottedPage(std::string file, column_info column_meta, int page_start, int page_end)
{
	if (page_end - page_start != PAGE_SIZE) 
	{
		//페이지 사이즈가 지정된 것과 다를 경우 예외처리
	}
	page_idx = page_start / PAGE_SIZE;
	file_name = file;

	std::ifstream fout;
	fout.open(file_name.c_str(), std::ios::binary | std::ios::in | std::ios::out);
	fout.seekg(page_start);
	unsigned char read_buffer[PAGE_SIZE];
	fout.read((char*)read_buffer, PAGE_SIZE);
	int file_cursor;
	unsigned char meta_byte_buffer[4];
	for (file_cursor = 0; file_cursor < 4; file_cursor++)
	{
		meta_byte_buffer[file_cursor] = read_buffer[file_cursor];
	}
	meta_data.entry_size = byte_arr_to_int(meta_byte_buffer);
	for (file_cursor = 4; file_cursor < 8; file_cursor++)
	{
		meta_byte_buffer[file_cursor - 4] = read_buffer[file_cursor];
	}
	meta_data.free_space_end_addr = byte_arr_to_int(meta_byte_buffer);
	meta_data.column_meta = column_meta;

	for (int entry_num = 0; entry_num < meta_data.entry_size; entry_num++)
	{
		for (int i = 0; i < 4; i++)
		{
			meta_byte_buffer[i] = read_buffer[file_cursor];
			file_cursor++;
		}
		int offset = byte_arr_to_int(meta_byte_buffer);
		for (int i = 0; i < 4; i++)
		{
			meta_byte_buffer[i] = read_buffer[file_cursor];
			file_cursor++;
		}
		int length = byte_arr_to_int(meta_byte_buffer);
		bool isDeleted = read_buffer[file_cursor];
		file_cursor++;
		record_ptr_arr.push_back(record_meta_data{offset, length, isDeleted });
	}

	for (int entry_num = 0; entry_num < meta_data.entry_size; entry_num++)
	{
		unsigned char* record_byte_arr = new unsigned char[record_ptr_arr[entry_num].length];
		int offset = record_ptr_arr[entry_num].offset - page_idx * PAGE_SIZE;
		int length = record_ptr_arr[entry_num].length;
		std::cout << "read starts from : " << offset << " to : " << offset + length << std::endl;
		for (int i = offset; i < offset + length; i++)
		{
			record_byte_arr[i - offset] = read_buffer[i];
		}
		Record record_from_buffer = Record(record_byte_arr, length, meta_data.column_meta);
		record_arr.push_back(record_from_buffer);
	}
	fout.close();
}

SlottedPage::SlottedPage(std::string file, column_info column_meta_info, int page_num)
{
	page_idx = page_num;
	file_name = file;
	meta_data.entry_size = 0;
	meta_data.free_space_end_addr = PAGE_SIZE * (page_idx + 1);
	meta_data.column_meta = column_meta_info;
}

std::vector<Record> SlottedPage::get_record_list()
{
	return record_arr;
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
		std::cout << "location starts : " << record_ptr_arr[i].offset << " with length : " << record_ptr_arr[i].length << std::endl;
		record_arr[i].print_record();
		cnt++;
	}
}

int SlottedPage::write_page_on_disk()
{
	std::fstream fout;
	fout.open(file_name.c_str(), std::ios::binary | std::ios::out | std::ios::in);
	unsigned char null_byte = NULL;
	fout.seekp(page_idx * PAGE_SIZE);
	for (int i = 0; i < PAGE_SIZE; i++)
	{
		fout.write((char*)&null_byte, 1);
	}
	fout.seekp(page_idx * PAGE_SIZE);
	// page meta data 파일 쓰기
	std::vector<unsigned char> entry_size_byte_arr = int_to_byte(meta_data.entry_size);
	fout.write((char*)&entry_size_byte_arr[0], 4);
	std::vector<unsigned char> free_space_byte_arr = int_to_byte(meta_data.free_space_end_addr);
	fout.write((char*)&free_space_byte_arr[0], 4);

	for (int i = 0; i < meta_data.entry_size; i++)
	{
		std::vector<unsigned char> offset_byte_arr = int_to_byte(record_ptr_arr[i].offset);
		std::vector<unsigned char> length_byte_arr = int_to_byte(record_ptr_arr[i].length);
		unsigned char is_deleted_byte;
		if (record_ptr_arr[i].is_deleted) is_deleted_byte = 1;
		else is_deleted_byte = 0;
		fout.write((char*)&offset_byte_arr[0], 4);
		fout.write((char*)&length_byte_arr[0], 4);
		fout.write((char*)&is_deleted_byte, 1);
	}

	int current_seek = PAGE_SIZE * (page_idx + 1);
	for (int i = 0; i < meta_data.entry_size; i++)
	{
		if (record_ptr_arr[i].is_deleted) continue;
		std::vector<unsigned char> byte_arr = record_arr[i].to_byte_vector();
		int buffer_offset = record_ptr_arr[i].offset;
		int buffer_length = record_ptr_arr[i].length;
		current_seek -= buffer_length;
		fout.seekp(current_seek);

		fout.write((char*) & byte_arr[0], byte_arr.size());
	}
	fout.close();
	return 0;
}

int SlottedPage::add_record(Record tRecord)
{
	int new_record_length = tRecord.get_record_size();
	meta_data.free_space_end_addr -= new_record_length;
	int new_record_offset = meta_data.free_space_end_addr;
	bool new_record_isDeleted = false;
	record_arr.push_back(tRecord);
	record_ptr_arr.push_back(record_meta_data{ new_record_offset, new_record_length, new_record_isDeleted });
	meta_data.entry_size++;
	return 0;
}

bool SlottedPage::is_able_insert()
{
	int free_space_start = 8 + 9 * record_ptr_arr.size();
	if (free_space_start < meta_data.free_space_end_addr) return true;
	else return false;
}