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
	//vector<char>��ʼ��
	Buffer(int initBuffSize = 1024);
	~Buffer() = default;

	//��д�������,writePos - size()
	size_t WritableBytes() const;
	//�ɶ���������,readPos - writePos
	size_t ReadableBytes() const;
	//��Ԥ���Ŀռ�, 0 - readPos_
	size_t PrependableBytes() const;

	//���ض��±�λ�õ�ָ��
	const char* Peek() const;
	//ȷ���ó��ȿ���д��
	void EnsureWriteable(size_t len);

	//�ƶ�д�±�
	void HasWritten(size_t len);
	//�ƶ����±�
	void Retrieve(size_t len);
	//��ȡ��endλ��
	void RetrieveUntil(const char* end);

	//��д�±����
	void RetrieveAll();
	//ȡ��ʣ���str
	std::string RetrieveAllToStr();

	//����д�±�λ��ָ��
	const char* BeginWriteConst()const;
	char* BeginWrite();
	
	//���str��������
	void Append(const std::string& str);
	void Append(const char* str, size_t len);
	void Append(const void* data, size_t len);
	void Append(const Buffer& buff);

	//��fd���뻺����
	size_t ReadFd(int fd, int* Errno);
	//���ɶ���д��fd��
	size_t WriteFd(int fd, int* Errno);

private:
	//��������ʼλ��ָ��
	char* Beginptr_();
	//��������ʼλ��ָ��
	const char* BeginPtr_()const;
	//(Ԥ���ռ�+��д�ռ�)�ռ䲻�������ݣ����ͽ��ɶ����Ƶ���ͷ
	void MakeSpace_(size_t len);

	std::vector<char> buffer_;
	std::atomic<std::size_t> readPos_;
	std::atomic<std::size_t> writePos_;
};

#endif //BUFFER_H
