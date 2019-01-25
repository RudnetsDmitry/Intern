#include "SystemInfo.h"
#include "WindowsSysInfo.h"

namespace qt_learn
{
	qt_learn::SysInfo& SysInfo::Instance()
	{
		static WindowsSysInfo sysInfo;
		return sysInfo;
	}

}