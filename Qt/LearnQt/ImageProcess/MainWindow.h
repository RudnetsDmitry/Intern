/// @file	MainWindow.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    22.05.2024 21:55
///

#pragma once

#include <QMainWindow>
#include <QImage>
#include <QVector>

namespace Ui {
	class MainWindow;
}

namespace gallery
{
	class FilterWidget;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget* parent = nullptr);
		~MainWindow() override;

		void loadPicture();

	protected:
		void resizeEvent(QResizeEvent* event) override;

	private slots:
		void displayPicture(QImage const& picture);
		void saveAsPicture();

	private:
		void initFilters();
		void updatePicturePixmap();

	private:
		Ui::MainWindow* ui;
		QImage m_filteredPicture;
		QPixmap m_currentPixmap;
		FilterWidget* m_currentFilter;
		QVector<FilterWidget*> m_filters;
	};
}


