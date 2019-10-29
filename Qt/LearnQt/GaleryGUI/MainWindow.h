/// @file	MainWindow.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date   01.10.2019   12:38
///

#pragma once
#include <QMainWindow>

class QStackedWidget;

namespace Ui
{
	class MainWindow;
}

namespace gallery
{
	class GalleryWidget;
	class PictureWidget;

	class MainWindow : public QMainWindow
	{
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private:
		void displayGallery();
		void displayPicture(const QModelIndex& index);

	private:
		Ui::MainWindow * m_ui;
		GalleryWidget * m_galleryWidget;
		PictureWidget * m_pictureWidget;
		QStackedWidget * m_stackedWidget;
	};
}
