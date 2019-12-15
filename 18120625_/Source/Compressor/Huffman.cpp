#include "Huffman.h"
#include <queue>

using namespace std;

//xây dựng cây Huffman từ file inPath, outputSize là số lượng bit dữ liệu sau khi nén
//trả về gốc của cây đã xây
Huffman::Node* Huffman::buildHuffmanTree(std::string inPath, long long& outputSize)
{
	InStream in(inPath);
	Node* address[256] = { nullptr };
	int c;
	//đếm số lượng của các kí tự
	while ((c = in.get(8)) != -1)
	{
		if (address[c] == nullptr)
			address[c] = new Node(0, c, nullptr, nullptr);
		++address[c]->freq;
	}
	//xây dựng cây Huffman
	priority_queue<Node*, vector<Node*>, Compare> minHeap;
	for (int i = 0; i < 256; ++i)
		if (address[i] != nullptr) minHeap.push(address[i]);
	if (minHeap.size() == 1)
	{
		Node* tmp = minHeap.top();
		minHeap.push(new Node(0, tmp->chr, tmp->left, tmp->right));
	}
	outputSize = 0;
	while (minHeap.size() > 1)
	{
		Node* x = minHeap.top(); minHeap.pop();
		Node* y = minHeap.top(); minHeap.pop();
		Node* node = new Node(x->freq + y->freq, 0, x, y);
		outputSize += node->freq;
		minHeap.push(node);
	}
	//gốc của cây chính là nút duy nhất còn lại của min heap
	if (minHeap.empty()) return nullptr;
	return minHeap.top();
}

//xây dựng bộ từ điển từ cây Huffman
void Huffman::buildDictionary(unsigned char** dictionary, Node* huffmanTree)
{
	char code[32];
	_buildDictionary(dictionary, huffmanTree, 0, code);
}

//hàm đệ quy duyệt cây để xây dựng từ điển
void Huffman::_buildDictionary(unsigned char** dictionary, Node* node, int depth, char* code)
{
	//khi gặp nút lá thì ghi nhận dãy bit vừa trace là từ mã của kí tự tại nút lá
	if (node->left == nullptr)
	{
		dictionary[node->chr][0] = depth;
		int i;
		for (i = 0; i < depth / 8; ++i) dictionary[node->chr][i + 1] = code[i];
		if (depth % 8 != 0)  dictionary[node->chr][i + 1] = code[i];
		return;
	}
	//traverse xuống 2 con
	setBit(code, depth, 0);
	_buildDictionary(dictionary, node->left, depth + 1, code);
	setBit(code, depth, 1);
	_buildDictionary(dictionary, node->right, depth + 1, code);
}

//Đọc bằng đệ quy và xây dựng cây Huffman lưu trong file nén
//trả về gốc của cây qua node
void Huffman::readHuffmanTree(InStream& in, Node*& node)
{
	int state = in.get(1);
	node = new Node(0, 0, nullptr, nullptr);
	//nếu state bằng 0 thì đang ở 1 nút cha bất kì và tiếp tục đọc 2 nút con
	//bằng 1 thì đã tới nút lá và đọc kí tự được lưu
	if (state == 0)
	{
		node->chr = in.get(8);
		return;
	}
	readHuffmanTree(in, node->left);
	readHuffmanTree(in, node->right);
}

//viết cây Huffman vào file bằng đệ quy,
//cách ghi là ngược lại của cách đọc
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

//set bit tại ví trí pos của vùng nhớ target giá trị bit(0/1)
void Huffman::setBit(char* target, int pos, int bit)
{
	target += pos / 8;
	pos %= 8;
	unsigned int tmp = target[0];
	target[0] = (tmp | (1 << pos)) & ~(~bit << pos);
}

//đệ quy xóa cây huffman
void Huffman::disposeTree(Node* root)
{
	if (root == nullptr) return;
	disposeTree(root->left);
	disposeTree(root->right);
	delete root;
}

//nén file có đường dẫn inPath và ghi dữ liệu đã nén ra OutStream
void Huffman::encode(std::string inPath, OutStream& out)
{
	long long outputSize;
	Node* huffmanTree = buildHuffmanTree(inPath, outputSize);
	out.push((char*)&outputSize, 64);
	if (outputSize == 0) return;
	unsigned char** dictionary = new unsigned char* [256];
	for (int i = 0; i < 256; ++i)
		dictionary[i] = new unsigned char[33];
	buildDictionary(dictionary, huffmanTree);
	writeHuffmanTree(out, huffmanTree);
	InStream in(inPath);
	int c;
	//đọc kí tự của file, dịch qua từ điển và ghi vào out stream
	while ((c = in.get(8)) != -1)
		out.push((char*)dictionary[c] + 1, *dictionary[c]);
	disposeTree(huffmanTree);
	for (int i = 0; i < 256; ++i) delete[] dictionary[i];
	delete[] dictionary;
}

//giải nén đoạn dữ liệu đã nén từ InStream ra file có đường dẫn outPath
void Huffman::decode(InStream& in, std::string outPath)
{
	in.resetLim();
	OutStream out(outPath);
	long long lim;
	in.get((char*)&lim, 64);
	if (lim == 0) return;
	Node* huffmanTree;
	readHuffmanTree(in, huffmanTree);
	in.setLim(lim);
	Node* curNode = huffmanTree;
	int c;
	int bitCount = 31;
	//đọc từng bit trong file nén để tìm kí tự đã được mã hóa
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
				curNode = huffmanTree;
			}
		}
	}
	disposeTree(huffmanTree);
}
