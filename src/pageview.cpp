
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
#include "pageview.hpp"
#include "pagecontrol.hpp"
#include "fingergeometry.hpp"

// Qt include.
#include <QList>
#include <QResizeEvent>


namespace QtMWidgets {

//
// PageViewPrivate
//

class PageViewPrivate {
public:
	PageViewPrivate( PageView * parent )
		:	q( parent )
		,	viewport( 0 )
		,	control( 0 )
		,	showPageControl( true )
		,	controlOffset( 0 )
		,	leftButtonPressed( false )
		,	pagesPrepared( false )
		,	pagesOffset( 0 )
	{
		init();
	}

	//! Init.
	void init();
	//! Prepare widget.
	void prepareWidget( QWidget * widget, const QRect & r );
	//! Show page.
	void showPage( int index );
	//! Relayout children.
	void relayoutChildren( const QRect & r );
	//! Layout page.
	void layoutPage( QWidget * widget, const QRect & r );
	//! Layout page control.
	void layoutControl( const QRect & r );
	//! Prepare pages for moving.
	void preparePages( const QRect & r );
	//! Invalidate pages.
	void invalidatePages( const QRect & r );
	//! Move page left.
	void movePageLeft( int delta );
	//! Move page right.
	void movePageRight( int delta );
	//! Move pages.
	void movePages();
	//! Normalize page position.
	void normalizePagePos();
	//! Double in right.
	void doubleInRight( int delta );
	//! Double in left.
	void doubleInLeft( int delta );

