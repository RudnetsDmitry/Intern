﻿#pragma once
#include "SysInfoWidget.h"

class QWidget;
class QPieSeries;

namespace qt_learn
{
	class CpuWidget : public SysInfoWidget
	{
		using base_t = SysInfoWidget;
	public:
		explicit CpuWidget(QWidget* parent = 0);
	private:
		virtual void updateSeriesImpl() override;
	private:
		QPieSeries * m_series;
	};
}