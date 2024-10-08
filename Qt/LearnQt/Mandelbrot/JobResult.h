/// @file	JobResult.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    05.06.2024 21:44
///

#pragma once

#include <QSize>
#include <QVector>
#include <QPointF>
#include <QMetaType>

struct JobResult
{
    JobResult() = default;
    JobResult(int valueCount) : 
        values(valueCount)
    {
    }

    QSize areaSize = {0, 0};
    int pixelPositionY = 0;
    QPointF moveOffset = {0, 0};
    double scaleFactor = .0;

    QVector<int> values;
};

Q_DECLARE_METATYPE(JobResult)
