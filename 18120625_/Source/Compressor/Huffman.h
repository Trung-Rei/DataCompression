#pragma once
#include "CompressorBase.h"

//bộ mã nén sử dụng thuật toán Huffman
class Huffman : public CompressorBase
{
	//cấu trúc node cho cây Huffman
	struct Node
	{
		long long freq; //tần số của kí tự chr
		unsigned char chr;
		Node* left, * right;
		Node(long long f, char c, Node* l, Node* r) : freq(f), chr(c), left(l), right(r) {}
	};
	//struct compare dùng cho priority queue
	struct Compare
	{
		bool operator()(Node* x, Node* y) { return x->freq > y->freq; }
	};
	Node* buildHuffmanTree(std::string inPath, long long& outputSize);
	void buildDictionary(unsigned char** dictionary, Node* huffmanTree);
	void _buildDictionary(unsigned char** dictionary, Node* node, int depth, char* code);
	void readHuffmanTree(InStream& in, Node*& node);
	void writeHuffmanTree(OutStream& out, Node* node);
	static void setBit(char* target, int pos, int bit);
	void disposeTree(Node* node);

public:
	void encode(std::string inPath, OutStream& out);
	void decode(InStream& in, std::string outPath);
};
