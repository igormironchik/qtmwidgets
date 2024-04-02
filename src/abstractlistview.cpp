
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// QtMWidgets include.
#include "abstractlistview.hpp"


namespace QtMWidgets {

//
// AbstractListViewBasePrivate
//

AbstractListViewBasePrivate::AbstractListViewBasePrivate(
	AbstractListViewBase * parent )
	:	AbstractScrollAreaPrivate( parent )
	,	spacing( 0 )
{
}

AbstractListViewBasePrivate::~AbstractListViewBasePrivate()
{
}

inline AbstractListViewBase *
AbstractListViewBasePrivate::q_func()
{
	return static_cast< AbstractListViewBase* >( q );
}

inline const AbstractListViewBase *
AbstractListViewBasePrivate::q_func() const
{
	return static_cast< const AbstractListViewBase* >( q );
}


//
// AbstractListViewBase
//

AbstractListViewBase::AbstractListViewBase(
	AbstractListViewBasePrivate * dd, QWidget * parent )
	:	AbstractScrollArea( dd, parent )
{
}

AbstractListViewBase::~AbstractListViewBase()
{

}

int
AbstractListViewBase::spacing() const
{
	const AbstractListViewBasePrivate * d = d_func();

	return d->spacing;
}

void
AbstractListViewBase::setSpacing( int s )
{
	AbstractListViewBasePrivate * d = d_func();

	if( d->spacing != s )
	{
		d->spacing = s;

		recalculateSize();

		update();
	}
}

} /* namespace QtMWidgets */
