/// @file	AlbumWidget.cpp
///
/// @see   AlbumWidget.h
///
/// @author	RDN © Intern :)
///
/// @date   31.05.2019   13:54
///

#include "StdAfx.h"
#include "AlbumWidget.h"
#include "ThumbnailProxyModel.h"

#include "../GaleryCore/AlbumModel.h"
#include "../GaleryCore/Picture.h"
#include "../GaleryCore/PictureModel.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QDir>

#include "tmp/ui_AlbumWidget.h"
#include "PictureDelegate.h"


namespace gallery
{
	AlbumWidget::AlbumWidget(QWidget *parent) :
		QWidget(parent),
		m_ui(new Ui::AlbumWidget),
		m_albumModel(nullptr),
		m_albumSelectionModel(nullptr),
		m_pictureModel(nullptr),
		m_pictureSelectionModel(nullptr)
	{
		m_ui->setupUi(this);
		clearUi();

		m_ui->thumbnailListView->setSpacing(5);
		m_ui->thumbnailListView->setResizeMode(QListView::Adjust);
		m_ui->thumbnailListView->setFlow(QListView::LeftToRight);
		m_ui->thumbnailListView->setWrapping(true);
		m_ui->thumbnailListView->setItemDelegate(new PictureDelegate());

		connect(m_ui->thumbnailListView, &QListView::doubleClicked,
			this, &AlbumWidget::pictureActivated);

		connect(m_ui->deleteButton, &QPushButton::clicked,
			this, &AlbumWidget::deleteAlbum);

		connect(m_ui->editButton, &QPushButton::clicked,
			this, &AlbumWidget::editAlbum);

		connect(m_ui->addPicturesButton, &QPushButton::clicked,
			this, &AlbumWidget::addPictures);
	}

	AlbumWidget::~AlbumWidget()
	{
		delete m_ui;
	}

	void AlbumWidget::setAlbumModel(AlbumModel* albumModel)
	{
		m_albumModel = albumModel;

		connect(m_albumModel, &QAbstractItemModel::dataChanged, [this](const QModelIndex &topLeft)
			{
				if (topLeft == m_albumSelectionModel->currentIndex())
					loadAlbum(topLeft);
			});
	}

	void AlbumWidget::setAlbumSelectionModel(QItemSelectionModel* albumSelectionModel)
	{
		m_albumSelectionModel = albumSelectionModel;

		connect(m_albumSelectionModel, &QItemSelectionModel::selectionChanged, [this](const QItemSelection &selected)
			{
				if (selected.isEmpty())
				{
					clearUi();
					return;
				}
				loadAlbum(selected.indexes().first());
			});
	}

	void AlbumWidget::setPictureModel(ThumbnailProxyModel* pictureModel)
	{
		m_pictureModel = pictureModel;
		m_ui->thumbnailListView->setModel(pictureModel);
	}

	void AlbumWidget::setPictureSelectionModel(QItemSelectionModel* selectionModel)
	{
		m_ui->thumbnailListView->setSelectionModel(selectionModel);
	}

	void AlbumWidget::deleteAlbum()
	{
		if (m_albumSelectionModel->selectedIndexes().isEmpty())
			return;

		int row = m_albumSelectionModel->currentIndex().row();
		m_albumModel->removeRow(row);

		// Try to select the previous album
		QModelIndex previousModelIndex = m_albumModel->index(row - 1, 0);
		if (previousModelIndex.isValid())
		{
			m_albumSelectionModel->setCurrentIndex(previousModelIndex, QItemSelectionModel::SelectCurrent);
			return;
		}

		// Try to select the next album
		QModelIndex nextModelIndex = m_albumModel->index(row, 0);
		if (nextModelIndex.isValid())
			m_albumSelectionModel->setCurrentIndex(nextModelIndex, QItemSelectionModel::SelectCurrent);
	}

	void AlbumWidget::editAlbum()
	{
		if (m_albumSelectionModel->selectedIndexes().isEmpty())
			return;

		QModelIndex currentAlbumIndex = m_albumSelectionModel->selectedIndexes().first();
		QString oldAlbumName = m_albumModel->data(currentAlbumIndex, AlbumModel::Roles::NameRole).toString();

		bool ok;
		QString newName = QInputDialog::getText(this,
			"Album's name",
			"Change Album name",
			QLineEdit::Normal,
			oldAlbumName,
			&ok);

		if (ok && !newName.isEmpty())
			m_albumModel->setData(currentAlbumIndex, newName, AlbumModel::Roles::NameRole);
	}

	void AlbumWidget::addPictures()
	{
		QStringList filenames = QFileDialog::getOpenFileNames(this,
			"Add pictures",
			QDir::homePath(),
			"Picture files (*.jpg *.png)");

		if (filenames.isEmpty())
			return;

		QModelIndex lastModelIndex;

		for (auto const & filename : filenames)
		{
			Picture picture(filename);
			lastModelIndex = m_pictureModel->pictureModel()->addPicture(picture);
		}

		m_ui->thumbnailListView->setCurrentIndex(m_pictureModel->mapFromSource(lastModelIndex));
	}

	void AlbumWidget::clearUi()
	{
		m_ui->albumName->setText("");
		m_ui->deleteButton->setVisible(false);
		m_ui->editButton->setVisible(false);
		m_ui->addPicturesButton->setVisible(false);
	}

	void AlbumWidget::loadAlbum(const QModelIndex& albumIndex)
	{
		m_pictureModel->pictureModel()->setAlbumId(m_albumModel->data(albumIndex, AlbumModel::Roles::IdRole).toInt());
		m_ui->albumName->setText(m_albumModel->data(albumIndex, Qt::DisplayRole).toString());
		m_ui->deleteButton->setVisible(true);
		m_ui->editButton->setVisible(true);
		m_ui->addPicturesButton->setVisible(true);
	}
}
