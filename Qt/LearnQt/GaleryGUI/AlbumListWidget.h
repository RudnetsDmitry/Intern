/// @file	AlbumListWidget.h
///
/// @brief widget for albums...
///
/// @author	RDN © Intern :)
///
/// @date   22.05.2019   19:24
///

#pragma once

//#include "GaleryGUIExp.h"
#include <QWidget>

namespace Ui
{
	class AlbumListWidget;
}

class QItemSelectionModel;
namespace gallery
{
	class AlbumModel;

	class /*GALERYGUI_EXT_CLASS*/ AlbumListWidget : public QWidget
	{
		using base_t = QWidget;
	public:
		explicit AlbumListWidget(QWidget * parent = nullptr);
		~AlbumListWidget();
	public:
		void setModel(AlbumModel * model);
		void setSelectionModel(QItemSelectionModel * selectionModel);
	private:
		void createAlbum();
	private:
		::Ui::AlbumListWidget * m_ui;
		AlbumModel * m_AlbumModel;
	};
}
