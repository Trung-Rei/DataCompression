#include "LZWProgram.h"
#include <string>

using namespace std;

void LZWProgram::startMessage()
{
	cout << "COMPRESS FILE USING LZW ALGORITHM" << endl;
}

void LZWProgram::decompress(high_resolution_clock::time_point& start, high_resolution_clock::time_point& stop)
{
	string trash; getline(cin, trash);
	cout << "File to decompress (*.lzw): ";
	string inPath;
	getline(cin, inPath);
	cout << "Dictionary file (*.lzw.dict): ";
	string dictPath;
	getline(cin, dictPath);
	cout << "Output directory: ";
	string outDirPath;
	getline(cin, outDirPath);
	LZW lzw;
	cout << "Decompressing..." << endl;
	start = high_resolution_clock::now();
	lzw.decode(inPath, outDirPath, dictPath);
	stop = high_resolution_clock::now();
}

void LZWProgram::compress(high_resolution_clock::time_point& start, high_resolution_clock::time_point& stop)
{
	string trash; getline(cin, trash);
	cout << "Target to compress: ";
	string inPath;
	getline(cin, inPath);
	cout << "Output file (*.lzw): ";
	string outPath;
	getline(cin, outPath);
	LZW lzw;
	cout << "Compressing..." << endl;
	start = high_resolution_clock::now();
	lzw.encode(inPath, outPath, outPath + ".dict");
	stop = high_resolution_clock::now();
}
