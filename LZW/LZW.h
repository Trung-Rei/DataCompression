#pragma once
#include <unordered_map>
#include "InStream.h"
#include "OutStream.h"
#include <string>
#include <vector>

class LZW
{
	static const int maxDict = 4096;
	static std::string c2s(int c);
public:
	class Dictionary
	{
		std::vector<std::vector<short int> > _enDict;
		std::vector<std::string> _deDict;
		int _size;
		int find(int prefix, int c);
		void add(int prefix, int c);
		int size();
		void resize(int size);
		std::string& operator[](int index);
	public:
		Dictionary();
		friend class LZW;
	};
	static void encode(InStream& in, OutStream& out, Dictionary& dict);
	static void decode(InStream& in, OutStream& out, Dictionary& dict);
	static void exportDictionary(Dictionary& dict, OutStream& out);
	static void importDictionary(Dictionary& dict, InStream& in);
};

