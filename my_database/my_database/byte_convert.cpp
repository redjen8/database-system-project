#include <vector>
#include <format>
#include <iostream>

int byte_arr_to_int(unsigned char* x)
{
	int result = 0;
	for (int i = 0; i < 4; i++)
		result += x[3 - i] << (i * 8);
	return result;
}

std::vector<unsigned char> int_to_byte(int x)
{
	std::vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (x >> (i * 8));
	return arrayOfByte;
}


void print_byte_arr(unsigned char* byte_arr, int arr_length)
{
	int j = 0;
	for (int i = 0; i < arr_length; i++)
	{
		j++;
		std::cout << std::format("{:2x}", byte_arr[i]) << " ";
		if (j == 16)
		{
			j = 0;
			std::cout << std::endl;
		}
	}
	std::cout << std::endl << std::endl;
}
