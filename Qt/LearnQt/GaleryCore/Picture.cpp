/// @file	Picture.cpp
///
/// @see   Picture.h
///
/// @author	RDN © Intern :)
///
/// @date   21.02.2019   16:43
///

#include "StdAfx.h"
#include "GaleryCoreDecl.h"
#include "Picture.h"

#include <QUrl>

namespace gallery
{
	Picture::Picture(const QString & filePath /*= QString()*/)
		: Picture(QUrl::fromLocalFile(filePath))
	{
	}

	Picture::Picture(const QUrl & fileUrl)
		: m_fileUrl(fileUrl)
		, m_Id(EntityIdType::InvalidId)
		, m_albumId(EntityIdType::InvalidId)
	{
	}

	int Picture::GetId() const
	{
		return m_Id;
	}

	void Picture::SetId(int id)
	{
		m_Id = id;
	}

	int Picture::GetAlbumId() const
	{
		return m_albumId;
	}

	void Picture::SetAlbumId(int albumId)
	{
		m_albumId = albumId;
	}

	QUrl const & Picture::GetFileUrl() const
	{
		return m_fileUrl;
	}

	void Picture::SetFileUrl(const QUrl& fileUrl)
	{
		m_fileUrl = fileUrl;
	}
}
