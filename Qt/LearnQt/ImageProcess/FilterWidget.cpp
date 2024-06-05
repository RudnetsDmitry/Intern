/// @file	FilterWidget.cpp
///
/// @see	FilterWidget.h
///
/// @author	RDN © Intern :)
///
/// @date    21.05.2024 22:23
///

#include "stdafx.h"
#include "FilterWidget.h"

#include "../GaleryCore/Filter.h"

#include "./tmp/ui_FilterWidget.h"

namespace gallery
{
	FilterWidget::FilterWidget(QWidget* parent)
		: QWidget(parent)
		, m_ui(new Ui::FilterWidget)
		, m_filterType(EFilterType::Original)
		, m_defaultSourcePicture(":/lenna.jpg")
		, m_sourceThumbnail(m_defaultSourcePicture.scaled(QSize(256, 256),
			Qt::KeepAspectRatio,
			Qt::SmoothTransformation))
	{
		m_ui->setupUi(this);
		setFilterType(EFilterType::Original);
	}

	FilterWidget::~FilterWidget()
	{
		delete m_ui;
	}

	void FilterWidget::process()
	{
		m_filteredPicture = m_filter->process(m_sourcePicture);
		emit pictureProcessed(m_filteredPicture);
	}

	void FilterWidget::setSourcePicture(QImage const & sourcePicture)
	{
		m_sourcePicture = sourcePicture;
	}

	void FilterWidget::updateThumbnail(QImage const & sourceThumbnail)
	{
		m_sourceThumbnail = sourceThumbnail;
		m_filteredThumbnail = m_filter->process(m_sourceThumbnail);
		QPixmap pixmap = QPixmap::fromImage(m_filteredThumbnail);
		m_ui->thumbnailLabel->setPixmap(pixmap);
	}

	void FilterWidget::setTitle(QString const & tile)
	{
		m_ui->titleLabel->setText(tile);
	}

	void FilterWidget::setFilterType(EFilterType filterType)
	{
		if (filterType == m_filterType && m_filter)
			return;
		m_filterType = filterType;
		m_filter = CreateFilter(filterType);

		updateThumbnail(m_sourceThumbnail);
	}

	QString FilterWidget::title() const
	{
		return m_ui->titleLabel->text();
	}

	EFilterType FilterWidget::filterType() const
	{
		return m_filterType;
	}

	void FilterWidget::mousePressEvent(QMouseEvent*)
	{
		process();
	}
}
