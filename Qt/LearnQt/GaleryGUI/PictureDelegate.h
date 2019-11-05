/// @file	PictureDelegate.h
///
/// @brief
///
/// @author	RDN © Intern :)
///
/// @date   29.10.2019   14:43
///

#pragma once
#include <QStyledItemDelegate>

namespace gallery
{
	class PictureDelegate : public QStyledItemDelegate
	{
		using base_t = QStyledItemDelegate;
	public:
		using base_t::base_t;
	public:
		void paint(QPainter* painter,
			const QStyleOptionViewItem& option, const QModelIndex& index) const override;
		QSize sizeHint(const QStyleOptionViewItem& option,
			const QModelIndex& index) const override;
	};
}
