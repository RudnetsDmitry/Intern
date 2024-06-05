/// @file	ImageFilters.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    23.04.2024 20:12
///

#pragma once
#include <memory>

#include "ImageProcessExp.h"

class QString;
namespace gallery
{
	struct IFilter;

	enum class EFilterType
	{
		Original,
		Blur,
		Grayscale
	};

	IMAGEPROCESS_EXT_CLASS std::unique_ptr<IFilter> CreateFilter(EFilterType filterType);
}
