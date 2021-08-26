
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
#include "abstractscrollarea.hpp"
#include "private/abstractscrollarea_p.hpp"
#include "listmodel.hpp"
#include "fingergeometry.hpp"

// Qt include.
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QElapsedTimer>
#include <QPainter>


namespace QtMWidgets {

class AbstractListViewBase;


//
// AbstractListViewBasePrivate
//

class AbstractListViewBasePrivate
	:	public AbstractScrollAreaPrivate
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

	int maxOffsetAndFirstVisibleRow( int * row = 0 ) const;
	int calculateScroll( int row, int expectedOffset ) const;
	bool canScrollDown( int row ) const;
	void normalizeOffset( int & row, int & offset );
	QSize calcScrolledAreaSize() const;
	bool updateIfNeeded( int firstRow, int lastRow );
	void init();

	inline AbstractListView< T > * q_func();
	inline const AbstractListView< T > * q_func() const;

	//! Model.
	ListModel< T > * model;
	//! First visible row.
	int firstVisibleRow;
	//! Offset in drawing.
	int offset;
	//! Mouse move delta.
	int mouseMoveDelta;
	//! Click count;
	int clickCount;
	//! Timer.
	QTimer * timer;
	//! Elapsed timer.
	QElapsedTimer elapsedTimer;
}; // class AbstractListViewPrivate


namespace Private {

//
// Viewport
//

template< typename T >
class Viewport
	:	public QWidget
{
public:
	Viewport( QWidget * parent )
		:	QWidget( parent )
	{
		setContentsMargins( 0, 0, 0, 0 );
	}

	void setData( AbstractListViewPrivate< T > * d )
	{
		data = d;
	}

protected:
	void paintEvent( QPaintEvent * ) override
	{
		QPainter p( this );

		drawListView( &p );
	}

private:
	void drawListView( QPainter * p )
	{
		int row = data->firstVisibleRow;
		const int spacing = data->spacing;
		const QRect r = rect();
		const int x = spacing;
		int y = data->offset + spacing;

		if( data->model && row >= 0 )
			while( y < r.y() + r.height() && row < data->model->rowCount() )
			{
				const int width = r.width() - spacing * 2;
				const int height =
					data->q_func()->rowHeightForWidth( row, width );

				const QRect rowRect( x, y, width, height );

				data->q_func()->drawRow( p, rowRect, row );

				y += height + spacing;
				++row;
			}
	}

private:
	AbstractListViewPrivate< T > * data;
}; // class Viewport

} /* namespace Private */


//
// AbstractListViewBase
//

