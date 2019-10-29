/// @file	ThumbnailProxyModel.cpp
///
/// @see   ThumbnailProxyModel.h
///
/// @author	RDN © Intern :)
///
/// @date   22.05.2019   20:15
///


#include "StdAfx.h"
#include "ThumbnailProxyModel.h"
#include "../GaleryCore/PictureModel.h"

#include <QPixmap>

namespace gallery
{
	const unsigned int THUMBNAIL_SIZE = 350;

	ThumbnailProxyModel::ThumbnailProxyModel(QObject* parent /*= nullptr*/)
		: base_t(parent)
	{
	}

	QVariant ThumbnailProxyModel::data(const QModelIndex& index, int role) const
	{
		if (role != Qt::DecorationRole)
			return base_t::data(index, role);
		
		QString filePath = /*index.model()*/sourceModel()->data(index, PictureModel::Roles::FilePathRole).toString();
		return *m_thumbnails[filePath];
	}

	void ThumbnailProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
	{
		base_t::setSourceModel(sourceModel);
		if (!sourceModel)
			return;

		connect(sourceModel, &QAbstractItemModel::modelReset, [this] {reloadThumbnails(); });
	
		connect(sourceModel, &QAbstractItemModel::rowsInserted,
			[this] (QModelIndex const & parent, int first, int last)
		{
			generateThumbnails(index(first, 0), last - first + 1);
		});
	}

	PictureModel* ThumbnailProxyModel::pictureModel() const
	{
		return static_cast<PictureModel*>(sourceModel());
	}

	void ThumbnailProxyModel::generateThumbnails(const QModelIndex& startIndex, int count)
	{
		if (!startIndex.isValid())
			return;

		auto const * model = startIndex.model();
		for (int i = startIndex.row(), end = i + count; i < end; ++i)
		{
			QString filePath = model->data(model->index(i, 0), PictureModel::Roles::FilePathRole)
				.toString();
			if (filePath.isEmpty())
				continue;
			QPixmap pixmap(filePath);
			if (pixmap.isNull())
				continue;
			auto scaledPixmap = new QPixmap(pixmap.scaled(THUMBNAIL_SIZE, THUMBNAIL_SIZE,
				Qt::KeepAspectRatio, Qt::SmoothTransformation));
			m_thumbnails.insert(filePath, scaledPixmap);
		}
	}

	void ThumbnailProxyModel::reloadThumbnails()
	{
		qDeleteAll(m_thumbnails);
		m_thumbnails.clear();
		generateThumbnails(index(0, 0), rowCount());
	}

}
