/// @file	MainWindow.cpp
///
/// @see	MainWindow.h
///
/// @author	RDN © Intern :)
///
/// @date    22.05.2024 22:16
///

#include "stdafx.h"
#include "MainWindow.h"
#include "FilterWidget.h"

#include "./tmp/ui_MainWindow.h"

#include <QFileDialog>
#include <QPixmap>
#include <QDir>

namespace gallery
{
	MainWindow::MainWindow(QWidget* parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow),
		m_currentFilter(nullptr)
	{
		ui->setupUi(this);
		ui->actionSaveAs->setEnabled(false);
		ui->pictureLabel->setMinimumSize(1, 1);

		connect(ui->actionOpenPicture, &QAction::triggered,
			this, &MainWindow::loadPicture);
		connect(ui->actionSaveAs, &QAction::triggered,
			this, &MainWindow::saveAsPicture);
		connect(ui->actionExit, &QAction::triggered,
			this, &QMainWindow::close);
		initFilters();
	}

	MainWindow::~MainWindow()
	{
		delete ui;
	}

	void MainWindow::initFilters()
	{
		auto addFilterWF = [this](QString const& name, EFilterType filterType,
			QString const & title)
		{
			auto* newWidget = new FilterWidget(ui->centralWidget);
			newWidget->setObjectName(name);
			ui->filtersLayout->addWidget(newWidget);
			newWidget->setFilterType(filterType);
			newWidget->setTitle(title);

			m_filters.append(newWidget);
		};

		addFilterWF("filterWidgetOriginal", EFilterType::Original, "Original");
		addFilterWF("filterWidgetGrayscale", EFilterType::Grayscale, "Grayscale");
		addFilterWF("filterWidgetBlur", EFilterType::Blur, "Blur");

		for (auto * filter : m_filters)
			connect(filter, &FilterWidget::pictureProcessed,
				this, &MainWindow::displayPicture);
		
		m_currentFilter = m_filters[0];
	}

	void MainWindow::loadPicture()
	{
		QString filename = QFileDialog::getOpenFileName(this,
			"Open Picture",
			QDir::homePath(),
			tr("Images (*.png *.jpg)"));
		if (filename.isEmpty())
			return;
		
		ui->actionSaveAs->setEnabled(true);
		QImage sourcePicture = QImage(filename);
		QImage sourceThumbnail = sourcePicture.scaled(QSize(256, 256),
			Qt::KeepAspectRatio,
			Qt::SmoothTransformation);

		for (auto* filter : m_filters)
		{
			filter->setSourcePicture(sourcePicture);
			filter->updateThumbnail(sourceThumbnail);
		}
		
		m_currentFilter->process();
	}

	void MainWindow::resizeEvent(QResizeEvent* /*event*/)
	{
		updatePicturePixmap();
	}

	void MainWindow::displayPicture(QImage const& picture)
	{
		m_filteredPicture = picture;
		m_currentPixmap = QPixmap::fromImage(picture);
		updatePicturePixmap();
	}

	void MainWindow::saveAsPicture()
	{
		QString filename = QFileDialog::getSaveFileName(this,
			"Save Picture",
			QDir::homePath(),
			tr("Images (*.png *.jpg)"));
		if (!filename.isEmpty())
			m_filteredPicture.save(filename);
	}

	void MainWindow::updatePicturePixmap()
	{
		if (m_currentPixmap.isNull())
			return;

		ui->pictureLabel->setPixmap(m_currentPixmap);
		/*ui->pictureLabel->setPixmap(
			m_currentPixmap.scaled(ui->pictureLabel->size(),
				Qt::KeepAspectRatio,
				Qt::SmoothTransformation));*/
	}
}