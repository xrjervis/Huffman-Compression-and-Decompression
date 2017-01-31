#include "huffman.h"
#include <iostream>
using namespace std;

int main()
{
	string inputFile;
	char c;
	cout << "                              Instructions                              " << endl;
	cout << "========================================================================" << endl;
	cout << endl;
	cout << "   1. Huffman Compress. (Input file is named as resource.txt)           " << endl;
	cout << endl;
	cout << "   2. Huffman Decompress. (Input file is named as compressed.txt)       " << endl;
	cout << endl;
	cout << "	After decompression, decompressed.txt will be generated.             " << endl;
	cout << "   Compare it with resource.txt.                                        " << endl;
	cout << endl;
	cout << "========================================================================" << endl;
	cout << endl;
	cout << "         Made by Rui Xie, from 01/20/2017 to 01/21/2017                 " << endl;
	cout << endl;
	cout << "========================================================================" << endl;

	while (true)
	{
		cout << "  Please input your choice (1 - 2) : ";
		cin >> c;
		if (c == '1')
		{
			cout << "Start compression...." << endl;
			Huffman *h = new Huffman("resource.txt", "compressed.txt");
			h->compress();
			cout << "  Compress successfully. compressed.txt has been generated." << endl;
			system("pause");
			return 0;
		}
		else if (c == '2')
		{
			cout << "Start decompression...." << endl;
			Huffman *h2 = new Huffman("compressed.txt", "decompressed.txt");
			h2->decompress();
			cout << "  Decompress successfully. decompressed.txt has been generated." << endl;
			system("pause");
			return 0;
		}
		else
			cout << "Your input is not correct. Please try again!\n" << endl;		
	}
}