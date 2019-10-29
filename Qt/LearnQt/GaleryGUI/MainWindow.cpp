/// @file	MainWindow.cpp
///
/// @see   MainWindow.h
///
/// @author	RDN © Intern :)
///
/// @date   01.10.2019   12:43
///


#include "StdAfx.h"

#include "ExportFunctions.h"

#include "MainWindow.h"

#include "GalleryWidget.h"
#include "PictureWidget.h"
#include "ThumbnailProxyModel.h"

#include "../GaleryCore/AlbumModel.h"
#include "../GaleryCore/PictureModel.h"

#include "./tmp/ui_MainWindow.h"

#include <QStackedWidget>
#include <QItemSelectionModel>

namespace gallery
{
	MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent),
		m_ui(new Ui::MainWindow),
		m_galleryWidget(new GalleryWidget(this)),
		m_pictureWidget(new PictureWidget(this)),
		m_stackedWidget(new QStackedWidget(this))
	{
		m_ui->setupUi(this);

		AlbumModel* albumModel = new AlbumModel(this);
		QItemSelectionModel* albumSelectionModel = new QItemSelectionModel(albumModel, this);
		m_galleryWidget->setAlbumModel(albumModel);
		m_galleryWidget->setAlbumSelectionModel(albumSelectionModel);

		PictureModel* pictureModel = new PictureModel(*albumModel, this);
		ThumbnailProxyModel* thumbnailModel = new ThumbnailProxyModel(this);
		thumbnailModel->setSourceModel(pictureModel);

		QItemSelectionModel* pictureSelectionModel = new QItemSelectionModel(thumbnailModel, this);
		m_galleryWidget->setPictureModel(thumbnailModel);
		m_galleryWidget->setPictureSelectionModel(pictureSelectionModel);
		m_pictureWidget->setModel(thumbnailModel);
		m_pictureWidget->setSelectionModel(pictureSelectionModel);

		connect(m_galleryWidget, &GalleryWidget::pictureActivated,
			[this](QModelIndex const & index) {displayPicture(index); });

		connect(m_pictureWidget, &PictureWidget::backToGallery,
			[this]() {displayGallery(); });

		m_stackedWidget->addWidget(m_galleryWidget);
		m_stackedWidget->addWidget(m_pictureWidget);
		displayGallery();

		setCentralWidget(m_stackedWidget);
	}

	MainWindow::~MainWindow()
	{
		delete m_ui;
	}

	void MainWindow::displayGallery()
	{
		m_stackedWidget->setCurrentWidget(m_galleryWidget);
	}

	void MainWindow::displayPicture(const QModelIndex& /*index*/)
	{
		m_stackedWidget->setCurrentWidget(m_pictureWidget);
	}

	QMainWindow * MakeGalleryWindow()
	{
		return new MainWindow();
	}
}