#pragma once
#include <fstream>

class InStream
{
	std::ifstream _in;
	int _fileSize;
	static const int blockSize;
	char* _buffer;
	int _buffCur;
	int _readSize;
	long long _data;
	int _remainBit;
	unsigned _limit;
	static int getData(void* p, int pos, int count);
	static void addData(void* p, int pos, int data, int count);
public:
	int get(int bitCount);
	void get(char* data, int bitCount);
	void setLim(unsigned lim);
	void resetLim();

	InStream(std::string filePath);
	~InStream();
};

