#pragma once
#include <unordered_map>
#include "../StreamHandler/InStream.h"
#include "../StreamHandler/OutStream.h"
#include <vector>

//bộ mã hóa LZW
class LZW
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
		void add(int prefix, int c);
		bool isFull();
		int size();
		std::string& operator[](int index);

		Dictionary(int maxSize);
	};
	void exportDictionary(Dictionary& dict, std::string outPath);
	void importDictionary(Dictionary& dict, std::string inPath);
	static std::string getName(std::string path);
	static std::string c2s(int c);

public:
	void encode(std::string inPath, std::string outPath, std::string dictPath);
	void decode(std::string inPath, std::string outDirPath, std::string dictPath);
};

