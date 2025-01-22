#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

int main()
{
	ifstream img_file("DEATHSTAR5.png", ios::binary);

	if (!img_file.is_open())
	{
		cerr << "Error opening file!" << endl;
		return 1;
	}

	int packet_len = 32; // length of packet in Bytes
	int ctr = 0;
	char byte_char;
	int byte_int[packet_len];

	while(!img_file.eof())
	{
		for(int ii = 1; ii <= packet_len; ii++)
		{
			if(!img_file.eof())
			{
				img_file.read(&byte_char, 1);
				byte_int[ii] = static_cast<int>(byte_char);
				cout << hex << setw(2) << setfill('0') << byte_int[ii] << " ";
				ctr++;
			}
		}

		cout << endl;
	}

	cout << endl << "done" << endl;

        cout << dec << "Bytes: " << ctr << endl;

	return 0;
}
