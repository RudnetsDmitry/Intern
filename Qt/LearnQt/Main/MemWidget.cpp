#include "MemWidget.h"
#include "SystemInfo.h"

#include <QtCharts/QAreaSeries>
#include <QtCharts/QLineSeries>

namespace qt_learn
{
	const int CHART_X_RANGE_COUNT = 50;
	const int CHART_X_RANGE_MAX = CHART_X_RANGE_COUNT - 1;

	namespace
	{
		void SetUpAreaSerias(QAreaSeries & areaSeries)
		{
			const int CHART_X_RANGE_MAX = 50;
			const int COLOR_DARK_BLUE = 0x209fdf;
			const int COLOR_LIGHT_BLUE = 0xbfdfef;
			const int PEN_WIDTH = 3;

			QPen pen(COLOR_DARK_BLUE);
			pen.setWidth(PEN_WIDTH);
			QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
			gradient.setColorAt(1.0, COLOR_DARK_BLUE);
			gradient.setColorAt(0.0, COLOR_LIGHT_BLUE);
			gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

			areaSeries.setPen(pen);
			areaSeries.setBrush(gradient);
		}
	}

	MemWidget::MemWidget(QWidget* parent /*= 0*/)
		: base_t(parent)
		, m_series(new QLineSeries(this))
		, m_pointPositionX(0)
	{
		auto * areaSerias = new QAreaSeries(m_series);
		SetUpAreaSerias(*areaSerias);
		QChart * chart = chartView().chart();
		chart->addSeries(areaSerias);
		chart->setTitle("Memory used");
		chart->createDefaultAxes();
		auto horizAxises = chart->axes(Qt::Horizontal);
		auto * horizAxis = (horizAxises.size() == 1) ? horizAxises.front() : nullptr;
		if (horizAxis)
		{
			horizAxis->setVisible(false);
			horizAxis->setRange(0, CHART_X_RANGE_MAX);
		}

		auto vertAxises = chart->axes(Qt::Vertical);
		auto * vertAxis = (vertAxises.size() == 1) ? vertAxises.front() : nullptr;
		if (vertAxis)
			vertAxis->setRange(0, 100);
	}

	void MemWidget::updateSeriesImpl()
	{
		double memoryUsed = SysInfo::Instance().memoryUsed();
		m_series->append(m_pointPositionX++, memoryUsed);
		if (m_series->count() > CHART_X_RANGE_COUNT)
		{
			auto chart = chartView().chart();
			chart->scroll(chart->plotArea().width() / CHART_X_RANGE_MAX, 0);
			m_series->remove(0);
		}
	}

}