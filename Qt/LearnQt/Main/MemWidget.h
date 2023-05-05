#pragma once
#include "SysInfoWidget.h"

class QWidget;
class QLineSeries;


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
		QLineSeries * m_series;
		long long m_pointPositionX;
	};
}