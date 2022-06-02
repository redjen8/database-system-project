#include "system_module.h"
#include "meta_data.h"
#include <iostream>

using namespace std;

void show_menu()
{
	cout << "------------------------------" << endl;
	cout << "Please Select Operation." << endl;
	cout << "(1) Insert new table" << endl;
	cout << "(2) Insert a record into table" << endl;
	cout << "(3) Select every data in table" << endl;
	cout << "(4) Select a record by its primary key" << endl;
	cout << "(5) Select column list of a table" << endl;
	cout << "(6) exit" << endl;
	cout << "------------------------------" << endl;
}

int main()
{
	SystemModule system_module = SystemModule();
	cout << "Finished Loading System Module." << endl;
	while (true)
	{
		show_menu();
		int user_select = 0;
		cin >> user_select;
		switch (user_select)
		{
			case 1:
			{
				// 테이블 삽입
				cout << "Please input new table name." << endl;
				string table_name;
				cin >> table_name;

				cout << "How many fixed length columns will be in the new table?" << endl;
				int fixed_column_length = 0;
				cin >> fixed_column_length;
				vector<string> column_name_arr;
				vector<int> fixed_column_length_arr;
				vector<bool> column_type_arr;

				for (int i = 0; i < fixed_column_length; i++)
				{
					cout << "Please input name of index (" + to_string(i) + ") fixed length column." << endl;
					string column_name = "";
					cin >> column_name;
					column_name_arr.push_back(column_name);
					cout << "Please input length of index (" + to_string(i) + ") fixed length column." << endl;
					int column_length = 0;
					cin >> column_length;
					fixed_column_length_arr.push_back(column_length);
					column_type_arr.push_back(false);
				}
				cout << "How many variable length columns will be in the new table?" << endl;
				int variable_column_length = 0;
				cin >> variable_column_length;
				
				for (int i = 0; i < variable_column_length; i++)
				{
					cout << "Please input name of index (" + to_string(i) + ") variable length column." << endl;
					string column_name = "";
					cin >> column_name;
					column_name_arr.push_back(column_name);
					column_type_arr.push_back(true);
				}
				
				cout << "Which index would be the primary column for the search?" << endl;
				int pk_index;
				cin >> pk_index;

				std::vector<block_store_loc> block_location;
				table_meta_data tmd = table_meta_data{ table_name, block_location, column_name_arr, fixed_column_length_arr, variable_column_length, fixed_column_length, pk_index };
				Table new_table = Table(tmd);
				system_module.insert_new_table(new_table);
				break;
			}
			case 2:
			{
				// 레코드를 테이블에 삽입
				cout << "Please input table name to insert record : ";
				string table_name_input;
				cin >> table_name_input;
				int idx = system_module.table_name_index_map[table_name_input];
				system_module.insert_new_record(idx);
				break;
			}
			case 3:
			{
				// 테이블의 모든 데이터 출력
				cout << "Please input table name to search every data : ";
				string table_name_input;
				cin >> table_name_input;
				int idx = system_module.table_name_index_map[table_name_input];
				system_module.get_table_every_data(idx);
				break;
			}
			case 4:
			{
				// 테이블의 PK 값으로 레코드 검색
				cout << "Please input table name to search by pk value : ";
				string table_name_input;
				cin >> table_name_input;
				cout << "Please input value for search pk value : ";
				string query;
				cin >> query;
				int idx = system_module.table_name_index_map[table_name_input];
				system_module.search_by_pk(idx, query);
				break;
			}
			case 5:
			{
				// 테이블의 컬럼 목록 출력
				cout << "Please input table name to search column list : ";
				string table_name_input;
				cin >> table_name_input;
				int idx = system_module.table_name_index_map[table_name_input];
				system_module.get_table_column_list(idx);
				break;
			}
			case 6:
			{
				cout << "Terminating Program.." << endl;
				return 0;
			}
			default:
			{
				cout << "Wrong Input. Please Input again." << endl;
				break;
			}
		}
	}
	return 0;
}