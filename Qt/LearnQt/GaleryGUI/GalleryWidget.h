/// @file	GalleryWidget.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date   10.09.2019   14:21
///

#pragma once

#include <QWidget>

namespace Ui {
class GalleryWidget;
}

class QItemSelectionModel;

namespace gallery
{
	class AlbumModel;
	class PictureModel;

	class ThumbnailProxyModel;

	class GalleryWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit GalleryWidget(QWidget *parent = 0);
		~GalleryWidget();

		void setAlbumModel(AlbumModel* albumModel);
		void setAlbumSelectionModel(QItemSelectionModel* albumSelectionModel);

		void setPictureModel(ThumbnailProxyModel* pictureModel);
		void setPictureSelectionModel(QItemSelectionModel* pictureSelectionModel);

	signals:
		void pictureActivated(const QModelIndex& index);

	private:
		Ui::GalleryWidget * m_ui;
	};
}


