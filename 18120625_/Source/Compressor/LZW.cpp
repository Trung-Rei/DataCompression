#include "LZW.h"
#include <iostream>

using namespace std;

// chuyển kí tự thành chuỗi string
std::string LZW::c2s(int c)
{
	string tmp = " "; tmp[0] = c;
	return tmp;
}

//xuất từ điển dict ra file co đường dẫn outPath
void LZW::exportDictionary(Dictionary& dict, std::string outPath)
{
	OutStream out(outPath);
	out.push(dict.size(), 32);
	for (int i = 0; i < dict.size(); ++i)
	{
		out.push(dict[i].length(), 32);
		out.push(dict[i].c_str(), dict[i].length() * 8);
	}
}

//nhập từ điển từ file inPath
void LZW::importDictionary(Dictionary& dict, std::string inPath)
{
	InStream in(inPath);
	int size = in.get(32);
	for (int i = 0; i < size; ++i)
	{
		int tmp = in.get(32);
		dict[i].resize(tmp);
		for (auto& j : dict[i])
			j = in.get(8);
	}
}

//trả về tên file có đường dẫn path
std::string LZW::getName(std::string path)
{
	int last = path.length();
	while (path[last - 1] == 47 || path[last - 1] == 92) --last;
	int first = last;
	while (first > 0 && path[first - 1] != 47 && path[first - 1] != 92) --first;
	return path.substr(first, last - first);
}

//nén file inPath ra file outPath và xuất ra từ điển dictPath
void LZW::encode(std::string inPath, std::string outPath, std::string dictPath)
{
	InStream in(inPath);
	OutStream out(outPath);
	string fileName = getName(inPath);
	out.push(fileName.size(), 8);
	out.push(fileName.c_str(), fileName.size() * 8);
	Dictionary dict(maxDict);
	int prefix = -1;
	while (true)
	{
		int c = in.get(8);
		if (c == -1)
		{
			out.push(prefix, 12);
			break;
		}
		int nextPrefix = dict.find(prefix, c);
		if (nextPrefix != -1)
		{
			prefix = nextPrefix;
		}
		else
		{
			out.push(prefix, 12);
			if (!dict.isFull()) dict.add(prefix, c);
			prefix = c;
		}
	}
	exportDictionary(dict, dictPath);
}

//giải nén file inPath vào thư mục outDirPath, file từ điển là dictPath
void LZW::decode(std::string inPath, std::string outDirPath, std::string dictPath)
{
	Dictionary dict(maxDict);
	importDictionary(dict, dictPath);
	InStream in(inPath);
	int nameChrSize = in.get(8);
	char* tmp = new char[nameChrSize];
	in.get(tmp, nameChrSize * 8);
	string fileName = string(tmp, nameChrSize);
	OutStream out(outDirPath + "/" + fileName);
	int c;
	while ((c = in.get(12)) != -1)
	{
		out.push(dict[c].c_str(), dict[c].length() * 8);
	}
}

//tìm xem 'chuỗi có từ mã prefix + kí tự c' có trong từ điển chưa
//nếu có thì trả về từ mã của chuỗi đó
int LZW::Dictionary::find(int prefix, int c)
{
	if (prefix == -1) return c;
	return _enDict[prefix][c];
}

//thêm 'chuỗi có từ mã prefix + kí tự c' vào từ điển
void LZW::Dictionary::add(int prefix, int c)
{
	_enDict[prefix][c] = _size;
	_deDict[_size] = _deDict[prefix] + c2s(c);
	++_size;
}

//kiểm tra xem từ điển đã full chưa
bool LZW::Dictionary::isFull()
{
	return _size == _maxSize;
}

int LZW::Dictionary::size()
{
	return _size;
}

//trả về chuỗi có từ mã index
std::string& LZW::Dictionary::operator[](int index)
{
	return _deDict[index];
}

//khởi tạo từ điển với size = maxSize
LZW::Dictionary::Dictionary(int maxSize)
{
	_maxSize = maxSize;
	_enDict.resize(_maxSize);
	for (auto& i : _enDict) i.assign(256, -1);
	_deDict.resize(_maxSize);
	for (int i = 0; i < 256; ++i) _deDict[i] = c2s(i);
	_size = 256;
}

//sử dụng mã hóa ký tự 12bit, 2^12 = 4096
const int LZW::maxDict = 4096;