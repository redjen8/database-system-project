#include <vector>


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