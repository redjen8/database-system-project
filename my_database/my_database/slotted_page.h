#pragma once
#include "record.h"
#include <vector>

#define PAGE_SIZE 4096

//페이지 자체 메타 데이터, 페이지 크기, 레코드 개수, free space 마지막 위치, 컬럼 정보
typedef struct page_meta_data
{
	int entry_size;
	int free_space_end_addr;
	column_info column_meta;
}page_meta_data;

//페이지 내에 저장된 레코드의 메타 데이터
typedef struct record_meta_data
{
	int offset;
	int length;
	bool is_deleted;
}record_meta_data;

class SlottedPage
{
private:
	int page_idx; //page_idx * (PAGE_SIZE) ~ page_idx * (PAGE_SIZE + 1) 까지
	std::string file_name;
	page_meta_data meta_data;
	std::vector<record_meta_data> record_ptr_arr;
	std::vector<Record> record_arr;
public:
	SlottedPage(std::string file_name, column_info column_meta, int page_start, int page_end);
	SlottedPage(std::string file_name, int page_idx);
	Record* get_record_list();
	void print_slotted_page();
	int write_page_on_disk();
	int add_record(Record tRecord);
};
