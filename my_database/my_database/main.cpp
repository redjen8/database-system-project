#include "system_module.h"
#include "column_info.h"
#include <iostream>

using namespace std;

void show_menu()
{
	cout << endl << "Please Select Operation." << endl;
	cout << "(1) Insert new table" << endl;
	cout << "(2) Insert a record into table" << endl;
	cout << "(3) Select every data in table" << endl;
	cout << "(4) Select a record by its primary key" << endl;
	cout << "(5) Select column list of a table" << endl;
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

				column_info new_table_column = column_info{column_name_arr, column_type_arr, fixed_column_length_arr};
				Table new_table = Table(new_table_column);
				system_module.insert_new_table(new_table);
				break;
			}
			case 2:
			{
				// 레코드를 테이블에 삽입
				system_module.insert_new_record();
				break;
			}
			case 3:
			{
				// 테이블의 모든 데이터 출력
				system_module.get_table_every_data();
				break;
			}
			case 4:
			{
				// 테이블의 PK 값으로 레코드 검색
				system_module.search_by_pk();
				break;
			}
			case 5:
			{
				// 테이블의 컬럼 목록 출력
				system_module.get_table_column_list();
				break;
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