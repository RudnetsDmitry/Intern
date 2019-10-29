/// @file	PictureWidget.h
///
/// @brief widget to show fullscrean images
///
/// @author	RDN © Intern :)
///
/// @date   06.09.2019   18:15
///

#pragma once

#include <QWidget>

class QItemSelectionModel;
class QItemSelection;

namespace Ui
{
	class PictureWidget;
}

namespace gallery
{
	class PictureModel;
	class ThumbnailProxyModel;

	class PictureWidget : public QWidget
	{
		using base_t = QWidget;
		Q_OBJECT

	public:
		explicit PictureWidget(QWidget *parent = 0);
		~PictureWidget();
		void setModel(ThumbnailProxyModel* model);
		void setSelectionModel(QItemSelectionModel* selectionModel);

	signals:
		void backToGallery();

	protected:
		void resizeEvent(QResizeEvent* event) override;

	private:
		void deletePicture();
		void loadPicture(const QItemSelection& selected);

	private:
		void updatePicturePixmap();

	private:
		Ui::PictureWidget * m_ui;
		ThumbnailProxyModel * m_model;
		QItemSelectionModel * m_selectionModel;
		QPixmap m_pixmap;
	};

}
