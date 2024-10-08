/// @file	Job.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    05.06.2024 21:43
///

#pragma once

#include <QObject>
#include <QRunnable>
#include <QAtomicInteger>

#include "JobResult.h"

class Job : public QObject, public QRunnable
{
    Q_OBJECT
public:
    Job(QObject *parent = 0);
    void run() override;

    void setPixelPositionY(int value);
    void setMoveOffset(QPointF const & value);
    void setScaleFactor(double value);
    void setAreaSize(QSize const & value);
    void setIterationMax(int value);

signals:
    void jobCompleted(Job *, JobResult const & jobResult);

public slots:
    void abort();

private:
    QAtomicInteger<bool> m_abort;
    int m_pixelPositionY;
    QPointF m_moveOffset;
    double m_scaleFactor;
    QSize m_areaSize;
    int m_iterationMax;
};
