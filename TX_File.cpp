#include "cc1100_raspi.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include <getopt.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

using namespace std;

#define PACKAGE    "CC1100 SW"
#define VERSION_SW "0.9.6"

#define INTERVAL_1S_TIMER 1000


//--------------------------[Global CC1100 variables]--------------------------
uint8_t Tx_fifo[FIFOBUFFER], Rx_fifo[FIFOBUFFER];
uint8_t My_addr, Tx_addr, Rx_addr, Pktlen, pktlen, Lqi, Rssi;
uint8_t rx_addr,sender,lqi;
int8_t rssi_dbm;

int cc1100_freq_select, cc1100_mode_select, cc1100_channel_select;

uint32_t prev_millis_1s_timer = 0;

uint8_t cc1100_debug;								//set CC1100 lib in no-debug output mode
uint8_t tx_retries;
uint8_t rx_demo_addr;
int interval;

CC1100 cc1100;

//-------------------------- [End] --------------------------

void print_help(int exval) {
	printf("%s,%s by CW\r\n", PACKAGE, VERSION_SW);
	printf("%s [-h] [-V] [-a My_Addr] [-r RxDemo_Addr] [-i Msg_Interval] [-t tx_retries] [-c channel] [-f frequency]\r\n", PACKAGE);
	printf("          [-m modulation]\n\r\n\r");
	printf("  -h              			print this help and exit\r\n");
	printf("  -V              			print version and exit\r\n\r\n");
	printf("  -v              			set verbose flag\r\n");
	printf("  -a my address [1-255] 		set my address\r\n\r\n");
	printf("  -r rx address [1-255] 	  	set RxDemo receiver address\r\n\r\n");
	printf("  -i interval ms[1-6000] 	  	sets message interval timing\r\n\r\n");
	printf("  -t tx_retries [0-255] 	  	sets message send retries\r\n\r\n");
	printf("  -c channel 	[1-255] 		set transmit channel\r\n");
	printf("  -f frequency  [315,434,868,915] (1, 2, 3, 4) 	set ISM band\r\n\r\n");
	printf("  -m modulation [1,38,100,250,500,4] (1, 2, 3, 4, 5, 6)	set modulation\r\n\r\n");

	exit(exval);
}


//|============================ Main ============================|
int main() 
{

    // --------------- OPERATION PARAMETERS ---------------
    cc1100_debug = 0;
    My_addr = 0x01;
    rx_demo_addr = 0x02;
    Rx_addr = rx_demo_addr;                     //receiver address
    interval = 1;
    tx_retries = 0x00;
    cc1100_channel_select = 0x01;
    cc1100_freq_select = 0x04;
    cc1100_mode_select = 0x05;

	//------------- welcome message ------------------------
	printf("Raspberry CC1101 SPI Library test\n");

	//------------- hardware setup ------------------------
	wiringPiSetup();			//setup wiringPi library

	cc1100.begin(My_addr);			            //setup cc1000 RF IC
	cc1100.sidle();
    cc1100.set_mode(cc1100_mode_select);        //set modulation mode 1 = GFSK_1_2_kb; 2 = GFSK_38_4_kb; 3 = GFSK_100_kb; 4 = MSK_250_kb; 5 = MSK_500_kb; 6 = OOK_4_8_kb
	cc1100.set_ISM(cc1100_freq_select);         //set ISM Band 1=315MHz; 2=433MHz; 3=868MHz; 4=915MHz
	cc1100.set_channel(cc1100_channel_select);  //set channel
	cc1100.set_output_power_level(0);           //set PA level
	cc1100.set_myaddr(My_addr);
	
	cc1100.receive();

	cc1100.show_main_settings();             //shows setting debug messages to UART
	cc1100.show_register_settings();

	//------------------------- Main Loop ------------------------
	for (;;)
	{
		delay(1);   //delay to reduce system load. Leonard is unsure if this is needed, but removing it will not increase tx speed much

        int Datalen = 0x3d;             // length of the portion of file data sent in each transmission
        int Pktlen = Datalen + 0x03;    //set packet length to number of Bytes plus 3 for the buffer info
        char byte_char;                 // temporary storage of file data
        vector<int> file_data;               // very important variable here: the entire file will be stored in it. Remember that the index starts at 0

        ifstream img_file("DEATHSTAR5.png", ios::binary);   // open the image file

        // read all file data into the data vector
        while(!img_file.eof())
        {
            img_file.read(&byte_char, 1); //read single Byte as a character from file to temporary variable byte_char
            file_data.push_back(static_cast<int>(byte_char)); // append byte_char transformed to integer to the data vector
        }

        for(int ii = 0; ii < file_data.size(); ii++)    // loop through each element of the data vector
        {
            for(int jj = 1; jj <= Datalen; jj++)        // loop to put a portion of file data into the Tx_fifo array
            {
                if(ii <= file_data.size())              // check to make sure were not accessing elements outside the vector
                {
                    Tx_fifo[jj+2] = file_data[ii];      // load data into the array at element jj+2 becuase the first three elements are other stuff
                }
                if(jj != Datalen)                       // go to the next element in data array unless Tx_fifo is full
                {
                    ii++;
                }
            }

            // cc1100.wor_disable(); // this puts the radio into a sleep mode. commented out to potentially increase speed
            cc1100.sent_packet(My_addr, Rx_addr, Tx_fifo, Pktlen, tx_retries);  // transmit the data in Tx_fifo
            cout << "TX Timestamp: " << millis() << endl;                       // print a timestamp

        }
            cout << endl << "done" << endl;
            cout << dec << "Bytes: " << file_data.size() << endl; // just displays how long the file was
            break;
    }
        // ------------------END OF MAIN LOOP-----------------------------
    return 0;
} // all done :)

//-------------------- END OF MAIN --------------------------------
