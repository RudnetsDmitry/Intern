#pragma once

namespace qt_learn
{
	class SysInfo
	{
	public:
		static SysInfo& Instance();
	public:
		SysInfo(SysInfo const&) = delete;
		SysInfo & operator=(SysInfo const &) = delete;
		virtual ~SysInfo() = default;
		
		virtual void init() = 0;
		virtual double cpuLoadAverage() = 0;
		virtual double memoryUsed() = 0;

	protected:
		explicit SysInfo() = default;
	};

}
