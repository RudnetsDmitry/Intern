/// @file	Job.cpp
///
/// @see	Job.h
///
/// @author	RDN © Intern :)
///
/// @date    05.06.2024 21:50
///

#include "stdafx.h"
#include "Job.h"

#include <iostream>


Job::Job(QObject* parent)
	: QObject(parent) 
	, m_abort(false)
	, m_pixelPositionY(0)
	, m_moveOffset(0.0, 0.0)
	, m_scaleFactor(0.0)
	, m_areaSize(0, 0)
	, m_iterationMax(1)
{
}

void Job::run()
{
	JobResult jobResult(m_areaSize.width());
	jobResult.areaSize = m_areaSize;
	jobResult.pixelPositionY = m_pixelPositionY;
	jobResult.moveOffset = m_moveOffset;
	jobResult.scaleFactor = m_scaleFactor;

	double imageHalfWidth = m_areaSize.width() / 2.0;
	double imageHalfHeight = m_areaSize.height() / 2.0;

	for (int imageX = 0; imageX < m_areaSize.width(); ++imageX)
	{
		if (m_abort.loadRelaxed())
		{
			return emit jobCompleted(this, JobResult());
		}

		int iteration = 0;
		double x0 = (imageX - imageHalfWidth) * m_scaleFactor + m_moveOffset.x();
		double y0 = (m_pixelPositionY - imageHalfHeight) * m_scaleFactor - m_moveOffset.y();
		double x = 0.0;
		double y = 0.0;
		do {
			double nextX = (x * x) - (y * y) + x0;
			y = 2.0 * x * y + y0;
			x = nextX;
			iteration++;

		} while (iteration < m_iterationMax
			&& (x * x) + (y * y) < 4.0);

		jobResult.values[imageX] = iteration;
	}

	emit jobCompleted(this, std::move(jobResult));
}

void Job::setPixelPositionY(int value)
{
	m_pixelPositionY = value;
}

void Job::setMoveOffset(QPointF const& value)
{
	m_moveOffset = value;
}

void Job::setScaleFactor(double value)
{
	m_scaleFactor = value;
}

void Job::setAreaSize(QSize const& value)
{
	m_areaSize = value;
}

void Job::setIterationMax(int value)
{
	m_iterationMax = value;
}

void Job::abort()
{
	m_abort.storeRelaxed(true);
}
