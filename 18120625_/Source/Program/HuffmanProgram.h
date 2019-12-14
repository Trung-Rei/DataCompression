#pragma once
#include "CompressorProgram.h"
#include "../Compressor/DataCompressor.h"
#include "../Compressor/Huffman.h"

//program nén file và folder theo thuật toán Huffman
class HuffmanProgram :
	public CompressorProgram
{
	void startMessage();
	void compress(high_resolution_clock::time_point& start, high_resolution_clock::time_point& stop);
	void decompress(high_resolution_clock::time_point& start, high_resolution_clock::time_point& stop);
};

