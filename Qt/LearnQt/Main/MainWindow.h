#pragma once
#include <QtWidgets\QMainWindow>
#include <vector>

namespace Ui
{
	class MainWindow;
}
class Ui_MainWindow;

namespace qt_learn
{
	class TaskWidget;
	class MainWindow : public QMainWindow
	{
		using base_t = QMainWindow;
		using this_t = MainWindow;

		Q_OBJECT
	public:
		MainWindow(QWidget * parent = nullptr);
		~MainWindow();
	private slots:
		void OnAddTask();
		void OnRemoveTask(TaskWidget *);
	private:
		std::unique_ptr<Ui::MainWindow> m_ui;
		std::vector<TaskWidget*> m_tasks;
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}