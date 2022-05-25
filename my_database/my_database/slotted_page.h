#pragma once
#include <vector>
#include "record.h"
#define PAGE_SIZE 4096

typedef struct page_meta_data
{
	int page_size;
	int entry_size;
	int free_space_end_addr;
}page_meta_data;

typedef struct record_location
{
	int byte;
	int offset;
}record_location;

class SlottedPage
{
private:
	page_meta_data meta_data;
	std::vector<record_location> record_ptr_arr;

public:
	SlottedPage(page_meta_data input_meta);
	Record* get_record_list();
	void print_slotted_page();
	int write_page_on_disk();
};