
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// QtMWidgets include.
#include "pageview.hpp"
#include "pagecontrol.hpp"
#include "fingergeometry.hpp"

// Qt include.
#include <QList>
#include <QResizeEvent>
#include <QVariantAnimation>


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
		,	normalizeAnimation( 0 )
		,	indexAfterNormalizeAnimation( -1 )
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
	//! Normalize animation.
	QVariantAnimation * normalizeAnimation;
	//! Index after normalize animation.
	int indexAfterNormalizeAnimation;
}; // class PageViewPrivate

void
PageViewPrivate::init()
{
	viewport = new QWidget( q );

	control = new PageControl( viewport );

	controlOffset = FingerGeometry::height() / 2;

	normalizeAnimation = new QVariantAnimation( q );

	normalizeAnimation->setDuration( 300 );

	normalizeAnimation->setLoopCount( 1 );
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

		const QRect right( r.x() + r.width(), r.y(), r.width(), r.height() );

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
	if( !pagesPrepared )
		preparePages( viewport->rect() );

	if( control->currentIndex() < control->count() - 1 || pagesOffset > 0 )
	{
		pagesOffset -= delta;

		if( qAbs( pagesOffset ) < viewport->width() )
			movePages();
		else
		{
			invalidatePages( viewport->rect() );

			control->setCurrentIndex( control->currentIndex() + 1 );
		}
	}
}

void
PageViewPrivate::movePageRight( int delta )
{
	if( !pagesPrepared )
		preparePages( viewport->rect() );

	if( control->currentIndex() != 0 || pagesOffset < 0 )
	{
		pagesOffset += delta;

		if( qAbs( pagesOffset ) < viewport->width() )
			movePages();
		else
		{
			invalidatePages( viewport->rect() );

			control->setCurrentIndex( control->currentIndex() - 1 );
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
	indexAfterNormalizeAnimation = -1;

	int endPos = 0;

	if( qAbs( pagesOffset ) > viewport->width() / 2 )
	{
		if( pagesOffset < 0 && control->currentIndex() < control->count() - 1 )
		{
			indexAfterNormalizeAnimation = control->currentIndex() + 1;

			endPos = - viewport->width();
		}
		else if( pagesOffset > 0 && control->currentIndex() != 0 )
		{
			indexAfterNormalizeAnimation = control->currentIndex() - 1;

			endPos = viewport->width();
		}
	}

	normalizeAnimation->setStartValue( pagesOffset );
	normalizeAnimation->setEndValue( endPos );

	normalizeAnimation->start();
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

	connect( d->normalizeAnimation, &QVariantAnimation::valueChanged,
		this, &PageView::_q_normalizePageAnimation );

	connect( d->normalizeAnimation, &QVariantAnimation::finished,
		this, &PageView::_q_normalizeAnimationFinished );

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

		d->normalizeAnimation->stop();
	}

	e->ignore();
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

	e->ignore();
}

void
PageView::mouseReleaseEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
	{
		d->leftButtonPressed = false;

		d->normalizePagePos();
	}

	e->ignore();
}

void
PageView::_q_currentIndexChanged( int index, int prev )
{
	if( prev >= 0 && prev < d->pages.count() )
		d->pages.at( prev )->hide();

	if( index >= 0 && index < d->pages.count() )
		d->showPage( index );
}

void
PageView::_q_normalizePageAnimation( const QVariant & v )
{
	d->pagesOffset = v.toInt();

	d->movePages();
}

void
PageView::_q_normalizeAnimationFinished()
{
	d->invalidatePages( d->viewport->rect() );

	if( d->indexAfterNormalizeAnimation != -1 )
		d->control->setCurrentIndex( d->indexAfterNormalizeAnimation );
}

} /* namespace QtMWidgets */
