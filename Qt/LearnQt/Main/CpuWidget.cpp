#include "CpuWidget.h"
#include "SystemInfo.h"

#include <QtCharts/QpieSeries>

namespace qt_learn
{
	CpuWidget::CpuWidget(QWidget* parent /*= 0*/)
		: base_t(parent)
		, m_series(new QtCharts::QPieSeries(this))
	{
		m_series->setHoleSize(0.35);
		m_series->append("Load", 30.0);
		m_series->append("Free", 70.0);
		
		auto* chart = chartView().chart();
		chart->addSeries(m_series);
		chart->setTitle("CPU average load");
	}

	void CpuWidget::updateSeriesImpl()
	{
		double cpuLoadAverage = SysInfo::Instance().cpuLoadAverage();
		m_series->clear();
		m_series->append("Load", cpuLoadAverage);
		m_series->append("Free", 100.0 - cpuLoadAverage);
	}

}