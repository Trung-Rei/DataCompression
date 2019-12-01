#include "OutStream.h"

using namespace std;

int OutStream::getData(void* p, int pos, int count)
{
	long long tmp = ((long long)1 << count) - 1;
	return (*(long long*)p >> pos)& tmp;
}

void OutStream::addData(void* p, int pos, int data, int count)
{
	long long tmp = ((long long)1 << pos) - 1;
	*(long long*)p = *(long long*)p & tmp;
	tmp = ((long long)1 << count) - 1;
	tmp = tmp & (long long)data;
	*(long long*)p = *(long long*)p | (tmp << pos);
}

void OutStream::push(int data, int bitCount)
{
	if (_remainBit >= 32)
	{
		int tmp = getData(&_data, 0, 32);
		addData(_buffer + _buffCur, 0, tmp, 32);
		_data = _data >> 32;
		_remainBit -= 32;
		_buffCur += 4;
	}
	addData(&_data, _remainBit, data, bitCount);
	_remainBit += bitCount;
	if (_buffCur >= _writeSize)
	{
		_out.write(_buffer, _writeSize);
		_buffCur = 0;
	}
}

void OutStream::push(const char* data, int bitCount)
{
	while (bitCount >= 32)
	{
		push(*(int*)data, 32);
		bitCount -= 32;
		data += 4;
	}
	push(*(int*)data, bitCount);
}

void OutStream::writeAll()
{
	push(0, 0);
	int off = (_remainBit % 8 == 0 ? _remainBit / 8 : _remainBit / 8 + 1);
	_writeSize = _buffCur + off;
	_remainBit = 32;
	push(0, 0);
}

OutStream::OutStream(std::string filePath)
{
	_out.open(filePath, ios::binary);
	_buffer = new char[blockSize + 4];
	_buffCur = 0;
	_writeSize = blockSize;
	_data = 0;
	_remainBit = 0;
}

OutStream::~OutStream()
{
	writeAll();
	delete[] _buffer;
	_out.close();
}

const int OutStream::blockSize = 4096;