#pragma once
#include <fstream>

class OutStream
{
	std::ofstream _out;
	static const int blockSize;
	char* _buffer;
	int _buffCur;
	int _writeSize;
	long long _data;
	int _remainBit;
	static int getData(void* p, int pos, int count);
	static void addData(void* p, int pos, int data, int count);
	void writeAll();
public:
	void push(int data, int bitCount);
	void push(const char* data, int bitCount);

	OutStream(std::string filePath);
	~OutStream();
};

