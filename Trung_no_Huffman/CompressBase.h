#pragma once
#include "InStream.h"
#include "OutStream.h"

class CompressBase
{
protected:
	virtual void encode(std::string inPath, OutStream& out) = 0;
	virtual void decode(InStream& in, std::string outPath) = 0;
	friend class Compression;
};
