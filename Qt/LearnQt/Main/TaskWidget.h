#pragma once

#include <QtWidgets\qwidget.h>

class QString;

namespace Ui
{
	class TaskWidget;
}

namespace qt_learn
{
	class TaskWidget : public QWidget
	{
		using base_t = QWidget;
		using this_t = TaskWidget;

		Q_OBJECT
	public:
		TaskWidget(QString const & name, QWidget * parent = nullptr);
		~TaskWidget();

	public:
		void setName(QString const & name);
		QString name() const;
		bool isComplited() const;
	private slots:
		void onRename();
	
	signals:
		void onRemove(TaskWidget *);
	private:
		Ui::TaskWidget * m_ui;
	};

	
}