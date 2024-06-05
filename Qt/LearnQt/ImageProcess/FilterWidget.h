/// @file	FilterWidget.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    21.05.2024 22:13
///

#pragma once


#include <QWidget>
#include <QImage>
#include <memory>

#include "ImageFilters.h"

namespace Ui {
	class FilterWidget;
}

namespace gallery
{
	struct IFilter;

	class FilterWidget : public QWidget
	{
		Q_OBJECT
	public:

		explicit FilterWidget(QWidget * parent = nullptr);
		~FilterWidget() override;

		void process();

		void setSourcePicture(QImage const & sourcePicture);
		void updateThumbnail(QImage const & sourceThumbnail);

		QString title() const;
		EFilterType filterType() const;

	public slots:
		void setTitle(QString const & tile);
		void setFilterType(EFilterType filterType);

	signals:
		void pictureProcessed(const QImage& picture);

	protected:
		void mousePressEvent(QMouseEvent*) override;

	private:
		Ui::FilterWidget* m_ui;
		std::unique_ptr<IFilter> m_filter;
		EFilterType m_filterType;

		QImage m_defaultSourcePicture;
		QImage m_sourcePicture;
		QImage m_sourceThumbnail;

		QImage m_filteredPicture;
		QImage m_filteredThumbnail;
	};
}

