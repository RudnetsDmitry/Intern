/// @file	PictureWidget.cpp
///
/// @see   PictureWidget.h
///
/// @author	RDN © Intern :)
///
/// @date   06.09.2019   18:16
///


#include "StdAfx.h"
#include "PictureWidget.h"
#include "ThumbnailProxyModel.h"

#include "..\GaleryCore\PictureModel.h"

#include ".\tmp\ui_PictureWidget.h"

#include <QItemSelection>
#include <QItemSelectionModel>

namespace gallery
{
	PictureWidget::PictureWidget(QWidget *parent) :
		QWidget(parent),
		m_ui(new Ui::PictureWidget),
		m_model(nullptr),
		m_selectionModel(nullptr)
	{
		m_ui->setupUi(this);
		m_ui->pictureLabel->setMinimumSize(1, 1);

		connect(m_ui->backButton, &QPushButton::clicked,
			this, &PictureWidget::backToGallery);

		connect(m_ui->deleteButton, &QPushButton::clicked,
			this, &PictureWidget::deletePicture);

		connect(m_ui->previousButton, &QPushButton::clicked, [this]()
			{
				QModelIndex currentModelIndex = m_selectionModel->currentIndex();
				QModelIndex previousModelIndex = m_selectionModel->model()->index(currentModelIndex.row() - 1, 0);
				m_selectionModel->setCurrentIndex(previousModelIndex, QItemSelectionModel::SelectCurrent);
			});

		connect(m_ui->nextButton, &QPushButton::clicked, [this]()
			{
				QModelIndex currentModelIndex = m_selectionModel->currentIndex();
				QModelIndex nextModelIndex = m_selectionModel->model()->index(currentModelIndex.row() + 1, 0);
				m_selectionModel->setCurrentIndex(nextModelIndex, QItemSelectionModel::SelectCurrent);
			});
	}

	PictureWidget::~PictureWidget()
	{
		delete m_ui;
	}

	void PictureWidget::setModel(ThumbnailProxyModel* model)
	{
		m_model = model;
	}

	void PictureWidget::setSelectionModel(QItemSelectionModel* selectionModel)
	{
		m_selectionModel = selectionModel;
		if (m_selectionModel)
			connect(m_selectionModel, &QItemSelectionModel::selectionChanged, this, &PictureWidget::loadPicture);
	}

	void PictureWidget::resizeEvent(QResizeEvent* event)
	{
		QWidget::resizeEvent(event);
		updatePicturePixmap();
	}

	void PictureWidget::deletePicture()
	{
		// Remove the current picture
		int row = m_selectionModel->currentIndex().row();
		m_model->removeRow(m_selectionModel->currentIndex().row());

		// Try to select the previous picture
		QModelIndex previousModelIndex = m_model->index(row - 1, 0);
		if (previousModelIndex.isValid())
		{
			m_selectionModel->setCurrentIndex(previousModelIndex, QItemSelectionModel::SelectCurrent);
			return;
		}

		// Try to select the next picture
		QModelIndex nextModelIndex = m_model->index(row, 0);
		if (nextModelIndex.isValid())
		{
			m_selectionModel->setCurrentIndex(nextModelIndex, QItemSelectionModel::SelectCurrent);
			return;
		}

		emit backToGallery();
	}

	void PictureWidget::loadPicture(const QItemSelection& selected)
	{
		if (selected.indexes().isEmpty())
		{
			m_ui->nameLabel->setText("");
			m_ui->pictureLabel->setPixmap(QPixmap());
			m_ui->deleteButton->setEnabled(false);
			return;
		}

		QModelIndex current = selected.indexes().at(0);
		m_pixmap = QPixmap(m_model->data(current, PictureModel::Roles::FilePathRole).toString());

		m_ui->nameLabel->setText(m_model->data(current, Qt::DisplayRole).toString());
		updatePicturePixmap();

		m_ui->previousButton->setEnabled(current.row() > 0);
		m_ui->nextButton->setEnabled(current.row() < (m_model->rowCount() - 1));
		m_ui->deleteButton->setEnabled(true);
	}

	void PictureWidget::updatePicturePixmap()
	{
		if (!m_pixmap.isNull())
			m_ui->pictureLabel->setPixmap(m_pixmap.scaled(m_ui->pictureLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}
}