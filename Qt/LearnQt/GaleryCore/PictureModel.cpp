/// @file	PictureModel.cpp
///
/// @see   PictureModel.h
///
/// @author	RDN © Intern :)
///
/// @date   22.05.2019   17:14
///


#include "StdAfx.h"
#include "PictureModel.h"

#include "SqlObjectsHolder.h"
#include "Picture.h"
#include "AlbumModel.h"

namespace gallery
{
	class PictureModel::Impl
	{
		using pictures_t = std::vector<std::unique_ptr<Picture>>;

		AlbumModel const & m_albumModel;
		IPictureManager & m_pictureManager;
		pictures_t m_items;
		int m_albumId;

	public:

		Impl(AlbumModel const & albumModel, IGalleryManager & galleryMan)
			: m_albumModel(albumModel)
			, m_pictureManager(galleryMan.GetPictureManager())
			, m_albumId(-1)
		{
		}

		pictures_t const & GetPictures()
		{
			if (m_items.empty() && m_albumId > 0)
				m_items = m_pictureManager.GetAllItems(m_albumId);
			return m_items;
		}

		void SetAlbumId(int albumId)
		{
			m_albumId = albumId;
			ClearPictures();
		}

		void UpdatePicture(Picture const & picture)
		{
			m_pictureManager.UpdateItem(picture);
		}

		void AddPicture(Picture const & picture)
		{
			auto newPicture = std::make_unique<Picture>(picture);
			m_pictureManager.AddItem(m_albumId, *newPicture);
			m_items.emplace_back(std::move(newPicture));
		}

		void RemovePictures(int fromPos, int count)
		{
			auto begIt = m_items.begin() + fromPos;
			auto endIt = m_items.begin() + fromPos + count;

			for (auto it = begIt; it != endIt; ++it)
				m_pictureManager.RemoveItem((*it)->GetId());

			m_items.erase(begIt, endIt);
		}

		void ClearPictures()
		{
			m_items.clear();
		}
	};

	PictureModel::PictureModel(AlbumModel const & albumModel, QObject * parent /*= nullptr*/)
		: base_t(parent)
		, m_impl(std::make_unique<Impl>(albumModel, gallery::GetSqlGaleryManager()))
	{
		connect(&albumModel, &AlbumModel::rowsRemoved, this, [this]() {m_impl->ClearPictures(); });
	}

	PictureModel::~PictureModel() = default;

	QModelIndex PictureModel::addPicture(const Picture& picture)
	{
		int rowIndex = rowCount();
		beginInsertRows(QModelIndex(), rowIndex, rowIndex);
		m_impl->AddPicture(picture);
		endInsertRows();

		return index(rowIndex, 0);
	}

	int PictureModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
	{
		return static_cast<int>(m_impl->GetPictures().size());
	}

	QVariant PictureModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
	{
		if (!isIndexValid(index))
			return QVariant();

		auto const & picture = m_impl->GetPictures()[index.row()];
		switch (role)
		{
		case Roles::FilePathRole:
			return picture->GetFileUrl().toString();
		default:
			return QVariant();
		}
	}

	bool PictureModel::setData(const QModelIndex& index, const QVariant& value, int role)
	{
		if (!isIndexValid(index) || role != Roles::FilePathRole)
			return false;

		auto & picture = m_impl->GetPictures()[index.row()];
		picture->SetFileUrl(value.toString());
		m_impl->UpdatePicture(*picture);

		emit dataChanged(index, index);
		return true;
	}

	bool PictureModel::removeRows(int row, int count, const QModelIndex& parent)
	{
		int rowsCount = rowCount();
		if (row < 0 || count < 0 || row >= rowsCount || (row + count) > rowsCount)
			return false;

		beginRemoveRows(parent, row, row + count - 1);
		m_impl->RemovePictures(row, count);
		endRemoveRows();

		return true;
	}

	QHash<int, QByteArray> PictureModel::roleNames() const
	{
		return { {Roles::FilePathRole, "filePath"} };
	}

	void PictureModel::setAlbumId(int albumId)
	{
		beginResetModel();
		m_impl->SetAlbumId(albumId);
		endResetModel();
	}

	bool PictureModel::isIndexValid(const QModelIndex& index) const
	{
		return index.row() >= 0 && index.row() < rowCount();
	}

}
