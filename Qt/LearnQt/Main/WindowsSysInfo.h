#pragma once

#include "SystemInfo.h"
#include <vector>

namespace qt_learn
{
	class WindowsSysInfo : public SysInfo
	{
	public:
		virtual void init() override;
		virtual double cpuLoadAverage() override;
		virtual double memoryUsed() override;
	private:
		std::vector<long long> m_cpuLoadLastValues;
	};

}