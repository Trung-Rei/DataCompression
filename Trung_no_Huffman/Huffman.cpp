#include "Huffman.h"
#include <queue>

using namespace std;

int Huffman::buildHuffmanTree(std::string inPath)
{
	InStream in(inPath);
	Node* address[256];
	for (int i = 0; i < 256; ++i) address[i] = nullptr;
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
	if (minHeap.size() == 1)
	{
		Node* tmp = minHeap.top();
		minHeap.push(new Node(0, tmp->chr, tmp->left, tmp->right));
	}
	int outputSize = 0;
	while (minHeap.size() > 1)
	{
		Node* x = minHeap.top(); minHeap.pop();
		Node* y = minHeap.top(); minHeap.pop();
		Node* node = new Node(x->freq + y->freq, 0, x, y);
		outputSize += node->freq;
		minHeap.push(node);
	}
	_huffmanTree = minHeap.top();
	return outputSize;
}

void Huffman::buildDictionary()
{
	char code[32];
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
	int state = in.get(1);
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
	if (node->right != nullptr) state = 1;
	out.push(state, 1);
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

void Huffman::encode(std::string inPath, OutStream& out)
{
	if (_huffmanTree != nullptr) disposeTree(_huffmanTree);
	int outputSize = buildHuffmanTree(inPath);
	buildDictionary();
	writeHuffmanTree(out, _huffmanTree);
	out.push(outputSize, 32);
	InStream in(inPath);
	int c;
	while ((c = in.get(8)) != -1)
		out.push((char*)_dictionary[c] + 1, *_dictionary[c]);
}

void Huffman::decode(InStream& in, std::string outPath)
{
	in.resetLim();
	if (_huffmanTree != nullptr) disposeTree(_huffmanTree);
	OutStream out(outPath);
	readHuffmanTree(in, _huffmanTree);
	in.setLim(in.get(32));
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
}

Huffman::~Huffman()
{
	disposeTree(_huffmanTree);
}
