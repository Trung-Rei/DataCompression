#include "LZW.h"
#include <iostream>

using namespace std;

std::string LZW::c2s(int c)
{
	string tmp = " "; tmp[0] = c;
	return tmp;
}

void LZW::encode(InStream& in, OutStream& out, Dictionary& dict)
{
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
			if (dict.size() < maxDict) dict.add(prefix, c);
			prefix = c;
		}
	}
	out.writeAll();
}

void LZW::decode(InStream& in, OutStream& out, Dictionary& dict)
{
	while (true)
	{
		int code = in.get(12);
		if (code == -1) break;
		out.push(dict[code].c_str(), dict[code].length() * 8);
	}
	out.writeAll();
}

void LZW::exportDictionary(Dictionary& dict, OutStream& out)
{
	out.push(dict.size(), 32);
	for (int i = 0; i < dict.size(); ++i)
	{
		out.push(dict[i].length(), 32);
		out.push(dict[i].c_str(), dict[i].length() * 8);
	}
	out.writeAll();
}

void LZW::importDictionary(Dictionary& dict, InStream& in)
{
	int size = in.get(32);
	dict.resize(size);
	for (int i = 0; i < size; ++i)
	{
		int tmp = in.get(32);
		dict[i].resize(tmp);
		for (auto& j : dict[i])
			j = (char)in.get(8);
	}
}

LZW::Dictionary::Dictionary()
{
	_enDict.resize(maxDict);
	for (auto& i : _enDict) i.assign(256, -1);
	_deDict.assign(maxDict, "");
	for (int i = 0; i < 256; ++i) _deDict[i] = c2s(i);
	_size = 256;
}

int LZW::Dictionary::find(int prefix, int c)
{
	if (prefix == -1) return c;
	if (_enDict[prefix][c] != -1) return _enDict[prefix][c];
	return -1;
}

void LZW::Dictionary::add(int prefix, int c)
{
	_enDict[prefix][c] = _size;
	_deDict[_size] = _deDict[prefix] + c2s(c);
	++_size;
}

int LZW::Dictionary::size()
{
	return _size;
}

void LZW::Dictionary::resize(int size)
{
	_size = size;
	_deDict.resize(size);
}

std::string& LZW::Dictionary::operator[](int index)
{
	return _deDict[index];
}
