/// @file	ThumbnailProxyModel.cpp
///
/// @see   ThumbnailProxyModel.h
///
/// @author	RDN © Intern :)
///
/// @date   22.05.2019   20:15
///


#include "stdafx.h"
#include "ThumbnailProxyModel.h"
#include "../GaleryCore/PictureModel.h"
#include "../GaleryCore/Picture.h"

#include <QPixmap>
#include <execution>

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

		std::vector<QString> filePathes;
		filePathes.reserve(count);

		auto const* model = startIndex.model();
		for (int i = startIndex.row(), end = i + count; i < end; ++i)
		{
			QString filePath = model->data(model->index(i, 0), PictureModel::Roles::FilePathRole)
				.toString();
			if (!filePath.isEmpty())
				filePathes.emplace_back(filePath);
		}

		std::mutex mtx;
		std::for_each(std::execution::par, filePathes.begin(), filePathes.end(), [&](auto const& path)
			{
				QPixmap pixmap = ::gallery::LoadPicture(path);
				if (pixmap.isNull())
					return;
				auto scaledPixmap = new QPixmap(pixmap.scaled(THUMBNAIL_SIZE, THUMBNAIL_SIZE,
					Qt::KeepAspectRatio, Qt::SmoothTransformation));
				std::lock_guard<std::mutex> lock(mtx);
				m_thumbnails.insert(path, scaledPixmap);
			});
	}

	void ThumbnailProxyModel::reloadThumbnails()
	{
		qDeleteAll(m_thumbnails);
		m_thumbnails.clear();
		generateThumbnails(index(0, 0), rowCount());
	}

}
