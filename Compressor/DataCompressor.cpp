#include "DataCompressor.h"
#include <filesystem>

using namespace std;

//thực hiện việc nén thư mục,
//đầu vào là thư mục cần nén, đầu ra là out stream của file nén
//cây thư mục được nén theo đệ quy
void DataCompressor::encode(std::string inPath, OutStream& out)
{
	string name = getName(inPath);
	out.push(name.length(), 8);
	out.push(name.c_str(), name.length() * 8);
	filesystem::directory_entry entry(inPath);
	if (!entry.is_directory())
	{
		out.push(0, 1);
		_algorithm->encode(inPath, out);
		return;
	}
	out.push(1, 1);
	filesystem::directory_iterator tmpItr(inPath);
	int itemCount = 0;
	for (auto& i : tmpItr) ++itemCount;
	out.push(itemCount, 32);
	filesystem::directory_iterator itr(inPath);
	for (auto& i : itr)
		encode(i.path().string(), out);
}

//thực hiện việc giải nén thư mục,
//đầu vào là đoạn mã nén từ in stream, outDirPath là thư mục để giải nén vào
void DataCompressor::decode(InStream& in, std::string outDirPath)
{
	in.resetLim();
	int nameChrCount = in.get(8);
	char* tmp = new char[nameChrCount];
	in.get(tmp, nameChrCount * 8);
	string name = string(tmp, nameChrCount);
	string targetPath = outDirPath + "/" + name;
	if (in.get(1) == 0)
	{
		_algorithm->decode(in, targetPath);
		return;
	}
	filesystem::create_directory(targetPath);
	int itemCount = in.get(32);
	for (int i = 0; i < itemCount; ++i)
		decode(in, targetPath);
}

//lấy tên đối tượng từ đường dẫn
std::string DataCompressor::getName(std::string path)
{
	int last = path.length();
	while (path[last - 1] == 47 || path[last - 1] == 92) --last;
	int first = last;
	while (first > 0 && path[first - 1] != 47 && path[first - 1] != 92) --first;
	return path.substr(first, last - first);
}
//thực hiện nén file hoặc thư mục có đường dẫn inPath ra file có đường dẫn outPath
void DataCompressor::encode(std::string inPath, std::string outPath)
{
	OutStream out(outPath);
	encode(inPath, out);
}

//thực hiện giải nén file có đường dẫn inPath vào thư mục có đường dẫn outDirPath
void DataCompressor::decode(std::string inPath, std::string outDirPath)
{
	InStream in(inPath);
	decode(in, outDirPath);
}

//khởi tạo máy nén sử dụng bộ mã hóa algo
DataCompressor::DataCompressor(CompressorBase* algo)
{
	_algorithm = algo;
}
