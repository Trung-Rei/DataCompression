﻿#include "InStream.h"

using namespace std;

//lấy count bit từ vùng nhớ p bắt đầu từ vị trí pos
int InStream::getData(void* p, int pos, int count)
{
	long long tmp = ((long long)1 << count) - 1;
	return (*(long long*)p >> pos)& tmp;
}

//ghi count bit đầu của data vào vùng nhớ p bắt đầu tại vị trí pos
void InStream::addData(void* p, int pos, int data, int count)
{
	long long tmp = ((long long)1 << pos) - 1;
	*(long long*)p = *(long long*)p & tmp;
	tmp = ((long long)1 << count) - 1;
	tmp = tmp & (long long)data;
	*(long long*)p = *(long long*)p | (tmp << pos);
}

// lấy số lượng bit bitcount từ bộ đệm
// giới hạn mỗi lần lấy không quá 32bit
int InStream::get(int bitCount)
{
	//buffer trống thì đọc _readSize byte dữ liệu từ file vào
	if (_buffCur == _readSize && _fileSize > 0)
	{
		if (_readSize > _fileSize) _readSize = _fileSize;
		_in.read(_buffer, _readSize);
		_fileSize -= _readSize;
		_buffCur = 0;
	}
	//nếu số bit còn lại trong data không đủ thì đẩy từ buffer vào
	if (_remainBit < 32 && _buffCur < _readSize)
	{
		int tmp = getData(_buffer + _buffCur, 0, 32);
		addData(&_data, _remainBit, tmp, 32);
		_remainBit += (_readSize - _buffCur < 4 ? (_readSize - _buffCur) * 8 : 32);
		_buffCur += 4;
		if (_buffCur > _readSize) _buffCur = _readSize;
	}
	if (_remainBit < bitCount || _limit < bitCount) return -1; //kiểm tra có đủ điều kiện để lấy bit ko
	int retVal = getData(&_data, 0, bitCount);
	_data = _data >> bitCount;
	_remainBit -= bitCount;
	_limit -= bitCount;
	return retVal;
}

//ghi bitcount bit từ file vào vùng nhớ data
//bitCount ko giới hạn
void InStream::get(char* data, int bitCount)
{
	while (bitCount >= 16)
	{
		*(short*)data = (short)get(16);
		bitCount -= 16;
		data += 2;
	}
	short tmp;
	while ((tmp = get(bitCount)) == -1)
		--bitCount;
	int n = (bitCount % 8 == 0 ? bitCount / 8 : bitCount / 8 + 1);
	for (int i = 0; i < n; ++i)
		data[i] = ((char*)&tmp)[i];
}

//đặt giới hạn số bit cho phép đọc là lim bit
void InStream::setLim(unsigned long long lim)
{
	_limit = lim;
}

//bỏ giới hạn lim
void InStream::resetLim()
{
	_limit = long long(-1);
}

InStream::InStream(std::string filePath)
{
	_in.open(filePath, ios::binary);
	_in.seekg(0, _in.end);
	_fileSize = _in.tellg();
	_in.seekg(0, _in.beg);
	_limit = long long(-1);
	_buffer = new char[blockSize];
	_buffCur = blockSize;
	_readSize = blockSize;
	_data = 0;
	_remainBit = 0;
}

InStream::~InStream()
{
	delete[] _buffer;
	_in.close();
}

const int InStream::blockSize = 4096;