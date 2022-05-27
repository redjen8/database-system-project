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

class SlottedPage
{
private:
	page_meta_data meta_data;
	std::vector<Record> record_arr;

public:
	SlottedPage();
	Record* get_record_list();
	void print_slotted_page();
	int write_page_on_disk();
	int add_record(Record tRecord);
};
