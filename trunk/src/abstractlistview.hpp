
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

#ifndef QTMWIDGTES__ABSTRACTLISTVIEW_HPP__INCLUDED
#define QTMWIDGTES__ABSTRACTLISTVIEW_HPP__INCLUDED

// QtMWidgets include.
#include "scrollarea.hpp"
#include "private/scrollarea_p.hpp"
#include "listmodel.hpp"


namespace QtMWidgets {

class AbstractListViewBase;


//
// AbstractListViewBasePrivate
//

class AbstractListViewBasePrivate
	:	public ScrollAreaPrivate
{
public:
	AbstractListViewBasePrivate( AbstractListViewBase * parent );
	virtual ~AbstractListViewBasePrivate();

	inline AbstractListViewBase * q_func();

	inline const AbstractListViewBase * q_func() const;

	//! Spacing.
	int spacing;
}; // AbstractListViewBasePrivate


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
	AbstractListViewPrivate( AbstractListView< T > * parent );
	virtual ~AbstractListViewPrivate();

	inline AbstractListView< T > * q_func();
	inline const AbstractListView< T > * q_func() const;

	//! Model.
	ListModel< T > * model;
}; // class AbstractListViewPrivate

//
// AbstractListViewBase
//

class AbstractListViewBase
	:	public ScrollArea
{
	Q_OBJECT
	Q_ENUMS( ScrollHint )
	/*!
		\property spacing

		This property holds the space around the items in the layout.

		This property is the size of the empty space that is padded around
		an item in the layout.

		Setting this property when the view is visible will cause the
		items to be laid out again.

		By default, this property contains a value of 0.
	*/
	Q_PROPERTY( int spacing READ spacing WRITE setSpacing )

signals:
	//! This signal emits when user touched the row.
	void rowTouched( int row );
	//! This signal emits when user long touched the row.
	void rowLongTouched( int row );
	//! This signal emits when user double touched the row.
	void rowDoubleTouched( int row );

public:
	//! Scroll hints.
	enum ScrollHint {
		//! Scroll to ensure that the item is visible.
		EnsureVisible = 0,
		//! Scroll to position the item at the top of the viewport.
		PositionAtTop = 1,
		//! Scroll to position the item at the bottom of the viewport.
		PositionAtBottom = 2,
		//! Scroll to position the item at the center of the viewport.
		PositionAtCenter = 3
	}; // enum ScrollHint

public:
	virtual ~AbstractListViewBase();

	//! \return Spacing.
	int spacing() const;
	//! Set spacing.
	void setSpacing( int s );

protected:
	AbstractListViewBase( AbstractListViewBasePrivate * dd,
		QWidget * parent = 0 );

protected slots:
	virtual void dataChanged( int first, int last ) = 0;
	virtual void modelReset() = 0;
	virtual void rowsInserted( int first, int last ) = 0;
	virtual void rowsRemoved( int first, int last ) = 0;
	virtual void rowsMoved( int sourceStart,
		int sourceEnd, int destinationRow ) = 0;

private:
	friend class AbstractListViewBasePrivate;

	inline AbstractListViewBasePrivate * d_func()
		{ return reinterpret_cast< AbstractListViewBasePrivate* >
			( d.data() ); }
	inline const AbstractListViewBasePrivate * d_func() const
		{ return reinterpret_cast< const AbstractListViewBasePrivate* >
			( d.data() ); }

	Q_DISABLE_COPY( AbstractListViewBase )
}; // class AbstractListViewBase


//
// AbstractListView
//

template< typename T >
class AbstractListView
	:	public AbstractListViewBase
{
public:
	AbstractListView( QWidget * parent = 0 )
		:	AbstractListViewBase(
				new AbstractListViewPrivate< T > ( this ), parent )
	{
	}

	virtual ~AbstractListView()
	{
	}

	ListModel< T > * model() const;
	void setModel( ListModel< T > * m );

	int rowAt( const QPoint & p ) const;
	void scrollTo( int row, ScrollHint hint = EnsureVisible );
	QRect visualRect( int row ) const;

protected:
	explicit AbstractListView( AbstractListViewPrivate< T > * dd,
		QWidget * parent = 0 )
		:	AbstractListViewBase( dd, parent )
	{
	}

	//! Draw row in this list view.
	virtual void drawRow( QPainter * painter,
		const QRect & rect, int row ) = 0;

	virtual void dataChanged( int first, int last );
	virtual void modelReset();
	virtual void rowsInserted( int first, int last );
	virtual void rowsRemoved( int first, int last );
	virtual void rowsMoved( int sourceStart,
		int sourceEnd, int destinationRow );

protected:
	virtual void mousePressEvent( QMouseEvent * e );
	virtual void mouseMoveEvent( QMouseEvent * e );
	virtual void mouseReleaseEvent( QMouseEvent * e );

private:
	friend class AbstractListViewPrivate< T >;

	inline AbstractListViewPrivate< T > * d_func()
		{ return reinterpret_cast< AbstractListViewPrivate< T >* >
			( d.data() ); }
	inline const AbstractListViewPrivate< T > * d_func() const
		{ return reinterpret_cast< const AbstractListViewPrivate< T >* >
			( d.data() ); }

	Q_DISABLE_COPY( AbstractListView )
}; // class AbstractListView


//
// AbstractListViewPrivate
//

template< typename T >
inline
AbstractListViewPrivate< T >::AbstractListViewPrivate(
	AbstractListView< T > * parent )
	:	AbstractListViewBasePrivate( parent )
	,	model( 0 )
{
}

template< typename T >
inline
AbstractListViewPrivate< T >::~AbstractListViewPrivate()
{
}

template< typename T >
inline
AbstractListView< T > *
AbstractListViewPrivate< T >::q_func()
{
	return static_cast< AbstractListView< T >* >( q );
}

template< typename T >
inline
const AbstractListView< T > *
AbstractListViewPrivate< T >::q_func() const
{
	return static_cast< const AbstractListView< T >* >( q );
}

} /* namespace QtMWidgets */

#endif // QTMWIDGTES__ABSTRACTLISTVIEW_HPP__INCLUDED
