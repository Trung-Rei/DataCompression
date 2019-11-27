#pragma once
#include "InStream.h"
#include "OutStream.h"

class Huffman
{
	struct Node
	{
		int freq;
		unsigned char chr;
		Node* left, * right;
		Node(int f, char c, Node* l, Node* r) : freq(f), chr(c), left(l), right(r) {}
	};
	struct Compare
	{
		bool operator()(Node* x, Node* y) { return x->freq > y->freq; }
	};
	Node* _huffmanTree;
	unsigned char** _dictionary;
	void buildHuffmanTree(const char* inPath);
	void buildDictionary();
	void _buildDictionary(Node* node, int depth, char* code);
	void readHuffmanTree(InStream& in, Node*& node);
	void writeHuffmanTree(OutStream& out, Node* node);
	static void setBit(char* target, int pos, int bit);
	static void disposeTree(Node* root);
public:
	void encode(const char* inPath, OutStream& out);
	void decode(InStream& in, const char* outPath);

	Huffman();
	~Huffman();
};
