#include "TaskWidget.h"

#include <QtWidgets\QInputDialog>

#include "./tmp/ui_TaskWidget.h"

namespace qt_learn
{
	TaskWidget::TaskWidget(QString const & name, QWidget * parent /*= nullptr*/)
		: base_t(parent)
		, m_ui(new Ui::TaskWidget)
	{
		m_ui->setupUi(this);
		setName(name);
		connect(m_ui->editButton, &QPushButton::clicked, this, &this_t::onRename);
		connect(m_ui->removeButton, &QPushButton::clicked, 
			[this] {emit onRemove(this); });
	}

	TaskWidget::~TaskWidget()
	{
		delete m_ui;
	}

	void TaskWidget::setName(QString const & name)
	{
		m_ui->checkBox->setText(name);
	}

	void TaskWidget::onRename()
	{
		bool ok;
		QString value = QInputDialog::getText(this, tr("Edit task"),
			tr("Task name"),
			QLineEdit::Normal,
			this->name(), &ok);
		if (ok && !value.isEmpty()) {
			setName(value);
		}
	}

	QString TaskWidget::name() const
	{
		return m_ui->checkBox->text();
	}

	bool TaskWidget::isComplited() const
	{
		return m_ui->checkBox->isChecked();
	}
}