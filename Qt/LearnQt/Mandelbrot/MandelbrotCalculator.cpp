/// @file	MandelbrotCalculator.cpp
///
/// @see	MandelbrotCalculator.h
///
/// @author	RDN © Intern :)
///
/// @date    05.06.2024 22:17
///

#include "stdafx.h"
#include "MandelbrotCalculator.h"

#include <iostream>
#include <QDebug>
#include <QThreadPool>

#include "Job.h"

const int JOB_RESULT_THRESHOLD = 20;

MandelbrotCalculator::MandelbrotCalculator(QObject* parent)
	: QObject(parent),
	m_moveOffset(0.0, 0.0),
	m_scaleFactor(0.005),
	m_areaSize(0, 0),
	m_iterationMax(10),
	m_receivedJobResults(0)
{
}

MandelbrotCalculator::~MandelbrotCalculator()
{
	clearJobs();
}

void MandelbrotCalculator::generatePicture(QSize const & areaSize, QPointF moveOffset, double scaleFactor, int iterationMax)
{
	if (areaSize.isEmpty())
		return;

	m_timer.start();
	clearJobs();

	m_areaSize = areaSize;
	m_moveOffset = moveOffset;
	m_scaleFactor = scaleFactor;
	m_iterationMax = iterationMax;

	std::lock_guard lockG(m_mutex);
	for (int pixelPositionY = 0; pixelPositionY < m_areaSize.height(); pixelPositionY++)
		QThreadPool::globalInstance()->start(createJob(pixelPositionY));
}

void MandelbrotCalculator::process(Job * job, JobResult const & jobResult)
{
	std::lock_guard lockG(m_mutex);

	m_jobs.erase(job);
	delete job;
	
	if (jobResult.values.isEmpty())
		return;

	if (jobResult.areaSize != m_areaSize ||
		jobResult.moveOffset != m_moveOffset ||
		jobResult.scaleFactor != m_scaleFactor)
		return;

	m_receivedJobResults++;
	m_jobResults.append(std::move(jobResult));

	if (m_jobResults.size() >= JOB_RESULT_THRESHOLD || m_receivedJobResults == m_areaSize.height())
	{
		emit pictureLinesGenerated(std::move(m_jobResults));
		m_jobResults.clear();
	}

	if (m_receivedJobResults == m_areaSize.height()) 
	{
		qDebug() << "Generated in " << m_timer.elapsed() << " ms";
	}
}

Job* MandelbrotCalculator::createJob(int pixelPositionY)
{
	Job* job = new Job(std::bind(&this_t::process, this, std::placeholders::_1, std::placeholders::_2));

	job->setPixelPositionY(pixelPositionY);
	job->setMoveOffset(m_moveOffset);
	job->setScaleFactor(m_scaleFactor);
	job->setAreaSize(m_areaSize);
	job->setIterationMax(m_iterationMax);
	job->setAutoDelete(false);

	/*connect(this, &MandelbrotCalculator::abortAllJobs,
		job, &Job1::abort);

	connect(job, &Job1::jobCompleted,
		this, &MandelbrotCalculator::process);*/
	m_jobs.insert(job);

	return job;
}

void MandelbrotCalculator::clearJobs()
{
	m_receivedJobResults = 0;
	{
		std::lock_guard lockG(m_mutex);
		for (auto* job : m_jobs)
			job->abort();
	}
	QThreadPool::globalInstance()->waitForDone();
	QThreadPool::globalInstance()->clear();
}

namespace v2
{
	MandelbrotCalculator::MandelbrotCalculator(QObject* parent)
		: QObject(parent),
		m_moveOffset(0.0, 0.0),
		m_scaleFactor(0.005),
		m_areaSize(0, 0),
		m_iterationMax(10),
		m_receivedJobResults(0)
		,m_pool(20)
	{
	}

	MandelbrotCalculator::~MandelbrotCalculator()
	{
		clearJobs();
	}

	void MandelbrotCalculator::generatePicture(QSize const& areaSize, QPointF moveOffset, double scaleFactor, int iterationMax)
	{
		if (areaSize.isEmpty())
			return;

		m_timer.start();
		clearJobs();

		m_areaSize = areaSize;
		m_moveOffset = moveOffset;
		m_scaleFactor = scaleFactor;
		m_iterationMax = iterationMax;

		std::lock_guard lockG(m_mutex);
		for (int pixelPositionY = 0; pixelPositionY < m_areaSize.height(); pixelPositionY++)
		{
			auto job = makeJob(pixelPositionY);
			m_pool.AddTask([job = std::move(job)](std::atomic<bool>& abort) mutable
				{
					job.run(abort);
				});
		}
	}

	void MandelbrotCalculator::process(JobResult && jobResult)
	{
		std::lock_guard lockG(m_mutex);

		if (jobResult.areaSize != m_areaSize ||
			jobResult.moveOffset != m_moveOffset ||
			jobResult.scaleFactor != m_scaleFactor)
			return;

		m_receivedJobResults++;
		m_jobResults.append(std::move(jobResult));

		if (m_jobResults.size() >= JOB_RESULT_THRESHOLD || m_receivedJobResults == m_areaSize.height())
		{
			emit pictureLinesGenerated(std::move(m_jobResults));
			m_jobResults.clear();
		}

		if (m_receivedJobResults == m_areaSize.height())
		{
			qDebug() << "Generated in " << m_timer.elapsed() << " ms";
		}
	}

	v2::Job1 MandelbrotCalculator::makeJob(int pixelPositionY)
	{
		v2::Job1 job;

		job.setPixelPositionY(pixelPositionY);
		job.setMoveOffset(m_moveOffset);
		job.setScaleFactor(m_scaleFactor);
		job.setAreaSize(m_areaSize);
		job.setIterationMax(m_iterationMax);
		job.setCallback(std::bind(&this_t::process, this, std::placeholders::_1));

		return job;
	}

	void MandelbrotCalculator::clearJobs()
	{
		m_receivedJobResults = 0;
		{
			std::lock_guard lockG(m_mutex);
			m_pool.Clear();
		}
	}
}
