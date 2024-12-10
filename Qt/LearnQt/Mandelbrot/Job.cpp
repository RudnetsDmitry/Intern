/// @file	Job1.cpp
///
/// @see	Job1.h
///
/// @author	RDN © Intern :)
///
/// @date    05.06.2024 21:50
///

#include "stdafx.h"
#include "Job.h"

Job::Job(std::function<void(Job*, JobResult)> func)
	: m_abort(false)
	, m_pixelPositionY(0)
	, m_moveOffset(0.0, 0.0)
	, m_scaleFactor(0.0)
	, m_areaSize(0, 0)
	, m_iterationMax(1)
	, m_func(func)
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
			return m_func(this, JobResult());
		}

		int iteration = 0;
		double x0 = (imageX - imageHalfWidth) * m_scaleFactor + m_moveOffset.x();
		double y0 = (m_pixelPositionY - imageHalfHeight) * m_scaleFactor - m_moveOffset.y();
		double x = 0.0;
		double y = 0.0;
		double x2 = x * x;
		double y2 = y * y;
		do {
			double nextX = x2 - y2 + x0;
			y = 2.0 * x * y + y0;
			x = nextX;
			iteration++;
			x2 = x * x;
			y2 = y * y;

		} while (iteration < m_iterationMax
			&& (x2 + y2 < 4.0));

		jobResult.values[imageX] = iteration;
	}

	return m_func(this, std::move(jobResult));
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

namespace v2
{
	void Job1::run(std::atomic<bool>& abort)
	{
		m_abort = &abort;

		JobResult jobResult(m_areaSize.width());
		jobResult.areaSize = m_areaSize;
		jobResult.pixelPositionY = m_pixelPositionY;
		jobResult.moveOffset = m_moveOffset;
		jobResult.scaleFactor = m_scaleFactor;

		double imageHalfWidth = m_areaSize.width() / 2.0;
		double imageHalfHeight = m_areaSize.height() / 2.0;

		for (int imageX = 0; imageX < m_areaSize.width(); ++imageX)
		{
			if (*m_abort)
				return;

			int iteration = 0;
			double x0 = (imageX - imageHalfWidth) * m_scaleFactor + m_moveOffset.x();
			double y0 = (m_pixelPositionY - imageHalfHeight) * m_scaleFactor - m_moveOffset.y();
			double x = 0.0;
			double y = 0.0;
			double x2 = x * x;
			double y2 = y * y;
			do {
				double nextX = x2 - y2 + x0;
				y = 2.0 * x * y + y0;
				x = nextX;
				iteration++;
				x2 = x * x;
				y2 = y * y;

			} while (iteration < m_iterationMax
				&& (x2 + y2 < 4.0));

			jobResult.values[imageX] = iteration;
		}

		return m_func(std::move(jobResult));
	}

	void Job1::setPixelPositionY(int value)
	{
		m_pixelPositionY = value;
	}

	void Job1::setMoveOffset(QPointF const& value)
	{
		m_moveOffset = value;
	}

	void Job1::setScaleFactor(double value)
	{
		m_scaleFactor = value;
	}

	void Job1::setAreaSize(QSize const& value)
	{
		m_areaSize = value;
	}

	void Job1::setIterationMax(int value)
	{
		m_iterationMax = value;
	}

	void Job1::setCallback(std::function<void(JobResult)> func)
	{
		m_func = std::move(func);
	}
}
