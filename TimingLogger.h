#ifndef TICTACTOERECOGNITION_TIMING_LOGGER_H
#define TICTACTOERECOGNITION_TIMING_LOGGER_H

#include <string>
#include <map>

class TimingLogger
{
public:
	static TimingLogger* getInstance()
	{
		if (!sInstance)
			sInstance = new TimingLogger;
		return sInstance;
	}

	void onTaskStart(std::string task_name);

	void onTaskEnd(std::string task_name);

	void enableLogger();

	void dissableLogger();

private:
	static TimingLogger* sInstance;
	bool mIsLoggerEnabled;
	std::map<std::string, long> mTrackingTasks;
};

#endif TICTACTOERECOGNITION_TIMING_LOGGER_H