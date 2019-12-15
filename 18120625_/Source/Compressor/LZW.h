#pragma once
#include "CompressorBase.h"
#include <vector>
#include <string>

//bộ mã nén sử dụng thuật toán LZW
class LZW :
	public CompressorBase
{
	static const int maxDict; // max size của từ điển, maxDict = 2^n

	//class từ điển
	class Dictionary
	{
		int _maxSize;
		int _size;
		std::vector<std::vector<short> > _enDict;
		std::vector<std::string> _deDict;
	public:
		int find(int prefix, int c);
		bool find(int code);
		void add(int prefix, int c);
		bool isFull();
		int size();
		std::string& operator[](int index);

		Dictionary(int maxSize);
	};
	static std::string c2s(int c);

public:
	void encode(std::string inPath, OutStream& out);
	void decode(InStream& in, std::string outPath);
};

