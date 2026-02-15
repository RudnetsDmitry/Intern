/// @file	SolidModelWindow.h
///
/// @brief window for render 3d
///
/// @author	RDN © Intern :)
///
/// @date    15.02.2026 12:48
///

#pragma once
#include <QMainWindow>

class QExposeEvent;

namespace vsgQt
{
	class Window;
}

namespace model3d
{
	class SolidModelWindow : public QMainWindow
	{
		using base_t = QMainWindow;

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	public:
		SolidModelWindow(QWidget* parent);
		~SolidModelWindow() override;

	private:
		void OnRebuild();
	};
}
