#include "CompressorProgram.h"

using namespace std;

int CompressorProgram::input()
{
	cout << "1 to compress, 2 to decompress: ";
	int option;
	cin >> option;
	return option;
}

bool CompressorProgram::askToContinue()
{
	cout << "'x' to exit, any to restart: ";
	char option;
	cin >> option;
	if (option == 'x')
		return false;
	else
		return true;
}

void CompressorProgram::run()
{
	do {
		cout << "-----------------------------------" << endl;
		startMessage();
		int option = input();
		high_resolution_clock::time_point start, stop;
		if (option == 1)
			compress(start, stop);
		else
			decompress(start, stop);
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << "Processing time: " << duration.count() / 1000.0 << "s" << endl;
	} while (askToContinue());
}
