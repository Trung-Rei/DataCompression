#pragma once
#include "CompressBase.h"

class Compression
{
	CompressBase* _algorithm;
	void encode(std::string inPath, OutStream& out);
	void decode(InStream& in, std::string outDirPath);
	static std::string getName(std::string path);
public:
	void encode(std::string inPath, std::string outPath);
	void decode(std::string inPath, std::string outDirPath);

	Compression(CompressBase* algo);
};

