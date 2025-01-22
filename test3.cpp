#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int Pktlen = 0x23;                         		        //set packet length to number of Bytes plus 3 for the buffer
    int Byte_ctr = 0, tx_ctr = 0;
    char byte_char;
    vector<int> data = {};

    ifstream img_file("DEATHSTAR5.png", ios::binary);	// open the image file

    while(!img_file.eof())
    {
        img_file.read(&byte_char, 1);
        data.push_back(static_cast<int>(byte_char));
    }

    for(int ii = 1; ii <= data.size(); ii++)
    {
        cout << hex << data[ii];
	Byte_ctr = ii;
    }

    cout << endl << dec << "Done. Byte count: " << Byte_ctr << endl;

}
