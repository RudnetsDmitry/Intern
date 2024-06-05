/// @file	ImageFilters.cpp
///
/// @see	ImageFilters.h
///
/// @author	RDN © Intern :)
///
/// @date    23.04.2024 20:15
///

#include "stdafx.h"
#include "ImageFilters.h"

#include "../GaleryCore/Filter.h"

#include <QtGui/QImage>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace gallery
{
	struct OriginalFilter : public IFilter
	{
		QString name() const override
		{
			return "Original";
		}
		QImage process(const QImage& image) override
		{
			return image;
		}
	};

	struct GrayScaleFilter : public IFilter
	{
		QString name() const override
		{
			return "Grayscale";
		}

		QImage process(QImage const & image) override
		{
			// QImage => cv::mat
			cv::Mat tmp(image.height(),
				image.width(),
				CV_8UC4,
				(uchar*)image.bits(),
				image.bytesPerLine());

			cv::Mat resultMat;
			cv::cvtColor(tmp, resultMat, cv::COLOR_BGR2GRAY);

			// cv::mat => QImage
			QImage resultImage((const uchar*)resultMat.data,
				resultMat.cols,
				resultMat.rows,
				resultMat.step,
				QImage::Format_Grayscale8);
			return resultImage.copy();

		}
	};

	struct BlurFilter : public IFilter
	{
		QString name() const override
		{
			return "Blur";
		}

		QImage process(QImage const& image) override
		{
			// QImage => cv::mat
			cv::Mat tmp(image.height(),
				image.width(),
				CV_8UC4,
				(uchar*)image.bits(),
				image.bytesPerLine());

			int blur = 17;
			cv::Mat resultMat;

			cv::GaussianBlur(tmp,
				resultMat,
				cv::Size(blur, blur),
				0.0,
				0.0);

			// cv::mat => QImage
			QImage resultImage((const uchar*)resultMat.data,
				resultMat.cols,
				resultMat.rows,
				resultMat.step,
				QImage::Format_RGB32);
			return resultImage.copy();
		}
	};

	std::unique_ptr<IFilter> CreateFilter(EFilterType filterType)
	{
		switch (filterType)
		{
		case EFilterType::Original:
			return std::make_unique<OriginalFilter>();
		case EFilterType::Grayscale:
			return std::make_unique<GrayScaleFilter>();
		case EFilterType::Blur:
			return std::make_unique<BlurFilter>();
		default:
			return std::make_unique<OriginalFilter>();
		}
	}
}
