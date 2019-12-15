#include "LZWProgram.h"
#include <string>

using namespace std;

void LZWProgram::startMessage()
{
	cout << "LZW COMPRESSION" << endl;
}

void LZWProgram::decompress(high_resolution_clock::time_point& start, high_resolution_clock::time_point& stop)
{
	string trash; getline(cin, trash);
	cout << "File to decompress (*.lzw): ";
	string inPath;
	getline(cin, inPath);
	cout << "Output directory: ";
	string outDirPath;
	getline(cin, outDirPath);
	LZW* lzw = new LZW;
	DataCompressor compressor(lzw);
	cout << "Decompressing..." << endl;
	start = high_resolution_clock::now();
	compressor.decode(inPath, outDirPath);
	stop = high_resolution_clock::now();
	delete lzw;
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
	LZW* lzw = new LZW;
	DataCompressor compressor(lzw);
	cout << "Compressing..." << endl;
	start = high_resolution_clock::now();
	compressor.encode(inPath, outPath);
	stop = high_resolution_clock::now();
	delete lzw;
}
