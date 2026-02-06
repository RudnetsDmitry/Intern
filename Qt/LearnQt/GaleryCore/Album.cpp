/// @file	Album.cpp
///
/// @see   Album.h
///
/// @author	RDN © Intern :)
///
/// @date   03.02.2019   15:44
///

#include "stdafx.h"
#include "GaleryCoreDecl.h"

#include "Album.h"

namespace gallery
{
	Album::Album(const QString & name /*= QString()*/)
		: m_Name(name)
		, m_Id(EntityIdType::InvalidId)
	{
	}

	int Album::GetId() const
	{
		return m_Id;
	}

	void Album::SetId(int id)
	{
		m_Id = id;
	}

	QString const & Album::GetName() const
	{
		return m_Name;
	}

	void Album::SetName(const QString& name)
	{
		m_Name = name;
	}
}
