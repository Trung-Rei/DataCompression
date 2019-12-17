#include "LZW.h"

using namespace std;

void LZW::encode(std::string inPath, OutStream& out)
{
	InStream in(inPath);
	Dictionary dict(maxDict - 1);
	int prefix = -1;
	while (true)
	{
		int c = in.get(8);
		if (c == -1)
		{
			if (prefix != -1) out.push(prefix, 12);
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
	out.push(maxDict - 1, 12);
}

void LZW::decode(InStream& in, std::string outPath)
{
	in.resetLim();
	OutStream out(outPath);
	Dictionary dict(maxDict - 1);
	int prefix = -1;
	while (true)
	{
		int c = in.get(12);
		if (c == maxDict - 1) return;
		if (!dict.find(c)) dict.add(prefix, (unsigned char)dict[prefix][0]);
		out.push(dict[c].data(), dict[c].length() * 8);
		if (!dict.isFull() && dict.find(prefix, (unsigned char)dict[c][0]) == -1)
			dict.add(prefix, (unsigned char)dict[c][0]);
		prefix = c;
	}
}

//tìm xem 'chuỗi có từ mã prefix + kí tự c' có trong từ điển chưa
//nếu có thì trả về từ mã của chuỗi đó
int LZW::Dictionary::find(int prefix, int c)
{
	if (prefix == -1) return c;
	return _enDict[prefix][c];
}

//Đã tồn tại chuỗi có từ mã code chưa
bool LZW::Dictionary::find(int code)
{
	return !_deDict[code].empty();
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

// chuyển kí tự thành chuỗi string
std::string LZW::c2s(int c)
{
	string tmp = " "; tmp[0] = c;
	return tmp;
}

//sử dụng mã hóa ký tự 12bit, 2^12 = 4096
const int LZW::maxDict = 4096;