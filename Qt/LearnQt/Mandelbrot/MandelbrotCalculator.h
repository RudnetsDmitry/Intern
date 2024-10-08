/// @file	MandelbrotCalculator.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    05.06.2024 22:14
///

#pragma once

#include <QObject>
#include <QElapsedTimer>
#include <QList>

#include "JobResult.h"

class Job;

class MandelbrotCalculator : public QObject
{
    Q_OBJECT
public:
    explicit MandelbrotCalculator(QObject *parent = 0);
    ~MandelbrotCalculator() override;

signals:
    void pictureLinesGenerated(QList<JobResult> const & jobResults);
    void abortAllJobs();

public slots:
    void generatePicture(QSize const & areaSize, QPointF moveOffset, double scaleFactor, int iterationMax);
    void process(Job * job, JobResult const & jobResult);

private:
    Job* createJob(int pixelPositionY);
    void clearJobs();

private:
    QPointF m_moveOffset;
    double m_scaleFactor;
    QSize m_areaSize;
    int m_iterationMax;
    int m_receivedJobResults;
    QList<JobResult> m_jobResults;
    QElapsedTimer m_timer;
    int64_t m_objCount = 0;
};
