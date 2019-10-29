/// @file	AlbumWidget.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date   31.05.2019   13:51
///

#pragma once
//#include "GaleryGUIExp.h"

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class AlbumWidget;
}

class QItemSelectionModel;

namespace gallery
{
	class AlbumModel;
	class PictureModel;
	class ThumbnailProxyModel;

	class /*GALERYGUI_EXT_CLASS*/ AlbumWidget : public QWidget
	{
		Q_OBJECT
	public:
		explicit AlbumWidget(QWidget *parent = 0);
		~AlbumWidget();

		void setAlbumModel(AlbumModel* albumModel);
		void setAlbumSelectionModel(QItemSelectionModel* albumSelectionModel);
		void setPictureModel(ThumbnailProxyModel* pictureModel);
		void setPictureSelectionModel(QItemSelectionModel* selectionModel);

	signals:
		void pictureActivated(const QModelIndex& index);

	private:
		void deleteAlbum();
		void editAlbum();
		void addPictures();

	private:
		void clearUi();
		void loadAlbum(const QModelIndex& albumIndex);

	private:
		Ui::AlbumWidget* m_ui;
		AlbumModel* m_albumModel;
		QItemSelectionModel* m_albumSelectionModel;

		ThumbnailProxyModel* m_pictureModel;
		QItemSelectionModel* m_pictureSelectionModel;
	};
}
