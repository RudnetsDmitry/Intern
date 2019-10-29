/// @file	GalleryWidget.cpp
///
/// @see   GalleryWidget.h
///
/// @author	RDN © Intern :)
///
/// @date   10.09.2019   14:49
///


#include "StdAfx.h"
#include "GalleryWidget.h"

#include "./tmp/ui_GalleryWidget.h"

#include <QItemSelectionModel>

namespace gallery
{
	GalleryWidget::GalleryWidget(QWidget *parent) :
		QWidget(parent),
		m_ui(new Ui::GalleryWidget)
	{
		m_ui->setupUi(this);
		m_ui->albumListWidget->setMaximumWidth(250);
		connect(m_ui->albumWidget, &AlbumWidget::pictureActivated, this, &GalleryWidget::pictureActivated);
	}

	GalleryWidget::~GalleryWidget()
	{
		delete m_ui;
	}

	void GalleryWidget::setAlbumModel(AlbumModel* albumModel)
	{
		m_ui->albumListWidget->setModel(albumModel);
		m_ui->albumWidget->setAlbumModel(albumModel);
	}

	void GalleryWidget::setAlbumSelectionModel(QItemSelectionModel* albumSelectionModel)
	{
		m_ui->albumListWidget->setSelectionModel(albumSelectionModel);
		m_ui->albumWidget->setAlbumSelectionModel(albumSelectionModel);
	}

	void GalleryWidget::setPictureModel(ThumbnailProxyModel* pictureModel)
	{
		m_ui->albumWidget->setPictureModel(pictureModel);
	}

	void GalleryWidget::setPictureSelectionModel(QItemSelectionModel* pictureSelectionModel)
	{
		m_ui->albumWidget->setPictureSelectionModel(pictureSelectionModel);
	}

}
