#pragma once
#include "CompressorBase.h"

//Adapter nén và giải nén
class DataCompressor
{
	CompressorBase* _algorithm; //bộ mã hóa cần sử dụng để nén và giải nén
	void encode(std::string inPath, OutStream& out);
	void decode(InStream& in, std::string outDirPath);
	static std::string getName(std::string path);

public:
	void encode(std::string inPath, std::string outPath);
	void decode(std::string inPath, std::string outDirPath);

	DataCompressor(CompressorBase* algo);
};

