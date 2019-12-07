#pragma once
#include <chrono>
#include <iostream>

using namespace std::chrono;

//program frame cho các chương trình nén
class CompressorProgram
{
protected:
	virtual void startMessage() = 0;
	int input();
	
	//thực thi yêu cầu nén file hoặc thư mục từ người dùng,
	//trả về thời điểm bắt đầu và kết thúc của hàm nén để tính thời gian nén
	virtual void compress(high_resolution_clock::time_point& start, high_resolution_clock::time_point& stop) = 0;
	
	//thực thi yêu cầu giải nén file hoặc thư mục từ người dùng,
	//trả về thời điểm bắt đầu và kết thúc của hàm giải nén để tính thời gian giải nén
	virtual void decompress(high_resolution_clock::time_point& start, high_resolution_clock::time_point& stop) = 0;
	bool askToContinue();

public:

	//phương thức khuôn thực của chương trình nén
	void run();
};

