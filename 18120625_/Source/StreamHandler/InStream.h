#pragma once
#include <fstream>

//Adapter cho ifstream
class InStream
{
	std::ifstream _in;
	long long _fileSize;
	static const int blockSize; //số byte dữ liệu mỗi lần đọc vào từ đĩa
	char* _buffer; //bộ nhớ đệm chứa dữ liệu đọc vào từ file
	int _buffCur; 
	int _readSize;
	long long _data; //bộ đệm để xử lí lấy bit
	int _remainBit;
	unsigned long long _limit; //giới hạn đọc dữ liệu
	static int getData(void* p, int pos, int count);
	static void addData(void* p, int pos, int data, int count);
public:
	int get(int bitCount);
	void get(char* data, int bitCount);
	void setLim(unsigned long long lim);
	void resetLim();

	InStream(std::string filePath);
	~InStream();
};

