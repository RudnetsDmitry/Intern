/// @file	AlbumModel.h
///
/// @brief model for album data
///
/// @author	RDN © Intern :)
///
/// @date   29.04.2019   17:47
///

#pragma once
#include "GaleryCoreExp.h"
#include <QAbstractListModel>

#include <memory>

class QModelIndex;
class QVariant;

namespace gallery
{
	class Album;

	class GALERYCORE_EXT_CLASS AlbumModel : public QAbstractListModel
	{
		using base_t = QAbstractListModel;
	public:
		enum Roles
		{
			IdRole = Qt::UserRole + 1,
			NameRole
		};
	public:
		AlbumModel(QObject * parent = nullptr);
		~AlbumModel();

		QModelIndex addAlbum(const Album& album);
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const
			override;
		bool setData(const QModelIndex& index, const QVariant& value, int role)
			override;
		bool removeRows(int row, int count, const QModelIndex& parent) override;
		
		QHash<int, QByteArray> roleNames() const override;
	private:
		bool isIndexValid(const QModelIndex& index) const;
	private:
		class Impl;
		std::unique_ptr<Impl> m_impl;


	};
}