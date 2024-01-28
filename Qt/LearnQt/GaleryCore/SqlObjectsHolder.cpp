/// @file	SqlObjectsHolder.cpp
///
/// @see   SqlObjectsHolder.h
///
/// @author	RDN © Intern :)
///
/// @date   22.03.2019   13:09
///

#include "stdafx.h"
#include "SqlObjectsHolder.h"
#include "Album.h"
#include "Picture.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

namespace gallery
{
	class SqlAlbumManager : public IAlbumManager
	{
		QSqlDatabase & m_db;
	public:
		SqlAlbumManager(QSqlDatabase & db)
			: m_db(db)
		{
			if (!m_db.tables().contains("albums"))
			{
				QSqlQuery query(m_db);
				query.exec("CREATE TABLE albums (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT)");
			}
		}

	public:
		virtual void AddItem(Album & album) override
		{
			QSqlQuery sqlQuery(m_db);
			sqlQuery.prepare("INSERT INTO albums (name) VALUES (:name)");
			sqlQuery.bindValue(":name", album.GetName());
			if (sqlQuery.exec())
				album.SetId(sqlQuery.lastInsertId().toInt());
		}

		virtual void UpdateItem(Album const & album) override
		{
			QSqlQuery sqlQuery(m_db);
			sqlQuery.prepare(" UPDATE albums SET name=(:name) WHERE id=(:id)");
			sqlQuery.bindValue(":name", album.GetName());
			sqlQuery.bindValue(":id", album.GetId());
			sqlQuery.exec();
		}

		virtual void RemoveItem(int itemId) override
		{
			QSqlQuery sqlQuery(m_db);
			sqlQuery.prepare("DELETE FROM albums WHERE id=(:id)");
			sqlQuery.bindValue(":id", itemId);
			sqlQuery.exec();
		}

		virtual std::vector<std::unique_ptr<item_t>> GetAllItems() override
		{
			std::vector<std::unique_ptr<item_t>> items;

			QSqlQuery query("SELECT * FROM albums", m_db);
			query.exec();
	
			while (query.next())
			{
				auto album = std::make_unique<Album>();
				album->SetId(query.value("id").toInt());
				album->SetName(query.value("name").toString());

				items.emplace_back(std::move(album));
			}

			return items;
		}
	};

	class SqlPictureManager : public IPictureManager
	{
		QSqlDatabase & m_db;
	public:
		SqlPictureManager(QSqlDatabase & db)
			: m_db(db)
		{
			if (!m_db.tables().contains("pictures"))
			{
				QSqlQuery query(m_db);
				query.exec(QString("CREATE TABLE pictures")
					+ " (id INTEGER PRIMARY KEY AUTOINCREMENT, "
					+ "album_id INTEGER, "
					+ "url TEXT)");
			}
		}

	public:
		virtual void AddItem(key_t const & key, Picture & item) override
		{
			QSqlQuery sqlQuery(m_db);
			sqlQuery.prepare("INSERT INTO pictures (album_id, url) VALUES (:album_id, :url)");
			sqlQuery.bindValue(":album_id", key);
			sqlQuery.bindValue(":url", item.GetFileUrl());

			if (sqlQuery.exec())
			{
				item.SetAlbumId(key);
				item.SetId(sqlQuery.lastInsertId().toInt());
			}
		}

		virtual void UpdateItem(Picture const & item) override
		{
			QSqlQuery sqlQuery(m_db);
			sqlQuery.prepare(" UPDATE pictures SET album_id=(:album_id), url=(:url) WHERE id=(:id)");
			sqlQuery.bindValue(":album_id", item.GetAlbumId());
			sqlQuery.bindValue(":url", item.GetFileUrl());
			sqlQuery.bindValue(":id", item.GetId());
			sqlQuery.exec();
		}

		virtual void RemoveItem(int itemId) override
		{
			QSqlQuery sqlQuery(m_db);
			sqlQuery.prepare("DELETE FROM pictures WHERE id=(:id)");
			sqlQuery.bindValue(":id", itemId);
			sqlQuery.exec();
		}

		virtual void RemoveAll(key_t const & key) override
		{
			QSqlQuery sqlQuery(m_db);
			sqlQuery.prepare("DELETE FROM pictures WHERE album_id=(:album_id)");
			sqlQuery.bindValue(":album_id", key);
			sqlQuery.exec();
		}

		virtual std::vector<std::unique_ptr<item_t>> GetAllItems(key_t const & key) override
		{
			std::vector<std::unique_ptr<item_t>> items;

			QSqlQuery query(m_db);
			query.prepare("SELECT * FROM pictures WHERE album_id = :album_id");
			query.bindValue(":album_id", key);
			query.exec();

			while (query.next())
			{
				auto picture = std::make_unique<Picture>();
				picture->SetId(query.value("id").toInt());
				picture->SetAlbumId(query.value("album_id").toInt());
				picture->SetFileUrl(query.value("url").toString());

				items.emplace_back(std::move(picture));
			}

			return items;
		}
	};

	class SqlGalleryManager : public IGalleryManager
	{
		std::unique_ptr<QSqlDatabase> m_db;
		std::unique_ptr<SqlAlbumManager> m_albums;
		std::unique_ptr<SqlPictureManager> m_pictures;
	public:
		explicit SqlGalleryManager(QString const & dbPath)
			: m_db(std::make_unique<QSqlDatabase>(QSqlDatabase::addDatabase("QSQLITE")))
		{
			m_db->setDatabaseName(dbPath);
			m_db->open();

			m_albums = std::make_unique<SqlAlbumManager>(*m_db);
			m_pictures = std::make_unique<SqlPictureManager>(*m_db);
		}

		~SqlGalleryManager()
		{
			ClearData();
		}

		virtual IAlbumManager & GetAlbumManager() override
		{
			return *m_albums;
		}

		virtual IPictureManager & GetPictureManager() override
		{
			return *m_pictures;
		}
	private:

		void ClearData()
		{
			m_pictures.reset();
			m_albums.reset();
			if (m_db->isOpen())
				m_db->close();
		}
	};

	IGalleryManager & GetSqlGaleryManager()
	{
		static SqlGalleryManager galleryManager("D:/Develop/Projects/ProjectsData/test.db");
		return galleryManager;
	}
}
