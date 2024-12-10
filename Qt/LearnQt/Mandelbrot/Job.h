/// @file	Job1.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    05.06.2024 21:43
///

#pragma once

#include <QRunnable>
#include <QAtomicInteger>

#include "JobResult.h"

class Job : public QRunnable
{
public:
    Job(std::function<void(Job*, JobResult)> func);
    void run() override;

    void setPixelPositionY(int value);
    void setMoveOffset(QPointF const & value);
    void setScaleFactor(double value);
    void setAreaSize(QSize const & value);
    void setIterationMax(int value);

public:
    void abort();

private:
    std::function<void(Job*, JobResult)> m_func;
    QAtomicInteger<bool> m_abort;
    int m_pixelPositionY;
    QPointF m_moveOffset;
    double m_scaleFactor;
    QSize m_areaSize;
    int m_iterationMax;
};

namespace v2
{
    class Job1
    {
    public:
        void run(std::atomic<bool> & abort);

        void setPixelPositionY(int value);
        void setMoveOffset(QPointF const& value);
        void setScaleFactor(double value);
        void setAreaSize(QSize const& value);
        void setIterationMax(int value);
        void setCallback(std::function<void(JobResult)> func);

    private:
        std::function<void(JobResult)> m_func;
        std::atomic<bool> * m_abort = nullptr;
        int m_pixelPositionY = 0;
        QPointF m_moveOffset = {.0, .0};
        double m_scaleFactor = .0;
        QSize m_areaSize = {0, 0};
        int m_iterationMax = 1;
    };
}
