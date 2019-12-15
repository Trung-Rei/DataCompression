#pragma once
#include "CompressorProgram.h"
#include "../Compressor/DataCompressor.h"
#include "../Compressor/LZW.h"

//program nén file theo thuật toán LZW
class LZWProgram :
	public CompressorProgram
{
	void startMessage();
	void compress(high_resolution_clock::time_point& start, high_resolution_clock::time_point& stop);
	void decompress(high_resolution_clock::time_point& start, high_resolution_clock::time_point& stop);
};

