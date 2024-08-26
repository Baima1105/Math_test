#ifndef LOG_H
#define LOG_H

#include <mutex>
#include <string>
#include <thread>
#include <sys/time.h>
#include <string.h>
#include <stdarg.h>           // vastart va_end
#include <assert.h>
#include <sys/stat.h>         // mkdir
#include "blockqueue.h"
#include "buffer.h"

class Log {
public:
	void init(int level, const char* path = "./log", const char* suffix = "./log",
		int maxQueueCapacity = 1024);
	//��������������Ψһʵ��������
	static Log* Instance();
	//������־�̣߳����������������ݣ��첽д����־�ļ���
	static void FlushLogThread();

	void write(int level, const char* format, ...);
	//�����������е�������
	void flush();

	int GetLevel();
	void SetLevel(int level);
	bool IsOpen() { 
		return isOpen_;
	}

private:
	Log();
	//��Ӽ���
	void AppendLogLevelTitle_(int level);
	virtual ~Log();
	//�����������У��첽д����־
	void AsyncWrite_();
private:
	static const int LOG_PATH_LEN = 256;
	static const int LOG_NAME_LEN = 256;
	static const int MAX_LINES = 1000;

	const char* path_;
	const char* suffix_;

	int MAX_LINES_;

	int lineCount_;
	int toDay_;

	bool isOpen_;

	Buffer buff_;
	int level_;
	bool isAsync_;	//�Ƿ�Ϊ�첽����

	FILE* fp_;
	std::unique_ptr<BlockQueue<std::string>> deque_;
	std::unique_ptr<std::thread> writeThread_;
	std::mutex mtx_;

};

#define LOG_BASE(level,format,...)
do {
	\
		Log* log = Log::Instance(); \
		if (log->IsOpen() && log->GetLevel() <= level) {
			\
				log->write(level, format, ##__VA_ARGS__); \
				log->flush(); \
		}\
} while (0);

// �ĸ��궨�壬��Ҫ���ڲ�ͬ���͵���־�����Ҳ���ⲿʹ����־�Ľӿ�
// ...��ʾ�ɱ������__VA_ARGS__���ǽ�...��ֵ���Ƶ�����
// ǰ�����##�������ǣ����ɱ�����ĸ���Ϊ0ʱ�������##���԰Ѱ�ǰ������","ȥ��,�����������
#define LOG_DEBUG(format, ...) do {LOG_BASE(0, format, ##__VA_ARGS__)} while(0);    
#define LOG_INFO(format, ...) do {LOG_BASE(1, format, ##__VA_ARGS__)} while(0);
#define LOG_WARN(format, ...) do {LOG_BASE(2, format, ##__VA_ARGS__)} while(0);
#define LOG_ERROR(format, ...) do {LOG_BASE(3, format, ##__VA_ARGS__)} while(0);

#endif // LOG_H

