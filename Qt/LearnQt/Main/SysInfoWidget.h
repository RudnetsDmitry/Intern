#pragma once
#include <QtWidgets/QWidget>
#include <QtCharts/QChartView>
#include <QTimer>

namespace qt_learn 
{
	class SysInfoWidget : public QWidget
	{
		using baset_t = QWidget;
		using this_t = SysInfoWidget;
		Q_OBJECT
	public:
		explicit SysInfoWidget(QWidget *parent = 0,
			int startDelayMs = 500,
			int updateSeriesDelayMs = 500);
	protected:
		QChartView& chartView();
	protected slots:
		void updateSeries();
	private:
		virtual void updateSeriesImpl() = 0;
	private:
		QTimer m_refreshTimer;
		QChartView m_chartView;
	};
}
