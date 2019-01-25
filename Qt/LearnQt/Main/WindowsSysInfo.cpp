#include "WindowsSysInfo.h"
#include <windows.h>
#include "qglobal.h"

namespace qt_learn
{
	namespace
	{
		long long FileTime2LongLong(FILETIME const & filetime)
		{
			ULARGE_INTEGER largeInteger;
			largeInteger.LowPart = filetime.dwLowDateTime;
			largeInteger.HighPart = filetime.dwHighDateTime;
			return largeInteger.QuadPart;
		}

		std::vector<long long> CpuRawData()
		{
			FILETIME idleTime;
			FILETIME kernelTime;
			FILETIME userTime;
			GetSystemTimes(&idleTime, &kernelTime, &userTime);

			std::vector<long long> rawData;
			
			rawData.emplace_back(FileTime2LongLong(idleTime));
			rawData.emplace_back(FileTime2LongLong(kernelTime));
			rawData.emplace_back(FileTime2LongLong(userTime));
			
			return rawData;
		}
	}

	void WindowsSysInfo::init()
	{
		m_cpuLoadLastValues = CpuRawData();
	}

	double WindowsSysInfo::memoryUsed()
	{
		MEMORYSTATUSEX memoryStatus;
		memset(&memoryStatus, 0, sizeof(memoryStatus));
		memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);

		GlobalMemoryStatusEx(&memoryStatus);

		long long memoryPhysicalUsed =
			memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys;
		return (double)memoryPhysicalUsed /
			(double)memoryStatus.ullTotalPhys * 100.0;
	}

	double WindowsSysInfo::cpuLoadAverage()
	{
		auto firstSample = m_cpuLoadLastValues;
		auto secondSample = CpuRawData();
		m_cpuLoadLastValues = secondSample;

		auto currentIdle = secondSample[0] - firstSample[0];
		auto currentKernel = secondSample[1] - firstSample[1];
		auto currentUser = secondSample[2] - firstSample[2];
		auto currentSystem = currentKernel + currentUser;
		double percent = (currentSystem - currentIdle) * 100.0 /
			currentSystem;
		return qBound(0.0, percent, 100.0);
	}
}