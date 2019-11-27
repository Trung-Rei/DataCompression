#include "Huffman.h"
#include <queue>

using namespace std;

void Huffman::buildHuffmanTree(const char* inPath)
{
	InStream in(inPath);
	Node* address[256] = { nullptr };
	int c;
	while ((c = in.get(8)) != -1)
	{
		if (address[c] == nullptr)
			address[c] = new Node(0, c, nullptr, nullptr);
		++address[c]->freq;
	}
	priority_queue<Node*, vector<Node*>, Compare> minHeap;
	for (int i = 0; i < 256; ++i)
		if (address[i] != nullptr) minHeap.push(address[i]);
	while (minHeap.size() > 1)
	{
		Node* x = minHeap.top(); minHeap.pop();
		Node* y = minHeap.top(); minHeap.pop();
		Node* node = new Node(x->freq + y->freq, 0, x, y);
		minHeap.push(node);
	}
	_huffmanTree = minHeap.top();
}

void Huffman::buildDictionary()
{
	char code[32] = { 0 };
	_buildDictionary(_huffmanTree, 0, code);
}

void Huffman::_buildDictionary(Node* node, int depth, char* code)
{
	if (node->left == nullptr)
	{
		_dictionary[node->chr][0] = depth;
		int i;
		for (i = 0; i < depth / 8; ++i) _dictionary[node->chr][i + 1] = code[i];
		if (depth % 8 != 0)  _dictionary[node->chr][i + 1] = code[i];
		return;
	}
	setBit(code, depth, 0);
	_buildDictionary(node->left, depth + 1, code);
	setBit(code, depth, 1);
	_buildDictionary(node->right, depth + 1, code);
}

void Huffman::readHuffmanTree(InStream& in, Node*& node)
{
	int state = in.get(2);
	node = new Node(0, 0, nullptr, nullptr);
	if (state == 0)
	{
		node->chr = in.get(8);
		return;
	}
	readHuffmanTree(in, node->left);
	readHuffmanTree(in, node->right);
}

void Huffman::writeHuffmanTree(OutStream& out, Node* node)
{
	int state = 0;
	if (node->right != nullptr) state = 3;
	out.push(state, 2);
	if (state == 0)
	{
		out.push(node->chr, 8);
		return;
	}
	writeHuffmanTree(out, node->left);
	writeHuffmanTree(out, node->right);
}

void Huffman::setBit(char* target, int pos, int bit)
{
	target += pos / 8;
	pos %= 8;
	unsigned int tmp = target[0];
	target[0] = (tmp | (1 << pos)) & ~(~bit << pos);
}

void Huffman::disposeTree(Node* root)
{
	if (root == nullptr) return;
	disposeTree(root->left);
	disposeTree(root->right);
	delete root;
}

void Huffman::encode(const char* inPath, OutStream& out)
{
	if (_huffmanTree != nullptr) disposeTree(_huffmanTree);
	buildHuffmanTree(inPath);
	buildDictionary();
	writeHuffmanTree(out, _huffmanTree);
	InStream in(inPath);
	int c;
	while ((c = in.get(8)) != -1)
		out.push((char*)_dictionary[c] + 1, *_dictionary[c]);
}

void Huffman::decode(InStream& in, const char* outPath)
{
	if (_huffmanTree != nullptr) disposeTree(_huffmanTree);
	OutStream out(outPath);
	readHuffmanTree(in, _huffmanTree);
	Node* curNode = _huffmanTree;
	int c;
	int bitCount = 31;
	while (true)
	{
		while (bitCount > 0 && (c = in.get(bitCount)) == -1)
			--bitCount;
		if (bitCount == 0) break;
		for (int i = 0; i < bitCount; ++i)
		{
			curNode = (c & (1 << i) ? curNode->right : curNode->left);
			if (curNode->left == nullptr)
			{
				out.push(curNode->chr, 8);
				curNode = _huffmanTree;
			}
		}
	}
}

Huffman::Huffman()
{
	_huffmanTree = nullptr;
	_dictionary = new unsigned char* [256];
	for (int i = 0; i < 256; ++i)
		_dictionary[i] = new unsigned char[33];
}

Huffman::~Huffman()
{
	disposeTree(_huffmanTree);
	for (int i = 0; i < 256; ++i)
		delete[] _dictionary[i];
	delete[] _dictionary;

}
