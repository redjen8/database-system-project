#pragma once
// �÷� �̸� ��� column_name, column_type�� true�̸� ���� ���� �÷�, false�� ���� ���� Į��
typedef struct column_info
{
	std::vector<std::string> column_name;
	std::vector<bool> column_type;
	std::vector<int> fixed_column_length;
}column_info;