class AbstractListViewBase
	:	public AbstractScrollArea
{
	Q_OBJECT

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

	Q_ENUM( ScrollHint )

public:
	virtual ~AbstractListViewBase();

	//! \return Spacing.
	int spacing() const;
	//! Set spacing.
	void setSpacing( int s );

protected:
	AbstractListViewBase( AbstractListViewBasePrivate * dd,
		QWidget * parent = 0 );

	virtual void recalculateSize() = 0;

protected slots:
	virtual void dataChanged( int first, int last ) = 0;
	virtual void modelReset() = 0;
	virtual void rowsInserted( int first, int last ) = 0;
	virtual void rowsRemoved( int first, int last ) = 0;
	virtual void rowsMoved( int sourceStart,
		int sourceEnd, int destinationRow ) = 0;
	virtual void timerElapsed() = 0;

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
		AbstractListViewPrivate< T > * d = d_func();

		d->init();
	}

	virtual ~AbstractListView()
	{
	}

	//! \return Model.
	ListModel< T > * model() const
	{
		const AbstractListViewPrivate< T > * d = d_func();

		return d->model;
	}

	//! Set model.
	void setModel( ListModel< T > * m )
	{
		AbstractListViewPrivate< T > * d = d_func();

		if( d->model )
			disconnect( d->model, 0, this, 0 );

		d->model = m;

		connect( d->model, &ListModel< T >::dataChanged,
			this, &AbstractListView< T >::dataChanged );
		connect( d->model, &ListModel< T >::modelReset,
			this, &AbstractListView< T >::modelReset );
		connect( d->model, &ListModel< T >::rowsInserted,
			this, &AbstractListView< T >::rowsInserted );
		connect( d->model, &ListModel< T >::rowsRemoved,
			this, &AbstractListView< T >::rowsRemoved );
		connect( d->model, &ListModel< T >::rowsMoved,
			this, &AbstractListView< T >::rowsMoved );
	}

	/*!
		\return The model row of the item at the viewport coordinates point.
	*/
	int rowAt( const QPoint & p ) const
	{
		const AbstractListViewPrivate< T > * d = d_func();

		int row = d->firstVisibleRow;
		const int spacing = d->spacing;
		const int x = spacing;
		int y = d->offset;
		const int width = d->viewport->width() - spacing * 2;

		if( p.x() < x || p.x() > x + width )
			return -1;

		bool lastIteration = false;

		if( d->model )
			while( row >= 0 && row < d->model->rowCount() )
			{
				int height = rowHeightForWidth( row, width );

				const QRect r( x, y, width, height );

				if( r.contains( p ) )
					return row;

				if( !lastIteration )
				{
					if( p.y() < y )
					{
						int prevRowHeight = -1;

						--row;

						if( row < 0 )
							return -1;

						prevRowHeight = rowHeightForWidth( row, width );

						y -= ( prevRowHeight + spacing );

						if( y < p.y() )
							lastIteration = true;
					}
					else if( p.y() > y )
					{
						++row;

						y += height + spacing;

						if( y > p.y() )
							lastIteration = true;
					}
				}
				else
					return -1;
			}

		return -1;
	}

	/*!
		Scrolls the view if necessary to ensure that the item at
		\a row row is visible. The view will try to position the item
		according to the given \a hint hint.
	*/
	void scrollTo( int row, ScrollHint hint = EnsureVisible )
	{
		AbstractListViewPrivate< T > * d = d_func();

		if( d->model && row >= 0 && row < d->model->rowCount() )
		{
			switch( hint )
			{
				case EnsureVisible :
				{
					if( !visualRect( row ).isNull() )
						return;
					else
					{
						const int offset = -1;

						const int delta = d->calculateScroll( row, offset );

						QPoint p = topLeftPointShownArea();
						p.setY( p.y() + delta );
						setTopLeftPointShownArea( p );
					}
				}
					break;

				case PositionAtTop :
				{
					const int offset = -1;

					const int delta = d->calculateScroll( row, offset );

					QPoint p = topLeftPointShownArea();
					p.setY( p.y() + delta );
					setTopLeftPointShownArea( p );
				}
					break;

				case PositionAtBottom :
				{
					const QRect r = d->viewport->rect();

					const int offset = r.y() + r.height() -
						rowHeightForWidth( row,
							r.width() - d->spacing * 2 ) - d->spacing - 1;

					const int delta = d->calculateScroll( row, offset );

					QPoint p = topLeftPointShownArea();
					p.setY( p.y() + delta );
					setTopLeftPointShownArea( p );
				}
					break;

				case PositionAtCenter :
				{
					const QRect r = d->viewport->rect();

					const int offset = r.y() + r.height() / 2 -
						rowHeightForWidth( row,
							r.width() - d->spacing * 2 ) / 2;

					const int delta = d->calculateScroll( row, offset );

					QPoint p = topLeftPointShownArea();
					p.setY( p.y() + delta );
					setTopLeftPointShownArea( p );
				}
					break;

				default :
					break;
			}
		}
	}

	/*!
		Returns the rectangle on the viewport occupied by the
		item at \a row row.

		If your item is displayed in several areas then visualRect
		returns the primary area that contains index and not the complete
		area that \a row row might encompasses, touch or cause drawing.
	*/
	QRect visualRect( int row ) const
	{
		const AbstractListViewPrivate< T > * d = d_func();

		if( row >= d->firstVisibleRow )
		{
			int tmpRow = d->firstVisibleRow;
			const QRect r = d->viewport->rect();
			const int spacing = d->spacing;
			const int x = r.x() + spacing;
			int y = r.y() + d->offset;
			const int width = r.width() - spacing * 2;
			int height = rowHeightForWidth( tmpRow, width );

			while( tmpRow < row )
			{
				y += height + spacing;

				if( y > r.height() )
					return QRect();

				++tmpRow;
				height = rowHeightForWidth( tmpRow, width );
			}

			return r.intersected( QRect( x, y, width, height ) );
		}
		else
			return QRect();
	}

