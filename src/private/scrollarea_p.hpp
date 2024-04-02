
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef QTMWIDGETS__PRIVATE__SCROLLAREA_P_HPP__INCLUDED
#define QTMWIDGETS__PRIVATE__SCROLLAREA_P_HPP__INCLUDED

// QtMWidgets include.
#include "abstractscrollarea_p.hpp"
#include "../scrollarea.hpp"

// Qt include.
#include <QPointer>
#include <QWidget>


namespace QtMWidgets {

//
// ScrollAreaPrivate
//

class ScrollAreaPrivate
	:	public AbstractScrollAreaPrivate
{
public:
	explicit ScrollAreaPrivate( ScrollArea * parent )
		:	AbstractScrollAreaPrivate( parent )
		,	resizable( false )
	{
	}

	virtual ~ScrollAreaPrivate()
	{
	}

	void updateScrolledSize();
	void updateWidgetPosition();
	void paintEvent( QPaintEvent * e );

	inline ScrollArea * q_func() { return static_cast< ScrollArea* >( q ); }
	inline const ScrollArea * q_func() const { return static_cast< const ScrollArea* >( q ); }

	QPointer< QWidget > widget;
	bool resizable;
	Qt::Alignment alignment;
}; // class ScrollAreaPrivate

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PRIVATE__SCROLLAREA_P_HPP__INCLUDED
