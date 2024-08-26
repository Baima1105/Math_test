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
	//单例饿汉，创建唯一实例化对象
	static Log* Instance();
	//唤醒日志线程，读出阻塞队列内容，异步写入日志文件中
	static void FlushLogThread();

	void write(int level, const char* format, ...);
	//唤醒阻塞队列的消费者
	void flush();

	int GetLevel();
	void SetLevel(int level);
	bool IsOpen() { 
		return isOpen_;
	}

private:
	Log();
	//添加级别
	void AppendLogLevelTitle_(int level);
	virtual ~Log();
	//读出阻塞队列，异步写入日志
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
	bool isAsync_;	//是否为异步处理

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

// 四个宏定义，主要用于不同类型的日志输出，也是外部使用日志的接口
// ...表示可变参数，__VA_ARGS__就是将...的值复制到这里
// 前面加上##的作用是：当可变参数的个数为0时，这里的##可以把把前面多余的","去掉,否则会编译出错。
#define LOG_DEBUG(format, ...) do {LOG_BASE(0, format, ##__VA_ARGS__)} while(0);    
#define LOG_INFO(format, ...) do {LOG_BASE(1, format, ##__VA_ARGS__)} while(0);
#define LOG_WARN(format, ...) do {LOG_BASE(2, format, ##__VA_ARGS__)} while(0);
#define LOG_ERROR(format, ...) do {LOG_BASE(3, format, ##__VA_ARGS__)} while(0);

#endif // LOG_H

