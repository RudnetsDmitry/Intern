/// @file	MandelbrotWidget.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date    05.06.2024 22:34
///

#pragma once

#include <memory>

#include <QWidget>
#include <QPoint>
#include <QThread>
#include <QList>

#include "MandelbrotCalculator.h"

class QResizeEvent;

class MandelbrotWidget : public QWidget
{
    Q_OBJECT

	using calculator_t = v2::MandelbrotCalculator;
public:
    explicit MandelbrotWidget(QWidget *parent = 0);
    ~MandelbrotWidget() override;

public slots:
    void processJobResults(QList<JobResult> const & jobResults);

signals:
    void requestPicture(QSize areaSize, QPointF moveOffset, double scaleFactor, int iterationMax);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QRgb generateColorFromIteration(int iteration);
    void clearImage();

private:
    calculator_t m_mandelbrotCalculator;
    QThread m_threadCalculator;
    double m_scaleFactor;
    QPoint m_lastMouseMovePosition;
    QPointF m_moveOffset;
    QSize m_areaSize;
    int m_iterationMax;
    std::unique_ptr<QImage> m_image;
};
