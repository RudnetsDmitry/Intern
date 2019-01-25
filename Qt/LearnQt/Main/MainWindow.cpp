#include "MainWindow.h"
#include "TaskWidget.h"
#include "CpuWidget.h"
#include "MemWidget.h"
#include "SystemInfo.h"

#include <QtWidgets\QInputDialog>
#include <QtWidgets\QMessageBox>

#include <QtCore\QTemporaryFile>

#include "./tmp/ui_MainWindow.h"

namespace qt_learn
{
	struct MainWindow::Impl
	{
		explicit Impl(QWidget * parent)
			: m_cpuWidget(parent)
			, m_memWidget(parent)
		{
		}

		CpuWidget m_cpuWidget;
		MemWidget m_memWidget;
	};

	MainWindow::MainWindow(QWidget * parent /*= nullptr*/)
		: base_t(parent)
		, m_ui(new Ui::MainWindow())
		, m_impl(std::make_unique<Impl>(this))
	{
		m_ui->setupUi(this);
		bool res = connect(m_ui->addTaskButton, &QPushButton::clicked,
			this, &this_t::OnAddTask);
		assert(res);
		SysInfo::Instance().init();
		m_ui->sysWidgetsLayout->addWidget(&m_impl->m_cpuWidget);
		m_ui->sysWidgetsLayout->addWidget(&m_impl->m_memWidget);
	}

	MainWindow::~MainWindow()
	{
	}

	void MainWindow::OnRemoveTask(TaskWidget * taskWidget)
	{
		m_ui->taskLayout->removeWidget(taskWidget);
		taskWidget->setParent(0);
		delete taskWidget;
	}

	template<typename Time = std::chrono::microseconds,
			typename Clock = std::chrono::high_resolution_clock>
	struct perf_timer
	{
		template<typename F, typename... Args>
		static Time duration(F&& f, Args... args)
		{
			auto start = Clock::now();
			std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
			auto end = Clock::now();

			return std::chrono::duration_cast<Time>(end - start);
		}
	};

	std::pair<long long, long long> MakeQStringTest()
	{
		size_t constexpr count = 1000'000'000;
		using timer_t = std::chrono::high_resolution_clock;

		auto t1 = perf_timer<std::chrono::microseconds>::duration([&count]()
		{
			for (size_t i = 0; i < count; i++)
			{
				volatile QString temp("std::chrono::high_resolution_clock");
			}
		});

		auto t2 = perf_timer<std::chrono::microseconds>::duration([&count]()
		{
			for (size_t i = 0; i < count; i++)
			{
				volatile QString temp(QString::fromLatin1("std::chrono::high_resolution_clock"));
			}
		});

		return std::make_pair(t1.count(), t2.count());
	}

	void MainWindow::OnAddTask()
	{
		/*auto testRes = MakeQStringTest();
		QString strMsg = QString("from char - %1 from latin - %2").arg(testRes.first).arg(testRes.second);
		
		QMessageBox::information(this, "", strMsg, QMessageBox::Ok);*/		
		
		bool ok(false);
		QString taskName = QInputDialog::getText(this, tr("Add message"),
			tr("Task name"), QLineEdit::Normal, tr("Untitle task"), &ok);
		if (!ok || taskName.isEmpty())
			return;

		auto * newTask = new TaskWidget(taskName);
		connect(newTask, &TaskWidget::onRemove, this, &this_t::OnRemoveTask);
		m_ui->taskLayout->addWidget(newTask);
	}
}