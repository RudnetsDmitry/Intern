#include "SysInfoWidget.h"
#include <QtWidgets/QBoxLayout>

namespace qt_learn
{
	SysInfoWidget::SysInfoWidget(QWidget *parent /*= 0*/,
		int startDelayMs /*= 500*/, int updateSeriesDelayMs /*= 500*/)
		: baset_t(parent)
		, m_chartView(this)
	{
		m_refreshTimer.setInterval(updateSeriesDelayMs);
		auto resConnect = connect(&m_refreshTimer, &QTimer::timeout, this,
			&this_t::updateSeries);
		assert(resConnect);

		m_chartView.setRenderHint(QPainter::Antialiasing);
		m_chartView.chart()->legend()->setVisible(false);

		QVBoxLayout * layout = new QVBoxLayout(this);
		layout->addWidget(&m_chartView);
		setLayout(layout);

		QTimer::singleShot(startDelayMs, [this] {m_refreshTimer.start(); });
	}

	QtCharts::QChartView & SysInfoWidget::chartView()
	{
		return m_chartView;
	}

	void SysInfoWidget::updateSeries()
	{
		updateSeriesImpl();
	}

}
