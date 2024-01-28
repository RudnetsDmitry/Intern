/// @file	Picture.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date   21.02.2019   16:39
///

#pragma once

#include "GaleryCoreExp.h"
#include <QString>
#include <QUrl>

class QPixmap;

namespace gallery
{
	class GALERYCORE_EXT_CLASS Picture
	{
	public:
		explicit Picture(const QString & filePath = QString());
		explicit Picture(const QUrl & fileUrl);

		int GetId() const;
		void SetId(int id);

		int GetAlbumId() const;
		void SetAlbumId(int albumId);

		QUrl const & GetFileUrl() const;
		void SetFileUrl(const QUrl& fileUrl);

	private:
		int m_Id;
		int m_albumId;
		QUrl m_fileUrl;
	};

	GALERYCORE_EXT_CLASS QPixmap LoadPicture(QString const & filePath);

}
