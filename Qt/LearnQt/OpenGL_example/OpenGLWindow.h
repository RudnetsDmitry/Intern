/// @file	OpenGLWindow.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date   10.09.2021   19:54
///

#pragma once

#include <QOpenGLWindow>
#include <QMatrix4x4>
#include <QStaticText>

#include "background_renderer.h"

class OpenGLWindow : public QOpenGLWindow
{
	Q_OBJECT

public:
	OpenGLWindow();

protected:
	void paintGL() override;
	void resizeGL(int w, int h) override;
	void keyPressEvent(QKeyEvent* e) override;

private:
	void setAnimating(bool enabled);

	QMatrix4x4 m_window_normalised_matrix;
	QMatrix4x4 m_window_painter_matrix;
	QMatrix4x4 m_projection;
	QMatrix4x4 m_view;
	QMatrix4x4 m_model_triangle;
	QMatrix4x4 m_model_text;

	FragmentToy m_fragment_toy;
	QStaticText m_text_layout;
	bool m_animate;
};