protected:
	explicit AbstractListView( AbstractListViewPrivate< T > * dd,
		QWidget * parent = 0 )
		:	AbstractListViewBase( dd, parent )
	{
		AbstractListViewPrivate< T > * d = d_func();

		d->init();
	}

	//! Draw row in the list view.
	virtual void drawRow( QPainter * painter,
		const QRect & rect, int row ) = 0;

	//! \return Height of the given \a row row for the given \a width width.
	virtual int rowHeightForWidth( int row, int width ) const
	{
		Q_UNUSED( row )
		Q_UNUSED( width )

		return FingerGeometry::height();
	}

	void scrollContentsBy( int dx, int dy ) override
	{
		Q_UNUSED( dx )

		AbstractListViewPrivate< T > * d = d_func();

		d->offset += dy;

		d->normalizeOffset( d->firstVisibleRow, d->offset );
	}

	void dataChanged( int first, int last ) override
	{
		AbstractListViewPrivate< T > * d = d_func();

		recalculateSize();

		d->updateIfNeeded( first, last );
	}

	void modelReset() override
	{
		AbstractListViewPrivate< T > * d = d_func();

		d->firstVisibleRow = -1;
		d->offset = 0;

		recalculateSize();

		d->viewport->update();
	}

	void rowsInserted( int first, int last ) override
	{
		AbstractListViewPrivate< T > * d = d_func();

		if( d->firstVisibleRow == -1 )
			d->firstVisibleRow = 0;

		recalculateSize();

		d->updateIfNeeded( first, last );
	}

	void rowsRemoved( int first, int last ) override
	{
		AbstractListViewPrivate< T > * d = d_func();

		if( d->firstVisibleRow >= first && d->firstVisibleRow <= last )
		{
			if( d->model && d->model->rowCount() )
			{
				if( first - 1 >= 0 )
					d->firstVisibleRow = first - 1;
				else
					d->firstVisibleRow = last + 1;

				d->offset = 0;
			}
			else
			{
				d->firstVisibleRow = -1;
				d->offset = 0;
			}
		}

		recalculateSize();

		d->updateIfNeeded( first, last )	;
	}

	void rowsMoved( int sourceStart,
		int sourceEnd, int destinationRow ) override
	{
		AbstractListViewPrivate< T > * d = d_func();

		if( ( d->firstVisibleRow >= sourceStart
				&& d->firstVisibleRow <= sourceEnd ) ||
			( d->firstVisibleRow >= destinationRow
				&& d->firstVisibleRow <= destinationRow
						+ sourceEnd - sourceStart ) )
			d->offset = 0;

		if( !d->updateIfNeeded( sourceStart, sourceEnd ) )
			d->updateIfNeeded( destinationRow,
				destinationRow + sourceEnd - sourceStart );
	}

	void timerElapsed() override
	{
		AbstractListViewPrivate< T > * d = d_func();

		d->timer->stop();

		const int row = rowAt( d->mousePos );

		if( row >= 0 && row < d->model->rowCount() )
			emit rowLongTouched( row );
	}

	void recalculateSize() override
	{
		AbstractListViewPrivate< T > * d = d_func();

		setScrolledAreaSize( d->calcScrolledAreaSize() );
	}

protected:
	void mousePressEvent( QMouseEvent * e ) override
	{
		AbstractListViewBase::mousePressEvent( e );

		AbstractListViewPrivate< T > * d = d_func();

		if( e->button() == Qt::LeftButton )
		{
			if( d->elapsedTimer.elapsed() > 500 )
				d->clickCount = 0;

			d->mouseMoveDelta = 0;
			d->timer->start( 2000 );
			d->elapsedTimer.start();
		}
	}

	void mouseMoveEvent( QMouseEvent * e ) override
	{
		AbstractListViewPrivate< T > * d = d_func();

		d->mouseMoveDelta += ( d->mousePos - e->pos() ).manhattanLength();

		if( d->mouseMoveDelta > FingerGeometry::longTouchBounce() )
			d->timer->stop();

		AbstractListViewBase::mouseMoveEvent( e );
	}

	void mouseReleaseEvent( QMouseEvent * e ) override
	{
		AbstractListViewBase::mouseReleaseEvent( e );

		AbstractListViewPrivate< T > * d = d_func();

		d->timer->stop();

		if( e->button() == Qt::LeftButton &&
			d->mouseMoveDelta <= FingerGeometry::touchBounce() )
		{
			const int row = rowAt( e->pos() );

			if( row >= 0 && row < d->model->rowCount() )
				emit rowTouched( row );

			if( d->elapsedTimer.elapsed() <= 500 )
				++d->clickCount;
			else
				d->clickCount = 0;

			d->elapsedTimer.start();

			if( d->clickCount == 2 && row >= 0 && row < d->model->rowCount() )
				emit rowDoubleTouched( row );
		}
	}

	void resizeEvent( QResizeEvent * e ) override
	{
		AbstractListViewBase::resizeEvent( e );

		recalculateSize();

		AbstractListViewPrivate< T > * d = d_func();

		if( d->model &&
			scrolledAreaSize().height() - topLeftPointShownArea().y() <=
				d->viewport->height() )
				scrollTo( d->model->rowCount() - 1, PositionAtBottom );
	}

