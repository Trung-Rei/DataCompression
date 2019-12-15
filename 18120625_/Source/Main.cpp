#include <iostream>
#include "Program/HuffmanProgram.h"
#include "Program/LZWProgram.h"

using namespace std;

bool askToContinue()
{
	cout << "'x' to exit, any to go back to main menu: ";
	char option;
	cin >> option;
	if (option == 'x')
		return false;
	else
		return true;
}

void startMessage()
{
	cout << "+---------------------------------+" << endl;
	cout << "|       COMPRESSION PROGRAM       |" << endl;
	cout << "|    Made by: Nguyen Tran Trung   |" << endl;
	cout << "|         ID: 18120625            |" << endl;
	cout << "|      Class: 18CTT5C             |" << endl;
	cout << "+---------------------------------+" << endl;
}

int menu()
{
	cout << "1. Huffman compression" << endl;
	cout << "2. LZW compression" << endl;
	cout << "Your option: ";
	int option;
	cin >> option;
	return option;
}

int main()
{
	startMessage();
	do {
		cout << "-----------------------------------" << endl;
		int option = menu();
		CompressorProgram* prog;
		if (option == 1)
			prog = new HuffmanProgram;
		else
			prog = new LZWProgram;
		prog->run();
		delete prog;
	} while (askToContinue());
	return 0;
}