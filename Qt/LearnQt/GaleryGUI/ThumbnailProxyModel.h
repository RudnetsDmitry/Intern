/// @file	ThumbnailProxyModel.h
///
/// @brief proxi model for thumbnails...
///
/// @author	RDN © Intern :)
///
/// @date   22.05.2019   20:12
///

#pragma once

#include <QIdentityProxyModel>
#include <QHash>

class QPixmap;

namespace gallery
{
	class PictureModel;
	class ThumbnailProxyModel : public QIdentityProxyModel
	{
		using base_t = QIdentityProxyModel;
	public:
		ThumbnailProxyModel(QObject* parent = nullptr);
		QVariant data(const QModelIndex& index, int role) const override;
		void setSourceModel(QAbstractItemModel* sourceModel) override;
		PictureModel* pictureModel() const;
	private:
		void generateThumbnails(const QModelIndex& startIndex, int count);
		void reloadThumbnails();
	private:
		QHash<QString, QPixmap*> m_thumbnails;
	};
}
