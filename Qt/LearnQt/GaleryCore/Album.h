/// @file	Album.h
///
/// @brief application aims to organize pictures into albums
///
/// @author	RDN © Intern :)
///
/// @date   03.02.2019   15:39
///

#pragma once
#include "GaleryCoreExp.h"
#include <QString>

namespace gallery
{
	class GALERYCORE_EXT_CLASS Album
	{
	public:
		explicit Album(const QString& name = QString());
	
	public:
		int GetId() const;
		void SetId(int id);
		QString const & GetName() const;
		void SetName(const QString& name);
	private:
		// ToDo -> replace by boost::optional
		int m_Id;
		QString m_Name;
	};
}