private:
	friend class AbstractListViewPrivate< T >;
	friend class Private::Viewport< T >;

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
	,	firstVisibleRow( -1 )
	,	offset( 0 )
	,	mouseMoveDelta( 0 )
	,	clickCount( 0 )
	,	timer( 0 )
{
}

template< typename T >
inline
AbstractListViewPrivate< T >::~AbstractListViewPrivate()
{
}

template< typename T >
inline
int
AbstractListViewPrivate< T >::maxOffsetAndFirstVisibleRow( int * row ) const
{
	const QRect r = viewport->rect();
	const int width = r.width() - spacing * 2;
	int tmpRow = ( model ? model->rowCount() - 1 : -1 );
	int y = 0;

	const AbstractListView< T > * q = q_func();

	while( y < r.height() && tmpRow >= 0 )
	{
		y += q->rowHeightForWidth( tmpRow, width ) + spacing;
		--tmpRow;
	}

	if( row )
		*row = tmpRow + 1;

	if( y > r.height() )
		return r.height() - y - 1 - spacing;
	else
		return 0;
}

template< typename T >
inline
int
AbstractListViewPrivate< T >::calculateScroll( int row,
	int expectedOffset ) const
{
	const int width = viewport->width() - spacing * 2;

	int delta = - offset + expectedOffset;

	int tmpRow = firstVisibleRow;

	const AbstractListView< T > * q = q_func();

	if( tmpRow > row )
	{
		--tmpRow;

		while( tmpRow >= row )
		{
			delta += spacing + q->rowHeightForWidth( tmpRow, width );
			--tmpRow;
		}
	}
	else if( tmpRow < row )
	{
		while( tmpRow < row )
		{
			delta -= ( q->rowHeightForWidth( tmpRow, width ) + spacing );
			++tmpRow;
		}
	}

	return -delta;
}

template< typename T >
inline
bool
AbstractListViewPrivate< T >::canScrollDown( int row ) const
{
	if( row == -1 )
		return false;

	if( !model )
		return false;

	int tmpRow = 0;

	maxOffsetAndFirstVisibleRow( &tmpRow );

	if( row < tmpRow )
		return true;
	else
		return false;
}

template< typename T >
inline
void
AbstractListViewPrivate< T >::normalizeOffset( int & row, int & offset )
{
	AbstractListView< T > * q = q_func();

	if( offset > 0 )
	{
		if( row > 0 )
		{
			const int width = viewport->rect().width() - spacing * 2;

			while( offset > 0 )
			{
				if( row != 0 )
					--row;
				else
				{
					offset = 0;
					break;
				}

				const int delta = q->rowHeightForWidth( row,
					width ) + spacing;
				offset -= delta;
			}
		}
		else
			offset = 0;
	}
	else if( offset < 0 )
	{
		if( canScrollDown( row ) )
		{
			const int width = viewport->rect().width() - spacing * 2;
			int height = q->rowHeightForWidth( row, width );

			while( qAbs( offset ) > height + spacing )
			{
				const int delta = height + spacing;
				offset += delta;

				if( model && row < model->rowCount() - 1 )
				{
					++row;
					height = q->rowHeightForWidth( row, width );
				}
				else
				{
					offset = 0;
					break;
				}
			}
		}
		else
		{
			const int max = maxOffsetAndFirstVisibleRow();

			if( offset < max )
				offset = max;
		}
	}
}

template< typename T >
inline
QSize
AbstractListViewPrivate< T >::calcScrolledAreaSize() const
{
	const AbstractListView< T > * q = q_func();

	const int width = viewport->rect().width();
	const int rowWidth = width - spacing * 2;
	int height = spacing;

	if( model )
		for( int i = 0, last = model->rowCount(); i < last; ++i )
			height += q->rowHeightForWidth( i, rowWidth ) + spacing;

	return QSize( width, height );
}

template< typename T >
inline
bool
AbstractListViewPrivate< T >::updateIfNeeded( int firstRow, int lastRow )
{
	AbstractListView< T > * q = q_func();

	for( int i = firstRow; i <= lastRow; ++i )
	{
		if( !q->visualRect( i ).isNull() )
		{
			viewport->update();

			return true;
		}
	}

	return false;
}

template< typename T >
inline
void
AbstractListViewPrivate< T >::init()
{
	AbstractListView< T > * q = q_func();

	Private::Viewport< T > * viewport = new Private::Viewport< T >( q );
	viewport->setData( this );

	timer = new QTimer( q );

	q->setViewport( viewport );

	QObject::connect( timer, &QTimer::timeout,
		q, &AbstractListView< T >::timerElapsed );
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
