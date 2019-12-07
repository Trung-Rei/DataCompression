#include "HuffmanProgram.h"
#include <string>

using namespace std;

void HuffmanProgram::startMessage()
{
	cout << "COMPRESS FILE AND FOLDER USING HUFFMAN ALGORITHM" << endl;
}

void HuffmanProgram::compress(high_resolution_clock::time_point& start, high_resolution_clock::time_point& stop)
{
	string trash; getline(cin, trash);
	cout << "Target to compress: ";
	string inPath;
	getline(cin, inPath);
	cout << "Output file (*.huff): ";
	string outPath;
	getline(cin, outPath);
	Huffman* huff = new Huffman;
	DataCompressor compressor(huff);
	cout << "Compressing..." << endl;
	start = high_resolution_clock::now();
	compressor.encode(inPath, outPath);
	stop = high_resolution_clock::now();
	delete huff;
}

void HuffmanProgram::decompress(high_resolution_clock::time_point& start, high_resolution_clock::time_point& stop)
{
	string trash; getline(cin, trash);
	cout << "File to decompress (*.huff): ";
	string inPath;
	getline(cin, inPath);
	cout << "Output directory: ";
	string outDirPath;
	getline(cin, outDirPath);
	Huffman* huff = new Huffman;
	DataCompressor compressor(huff);
	cout << "Decompressing..." << endl;
	start = high_resolution_clock::now();
	compressor.decode(inPath, outDirPath);
	stop = high_resolution_clock::now();
	delete huff;
}
