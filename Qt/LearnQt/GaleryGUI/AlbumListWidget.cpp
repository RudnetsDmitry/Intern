/// @file	AlbumListWidget.cpp
///
/// @see   AlbumListWidget.h
///
/// @author	RDN © Intern :)
///
/// @date   22.05.2019   19:30
///


#include "stdafx.h"
#include "AlbumListWidget.h"

#include ".\tmp\ui_AlbumListWidget.h"

#include "..\GaleryCore\AlbumModel.h"
#include "..\GaleryCore\Album.h"

#include <QLineEdit>
#include <QInputDialog>


namespace gallery
{

	AlbumListWidget::AlbumListWidget(QWidget * parent /*= nullptr*/)
		: base_t(parent)
		, m_ui(new ::Ui::AlbumListWidget())
		, m_AlbumModel(nullptr)
	{
		m_ui->setupUi(this);
		connect(m_ui->createAlbumButton, &QPushButton::clicked, this, [this]() { createAlbum(); });
	}

	AlbumListWidget::~AlbumListWidget()
	{
		delete m_ui;
	}

	void AlbumListWidget::setModel(AlbumModel * model)
	{
		m_AlbumModel = model;
		m_ui->albumList->setModel(model);
	}

	void AlbumListWidget::setSelectionModel(QItemSelectionModel* selectionModel)
	{
		m_ui->albumList->setSelectionModel(selectionModel);
	}

	void AlbumListWidget::createAlbum()
	{
		if (!m_AlbumModel)
			return;

		bool ok;
		QString albumName = QInputDialog::getText(this, "Create a new Album", "Choose a name",
			QLineEdit::Normal, "New album", &ok);

		if (ok && !albumName.isEmpty())
		{
			QModelIndex createdIndex = m_AlbumModel->addAlbum(Album(albumName));
			m_ui->albumList->setCurrentIndex(createdIndex);
		}
	}

}
