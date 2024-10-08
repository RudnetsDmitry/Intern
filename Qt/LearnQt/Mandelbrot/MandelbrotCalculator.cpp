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

const int JOB_RESULT_THRESHOLD = 10;

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

	for (int pixelPositionY = 0; pixelPositionY < m_areaSize.height(); pixelPositionY++)
		QThreadPool::globalInstance()->start(createJob(pixelPositionY));
}

void MandelbrotCalculator::process(Job * job, JobResult const & jobResult)
{
	--m_objCount;
	disconnect(this, &MandelbrotCalculator::abortAllJobs,
		job, &Job::abort);

	if (jobResult.values.isEmpty())
	{
		job->deleteLater();
		return;
	}

	if (jobResult.areaSize != m_areaSize ||
		jobResult.moveOffset != m_moveOffset ||
		jobResult.scaleFactor != m_scaleFactor)
	{
		job->deleteLater();
		return;
	}

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
	job->deleteLater();
}

Job* MandelbrotCalculator::createJob(int pixelPositionY)
{
	++m_objCount;

	Job* job = new Job();

	job->setPixelPositionY(pixelPositionY);
	job->setMoveOffset(m_moveOffset);
	job->setScaleFactor(m_scaleFactor);
	job->setAreaSize(m_areaSize);
	job->setIterationMax(m_iterationMax);
	job->setAutoDelete(false);

	connect(this, &MandelbrotCalculator::abortAllJobs,
		job, &Job::abort);

	connect(job, &Job::jobCompleted,
		this, &MandelbrotCalculator::process);

	return job;
}

void MandelbrotCalculator::clearJobs()
{
	m_receivedJobResults = 0;
	emit abortAllJobs();
	QThreadPool::globalInstance()->waitForDone();
	qDebug() << "m_objCount " << m_objCount;
	QThreadPool::globalInstance()->clear();
}
