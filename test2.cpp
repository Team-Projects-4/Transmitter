#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main()
{
    string line;
    int ctr = 0;
    ifstream img_file("DEATHSTAR5.png", ios::binary);

    while(!img_file.eof())
    {
        getline(img_file, line);

        printf("%s", line);
        printf("\n");
        ctr++;
    }

    printf("Lines: %d\n", ctr);
}
