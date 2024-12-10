/// @file	MandelbrotCalculator.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    05.06.2024 22:14
///

#pragma once

#include <mutex>
#include <QObject>
#include <QElapsedTimer>
#include <QList>
#include <unordered_set>

#include "ThreadPool.h"
#include "JobResult.h"

class Job;

class MandelbrotCalculator : public QObject
{
    using this_t = MandelbrotCalculator;
    Q_OBJECT
public:
    explicit MandelbrotCalculator(QObject *parent = 0);
    ~MandelbrotCalculator() override;

signals:
    void pictureLinesGenerated(QList<JobResult> const & jobResults);

public slots:
    void generatePicture(QSize const & areaSize, QPointF moveOffset, double scaleFactor, int iterationMax);

private:
    Job* createJob(int pixelPositionY);
    void clearJobs();
    void process(Job* job, JobResult const& jobResult);

private:
    std::unordered_set<Job*> m_jobs;
    QPointF m_moveOffset;
    double m_scaleFactor;
    QSize m_areaSize;
    int m_iterationMax;
    int m_receivedJobResults;
    QList<JobResult> m_jobResults;
    QElapsedTimer m_timer;
    std::mutex m_mutex;
};

namespace v2
{
    class Job1;
    class MandelbrotCalculator : public QObject
    {
        using this_t = MandelbrotCalculator;
        Q_OBJECT
    public:
        explicit MandelbrotCalculator(QObject* parent = 0);
        ~MandelbrotCalculator() override;

    signals:
        void pictureLinesGenerated(QList<JobResult> const& jobResults);

    public slots:
        void generatePicture(QSize const& areaSize, QPointF moveOffset, double scaleFactor, int iterationMax);

    private:
        Job1 makeJob(int pixelPositionY);
        void clearJobs();
        void process(JobResult && jobResult);

    private:
        QPointF m_moveOffset;
        double m_scaleFactor;
        QSize m_areaSize;
        int m_iterationMax;
        int m_receivedJobResults;
        QList<JobResult> m_jobResults;
        QElapsedTimer m_timer;
        std::mutex m_mutex;

        core::ThreadPool m_pool;
    };
}
