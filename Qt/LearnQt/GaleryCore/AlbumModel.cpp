/// @file	AlbumModel.cpp
///
/// @see   AlbumModel.h
///
/// @author	RDN © Intern :)
///
/// @date   29.04.2019   17:53
///


#include "StdAfx.h"
#include "AlbumModel.h"
#include "SqlObjectsHolder.h"
#include "Album.h"

namespace gallery
{
	class AlbumModel::Impl
	{
		using albums_t = std::vector<std::unique_ptr<Album>>;

		IAlbumManager & m_albumManager;
		albums_t m_items;

	public:

		Impl(IGalleryManager & galleryMan)
			: m_albumManager(galleryMan.GetAlbumManager())
		{
		}

		albums_t const & GetAlbums()
		{
			if (m_items.empty())
				m_items = m_albumManager.GetAllItems();
			return m_items;
		}

		void AddAlmub(Album const & album)
		{
			auto newAlbum = std::make_unique<Album>(album);
			m_albumManager.AddItem(*newAlbum);
			m_items.emplace_back(std::move(newAlbum));
		}

		void RemoveAlbums(int fromPos, int count)
		{
			auto begIt = m_items.begin() + fromPos;
			auto endIt = m_items.begin() + fromPos + count;

			for (auto it = begIt; it != endIt; ++it)
				m_albumManager.RemoveItem((*it)->GetId());

			m_items.erase(begIt, endIt);
		}

		void UpdateAlbum(Album const & album)
		{
			m_albumManager.UpdateItem(album);
		}
	};

	AlbumModel::AlbumModel(QObject * parent /*= nullptr*/)
		: base_t(parent)
		, m_impl(std::make_unique<Impl>(gallery::GetSqlGaleryManager()))
	{
	}

	AlbumModel::~AlbumModel() = default;

	QModelIndex AlbumModel::addAlbum(const Album& album)
	{
		int rowIndex = rowCount();
		beginInsertRows(QModelIndex(), rowIndex, rowIndex);
		m_impl->AddAlmub(album);
		endInsertRows();

		return index(rowIndex, 0);
	}

	int AlbumModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
	{
		return static_cast<int>(m_impl->GetAlbums().size());
	}

	QVariant AlbumModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
	{
		if (!isIndexValid(index))
			return QVariant();

		auto const & album = m_impl->GetAlbums()[index.row()];
		switch (role)
		{
		case Roles::IdRole:
			return album->GetId();
		case Roles::NameRole:
		case Qt::DisplayRole:
			return album->GetName();
		default:
			return QVariant();
		}
	}

	bool AlbumModel::setData(const QModelIndex& index, const QVariant& value, int role)
	{
		if (!isIndexValid(index) || role != Roles::NameRole)
			return false;

		auto & album = m_impl->GetAlbums()[index.row()];
		album->SetName(value.toString());
		m_impl->UpdateAlbum(*album);

		emit dataChanged(index, index);
		return true;
	}

	bool AlbumModel::removeRows(int row, int count, const QModelIndex& parent)
	{
		int rowsCount = rowCount();
		if (row < 0 || count < 0 || row >= rowsCount || (row + count) > rowsCount)
			return false;

		beginRemoveRows(parent, row, row + count - 1);
		m_impl->RemoveAlbums(row, count);
		endRemoveRows();

		return true;
	}

	QHash<int, QByteArray> AlbumModel::roleNames() const
	{
		return { {Roles::IdRole, "id"}, {Roles::NameRole, "name"} };
	}

	bool AlbumModel::isIndexValid(const QModelIndex& index) const
	{
		return index.row() >= 0 && index.row() < rowCount();
	}

}
