
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2014 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
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
