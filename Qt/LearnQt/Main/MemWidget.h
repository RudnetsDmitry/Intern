#pragma once
#include "SysInfoWidget.h"

class QWidget;
namespace QtCharts
{
	class QLineSeries;
}

namespace qt_learn
{
	class MemWidget : public SysInfoWidget
	{
		using base_t = SysInfoWidget;
	public:
		explicit MemWidget(QWidget* parent = 0);
	private:
		virtual void updateSeriesImpl() override;
	private:
		QtCharts::QLineSeries * m_series;
		long long m_pointPositionX;
	};
}