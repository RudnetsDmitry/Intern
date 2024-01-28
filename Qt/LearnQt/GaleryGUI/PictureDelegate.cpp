/// @file	PictureDelegate.cpp
///
/// @see   PictureDelegate.h
///
/// @author	RDN © Intern :)
///
/// @date   29.10.2019   15:37
///

#include "stdafx.h"
#include "PictureDelegate.h"

#include <QPainter>

namespace gallery
{
	const unsigned int BANNER_HEIGHT = 20;
	const unsigned int BANNER_COLOR = 0x303030;
	const unsigned int BANNER_ALPHA = 200;
	const unsigned int BANNER_TEXT_COLOR = 0xffffff;
	const unsigned int HIGHLIGHT_ALPHA = 100;

	//PictureDelegate
	void PictureDelegate::paint(QPainter* painter,
		const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		if (!index.isValid())
			return;

		painter->save();

		QPixmap const & pixmap = index.model()->data(index, Qt::DecorationRole).value<QPixmap>();
		if (pixmap.isNull())
			return;
		
		QRect pixmapRect = pixmap.rect();
		painter->drawPixmap(option.rect.x(), option.rect.y(), pixmap);

		QRect bannerRect(option.rect.x(), option.rect.y(), pixmap.width(), BANNER_HEIGHT);
		QColor bannerColor(BANNER_COLOR);
		bannerColor.setAlpha(BANNER_ALPHA);
		painter->fillRect(bannerRect, bannerColor);

		QString fileName = index.model()->data(index, Qt::DisplayRole).toString();
		painter->setPen(BANNER_TEXT_COLOR);
		painter->drawText(bannerRect, Qt::AlignCenter, fileName);

		if (option.state.testFlag(QStyle::State_Selected))
		{
			QColor selectedColor = option.palette.highlight().color();
			selectedColor.setAlpha(BANNER_ALPHA);
			painter->fillRect(option.rect, selectedColor);
		}

		painter->restore();
	}
	
	QSize PictureDelegate::sizeHint(const QStyleOptionViewItem& option,
		const QModelIndex& index) const
	{
		QPixmap const& pixmap = index.model()->data(index, Qt::DecorationRole).value<QPixmap>();
		return pixmap.size();
	}
}
