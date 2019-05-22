/// @file	PictureModel.h
///
/// @brief model for pictures
///
/// @author	RDN © Intern :)
///
/// @date   22.05.2019   17:12
///

#pragma once
#include "GaleryCoreExp.h"
#include <QAbstractListModel>

#include <memory>

class QModelIndex;
class QVariant;

namespace gallery
{
	class Picture;
	class AlbumModel;

	class GALERYCORE_EXT_CLASS PictureModel : public QAbstractListModel
	{
		using base_t = QAbstractListModel;
	public:
		enum Roles
		{
			FilePathRole = Qt::UserRole + 1,
		};
	public:
		explicit PictureModel(AlbumModel const & albumModel, QObject * parent = nullptr);
		~PictureModel();

		QModelIndex addPicture(const Picture& album);
		int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const
			override;
		bool setData(const QModelIndex& index, const QVariant& value, int role)
			override;
		bool removeRows(int row, int count, const QModelIndex& parent) override;

		QHash<int, QByteArray> roleNames() const override;
	public:
		void setAlbumId(int albumId);
	private:
		bool isIndexValid(const QModelIndex& index) const;
	private:
		class Impl;
		std::unique_ptr<Impl> m_impl;
	};
}


