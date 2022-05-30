#pragma once
#include "record.h"
#include <vector>

#define PAGE_SIZE 4096

//������ ��ü ��Ÿ ������, ������ ũ��, ���ڵ� ����, free space ������ ��ġ, �÷� ����
typedef struct page_meta_data
{
	int entry_size;
	int free_space_end_addr;
	column_info column_meta;
}page_meta_data;

//������ ���� ����� ���ڵ��� ��Ÿ ������
typedef struct record_meta_data
{
	int offset;
	int length;
	bool is_deleted;
}record_meta_data;

class SlottedPage
{
private:
	int page_idx;
	page_meta_data meta_data;
	std::vector<record_meta_data> record_ptr_arr;
	std::vector<Record> record_arr;

public:
	SlottedPage(int page_idx);
	Record* get_record_list();
	void print_slotted_page();
	int write_page_on_disk();
	int read_from_disk(int page_start, int page_end);
	int add_record(Record tRecord);
};
