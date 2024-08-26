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
	static Log* Instance();
	static void FlushLogThread();

	void write(int level, const char* format, ...);
	void flush();

	int GetLevel();
	void SetLevel(int level);
	bool IsOpen() { return isOpen_; }

private:
	Log();
	void AppendLogLevelTitle_(int level);
	virtual ~Log();
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
	bool isAsync_;

	FILE* fp_;
	std::unique_ptr<BlockQueue<std::string>> deque_;
	std::unique_ptr<std::thread> writeThread_;
	std::mutex mtx_;;

};

#define LOG_BASE(level,format,...)

#endif // LOG_H

