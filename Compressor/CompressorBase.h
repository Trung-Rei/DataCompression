#pragma once
#include "../StreamHandler/InStream.h"
#include "../StreamHandler/OutStream.h"

//Base cho các class thực thi mã hóa/giải mã,
//gồm 2 phương thức nén và giải nén
class CompressorBase
{
public:
	//nén file có đường dẫn inPath và ghi dữ liệu đã nén ra OutStream
	virtual void encode(std::string inPath, OutStream& out) = 0;
	//giải nén đoạn dữ liệu đã nén từ InStream ra file có đường dẫn outPath
	virtual void decode(InStream& in, std::string outPath) = 0;
};