	//! Parent.
	PageView * q;
	//! Viewport.
	QWidget * viewport;
	//! Page control.
	PageControl * control;
	//! Is PageControl visible?
	bool showPageControl;
	//! Pages.
	QList< QWidget * > pages;
	//! Bottom offset for the control.
	int controlOffset;
	//! Left mouse button pressed.
	bool leftButtonPressed;
	//! Current cursor position.
	QPoint pos;
	//! Is pages prepared for moving?
	bool pagesPrepared;
	//! Current offset for pages.
	int pagesOffset;
}; // class PageViewPrivate

void
PageViewPrivate::init()
{
	viewport = new QWidget( q );

	control = new PageControl( viewport );

	controlOffset = FingerGeometry::height() / 2;
}

void
PageViewPrivate::prepareWidget( QWidget * widget, const QRect & r )
{
	if( !widget )
		return;

	if( widget->parent() != viewport )
		widget->setParent( viewport );

	layoutPage( widget, r );
}

void
PageViewPrivate::showPage( int index )
{
	pages.at( index )->raise();
	pages.at( index )->show();

	control->raise();
}

void
PageViewPrivate::relayoutChildren( const QRect & r )
{
	viewport->resize( r.size() );

	viewport->move( r.topLeft() );

	for( int i = 0, iMax = pages.count(); i < iMax; ++i )
		layoutPage( pages.at( i ), r );

	layoutControl( r );
}

void
PageViewPrivate::layoutPage( QWidget * widget, const QRect & r )
{
	widget->resize( r.size() );

	widget->move( r.topLeft() );
}

void
PageViewPrivate::layoutControl( const QRect & r )
{
	const QSize controlSize = QSize( r.width(),
		control->heightForWidth( r.width() ) );

	const int y = r.bottomLeft().y() - controlOffset - controlSize.height();

	control->resize( controlSize );

	control->move( r.topLeft().x(), y );
}

void
PageViewPrivate::preparePages( const QRect & r )
{
	const int index = control->currentIndex();

	if( index != 0 )
	{
		QWidget * w = pages.at( index - 1 );

		QRect left = r;
		left.moveLeft( r.x() - r.width() );

		w->move( left.topLeft() );

		w->show();
	}

	if( index < control->count() - 1 )
	{
		QWidget * w = pages.at( index + 1 );

		QRect right = r;
		right.moveRight( r.x() + r.width() );

		w->move( right.topLeft() );

		w->show();
	}

	control->raise();

	pagesPrepared = true;
}

void
PageViewPrivate::invalidatePages( const QRect & r )
{
	const int index = control->currentIndex();

	if( index != 0 )
	{
		QWidget * w = pages.at( index - 1 );

		w->move( r.topLeft() );

		w->hide();
	}

	if( index < control->count() - 1 )
	{
		QWidget * w = pages.at( index + 1 );

		w->move( r.topLeft() );

		w->hide();
	}

	pages.at( index )->move( r.topLeft() );

	control->raise();

	pagesPrepared = false;

	pagesOffset = 0;
}

void
PageViewPrivate::movePageLeft( int delta )
{
	if( control->currentIndex() == control->count() - 1 )
		doubleInRight( delta );
	else
	{
		if( !pagesPrepared )
			preparePages( viewport->rect() );

		pagesOffset -= delta;

		if( control->currentIndex() < control->count() - 1 )
		{
			if( qAbs( pagesOffset ) < viewport->width() )
				movePages();
			else
			{
				invalidatePages( viewport->rect() );

				control->setCurrentIndex( control->currentIndex() + 1 );
			}
		}
	}
}

void
PageViewPrivate::movePageRight( int delta )
{
	if( control->currentIndex() == 0 )
		doubleInLeft( delta );
	else
	{
		if( !pagesPrepared )
			preparePages( viewport->rect() );

		pagesOffset += delta;

		if( control->currentIndex() != 0 )
		{
			if( qAbs( pagesOffset ) < viewport->width() )
				movePages();
			else
			{
				invalidatePages( viewport->rect() );

				control->setCurrentIndex( control->currentIndex() - 1 );
			}
		}
	}
}

void
PageViewPrivate::movePages()
{
	const int index = control->currentIndex();

	if( index != 0 )
	{
		const QPoint p = viewport->rect().topLeft() -
			QPoint( viewport->width(), 0 ) + QPoint( pagesOffset, 0 );

		pages.at( index - 1 )->move( p );
	}

	if( index < control->count() - 1 )
	{
		const QPoint p = viewport->rect().topLeft() +
			QPoint( viewport->width(), 0 ) + QPoint( pagesOffset, 0 );

		pages.at( index + 1 )->move( p );
	}

	const QPoint p = viewport->rect().topLeft() +
		QPoint( pagesOffset, 0 );

	pages.at( index )->move( p );
}

void
PageViewPrivate::normalizePagePos()
{
	int index = -1;

	if( qAbs( pagesOffset ) > viewport->width() / 2 )
	{
		if( pagesOffset < 0 && control->currentIndex() < control->count() - 1 )
			index = control->currentIndex() + 1;
		else if( pagesOffset > 0 && control->currentIndex() != 0 )
			index = control->currentIndex() - 1;
	}

	invalidatePages( viewport->rect() );

	if( index != -1 )
		control->setCurrentIndex( index );
}

void
PageViewPrivate::doubleInRight( int delta )
{
	Q_UNUSED( delta )
}

void
PageViewPrivate::doubleInLeft( int delta )
{
	Q_UNUSED( delta )
}


//
// PageView
//

PageView::PageView( QWidget * parent )
	:	QFrame( parent )
	,	d( new PageViewPrivate( this ) )
{
	connect( d->control, &PageControl::currentChanged,
		this, &PageView::_q_currentIndexChanged );

	d->relayoutChildren( frameRect().adjusted( frameWidth(), frameWidth(),
		-frameWidth(), -frameWidth() ) );

	d->control->raise();

	d->control->show();
}

PageView::~PageView()
{
}

int
PageView::addWidget( QWidget * widget )
{
	return insertWidget( d->pages.count(), widget );
}

int
PageView::count() const
{
	return d->control->count();
}

int
PageView::currentIndex() const
{
	return d->control->currentIndex();
}

QWidget *
PageView::currentWidget() const
{
	if( currentIndex() != -1 )
		return d->pages.at( currentIndex() );
	else
		return 0;
}

int
PageView::indexOf( QWidget * widget ) const
{
	for( int i = 0; i < d->pages.count(); ++i )
	{
		if( d->pages.at( i ) == widget )
			return i;
	}

	return -1;
}

int
PageView::insertWidget( int index, QWidget * widget )
{
	index = qMin( index, d->pages.count() );

	if( index < 0 )
		index = d->pages.count();

	d->pages.insert( index, widget );

	d->prepareWidget( widget, frameRect().adjusted( frameWidth(), frameWidth(),
		-frameWidth(), -frameWidth() ) );

	widget->hide();

	d->control->setCount( d->pages.count() );

	if ( currentIndex() < 0 )
		setCurrentIndex( index );

	return index;

}

void
PageView::removeWidget( QWidget * widget )
{
	const int index  = indexOf( widget );

	if( index != -1 )
	{
		d->pages.removeAt( index );

		widget->setParent( 0 );

		widget->hide();

		const bool changePage = index == currentIndex();

		d->control->setCount( d->pages.count() );

		if( changePage && currentIndex() != -1 )
			d->showPage( currentIndex() );
	}
}

QWidget *
PageView::widget( int index ) const
{
	if( index >= 0 && index < d->pages.count() )
		return d->pages.at( index );
	else
		return 0;
}

bool
PageView::showPageControl() const
{
	return d->showPageControl;
}

void
PageView::setShowPageControl( bool show )
{
	if( d->showPageControl != show )
	{
		d->showPageControl = show;

		if( d->showPageControl )
		{
			d->control->raise();
			d->control->show();
		}
		else
			d->control->hide();
	}
}

void
PageView::setCurrentIndex( int index )
{
	d->control->setCurrentIndex( index );
}

void
PageView::setCurrentWidget( QWidget * widget )
{
	const int index = indexOf( widget );

	if( index >= 0 )
		setCurrentIndex( index );
}

void
PageView::resizeEvent( QResizeEvent * e )
{
	d->relayoutChildren( frameRect().adjusted( frameWidth(), frameWidth(),
	   -frameWidth(), -frameWidth() ) );

	e->accept();
}

void
PageView::mousePressEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
	{
		d->leftButtonPressed = true;

		d->pos = e->pos();
	}
}

void
PageView::mouseMoveEvent( QMouseEvent * e )
{
	if( d->leftButtonPressed )
	{
		const int delta = ( e->pos() - d->pos ).x();

		d->pos = e->pos();

		if( delta > 0 )
			d->movePageRight( delta );
		else if( delta < 0 )
			d->movePageLeft( qAbs( delta ) );
	}
}

void
PageView::mouseReleaseEvent( QMouseEvent * )
{
	d->leftButtonPressed = false;

	d->normalizePagePos();
}

void
PageView::_q_currentIndexChanged( int index, int prev )
{
	if( prev >= 0 && prev < d->pages.count() )
		d->pages.at( prev )->hide();

	if( index >= 0 && index < d->pages.count() )
		d->showPage( index );
}

} /* namespace QtMWidgets */
