#include "TimingLogger.h"
#include <iostream>
#include <ctime>

TimingLogger* TimingLogger::sInstance = nullptr;

void TimingLogger::onTaskStart(std::string task_name)
{
	mTrackingTasks.insert_or_assign(task_name, clock());
}

void TimingLogger::onTaskEnd(std::string task_name)
{
	auto start_time = mTrackingTasks.at(task_name);
	if (start_time == 0)
	{
		std::cout << "Task " << task_name << " is not tracking";
		return;
	}

	if (!mIsLoggerEnabled)
	{
		return;
	}

	double elapsed_secs = double(clock() - start_time) / CLOCKS_PER_SEC;
	char* severity;
	if (elapsed_secs < 0.001)
	{
		severity = "+   ";
	}
	else if (elapsed_secs < 0.01)
	{
		severity = "++  ";
	}
	else if (elapsed_secs < 0.1)
	{
		severity = "+++ ";
	}
	else
	{
		severity = "++++";
	}
	std::cout << "TimingLogger [" << severity << "]:" << task_name
		<< " took " << elapsed_secs << " sec" << std::endl;
}

void TimingLogger::enableLogger()
{
	mIsLoggerEnabled = true;
}

void TimingLogger::dissableLogger()
{
	mIsLoggerEnabled = false;
}
