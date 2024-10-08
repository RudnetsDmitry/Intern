/// @file	Filter.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    23.04.2024 15:50
///

#pragma once

class QString;
class QImage;

namespace gallery
{
	struct IFilter
	{
		virtual ~IFilter() = default;

		virtual QString name() const = 0;
		virtual QImage process(QImage const& image) = 0;
	};
}
