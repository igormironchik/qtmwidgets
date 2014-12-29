
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

#ifndef QTMWIDGETS__PRIVATE__ABSTRACTLISTVIEW_P_HPP__INCLUDED
#define QTMWIDGETS__PRIVATE__ABSTRACTLISTVIEW_P_HPP__INCLUDED

// QtMWidgets include.
#include "scrollarea_p.hpp"


namespace QtMWidgets {

class AbstractListViewBase;


//
// AbstractListViewBasePrivate
//

class AbstractListViewBasePrivate
	:	public ScrollAreaPrivate
{
public:
	AbstractListViewBasePrivate( AbstractListViewBase * parent )
		:	ScrollAreaPrivate( parent )
		,	spacing( 0 )
	{
	}

	virtual ~AbstractListViewBasePrivate()
	{
	}

	inline AbstractListViewBase *
	q_func() { return static_cast< AbstractListViewBase* >( q ); }

	inline const AbstractListViewBase *
	q_func() const { return static_cast< const AbstractListViewBase* >( q ); }

	//! Spacing.
	int spacing;
}; // AbstractListViewBasePrivate


template< typename T >
class ListModel;

template< typename T >
class AbstractListView;


//
// AbstractListViewPrivate
//

template< typename T >
class AbstractListViewPrivate
	:	public AbstractListViewBasePrivate
{
public:
	AbstractListViewPrivate( AbstractListView< T > * parent )
		:	AbstractListViewBasePrivate( parent )
		,	model( 0 )
	{
	}

	virtual ~AbstractListViewPrivate()
	{
	}

	inline AbstractListView< T > *
	q_func() { return static_cast< AbstractListView< T >* >( q ); }

	inline const AbstractListView< T > *
	q_func() const { return static_cast< const AbstractListView< T >* >( q ); }

	//! Model.
	ListModel< T > * model;
}; // class AbstractListViewPrivate

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PRIVATE__ABSTRACTLISTVIEW_P_HPP__INCLUDED
