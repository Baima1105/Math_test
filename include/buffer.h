#ifndef BUFFER_H
#define BUFFER_H

#include<cstring>
#include<iostream>
#include<unistd.h>
#include<sys/uio.h>
#include<vector>
#include<atomic>
#include<assert.h>

class Buffer {
public:
	//vector<char>初始化
	Buffer(int initBuffSize = 1024);
	~Buffer() = default;

	//可写入的数量,writePos - size()
	size_t WritableBytes() const;
	//可读出的数量,readPos - writePos
	size_t ReadableBytes() const;
	//可预留的空间, 0 - readPos_
	size_t PrependableBytes() const;

	//返回读下标位置的指针
	const char* Peek() const;
	//确保该长度可以写入
	void EnsureWriteable(size_t len);

	//移动写下标
	void HasWritten(size_t len);
	//移动读下标
	void Retrieve(size_t len);
	//读取到end位置
	void RetrieveUntil(const char* end);

	//读写下标归零
	void RetrieveAll();
	//取出剩余的str
	std::string RetrieveAllToStr();

	//返回写下标位置指针
	const char* BeginWriteConst()const;
	char* BeginWrite();
	
	//添加str到缓冲区
	void Append(const std::string& str);
	void Append(const char* str, size_t len);
	void Append(const void* data, size_t len);
	void Append(const Buffer& buff);

	//将fd读入缓冲区
	size_t ReadFd(int fd, int* Errno);
	//将可读区写入fd中
	size_t WriteFd(int fd, int* Errno);

private:
	//缓冲区起始位置指针
	char* Beginptr_();
	//缓冲区起始位置指针
	const char* BeginPtr_()const;
	//(预留空间+可写空间)空间不够就扩容，够就将可读区移到开头
	void MakeSpace_(size_t len);

	std::vector<char> buffer_;
	std::atomic<std::size_t> readPos_;
	std::atomic<std::size_t> writePos_;
};

#endif //BUFFER_H
