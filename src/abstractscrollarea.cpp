
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
#include "abstractscrollarea.hpp"

// Qt include.
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWheelEvent>


namespace QtMWidgets {

//
// AbstractScrollAreaPrivate
//

class AbstractScrollAreaPrivate {
public:
	explicit AbstractScrollAreaPrivate( AbstractScrollArea * parent )
		:	q( parent )
		,	viewport( 0 )
		,	scrolledAreaSize( 0, 0 )
		,	topLeftCorner( 0, 0 )
		,	top( 0 )
		,	bottom( 0 )
		,	right( 0 )
		,	left( 0 )
		,	verticalScrollIndicatorPolicy(
				AbstractScrollArea::ScrollIndicatorAsNeeded )
		,	horizontalScrollIndicatorPolicy(
				AbstractScrollArea::ScrollIndicatorAsNeeded )
		,	paintVerticalScrollIndicator( false )
		,	paintHorizontalScrollIndicator( false )
		,	leftMouseButtonPressed( false )
		,	hIndicatorSize( 0 )
		,	hIndicatorPos( 0 )
		,	vIndicatorSize( 0 )
		,	vIndicatorPos( 0 )
		,	indicatorWidth( 3 )
	{
	}

	void init();
	void layoutChildren( const QStyleOption & opt );
	void normalizePosition();
	void drawHorizontalScrollIndicator( QPainter * p );
	void drawVerticalScrollIndicator( QPainter * p );
	void calcIndicators();
	void calcIndicator( int scrolledSize, int scrolledPos,
		int viewportSize, int & indicatorSize,
		int & indicatorPos );

	virtual ~AbstractScrollAreaPrivate()
	{
	}

	AbstractScrollArea * q;
	QColor indicatorColor;
	QWidget * viewport;
	QSize scrolledAreaSize;
	QPoint topLeftCorner;
	int top;
	int bottom;
	int right;
	int left;
	AbstractScrollArea::ScrollIndicatorPolicy verticalScrollIndicatorPolicy;
	AbstractScrollArea::ScrollIndicatorPolicy horizontalScrollIndicatorPolicy;
	bool paintVerticalScrollIndicator;
	bool paintHorizontalScrollIndicator;
	bool leftMouseButtonPressed;
	QPoint mousePos;
	int hIndicatorSize;
	int hIndicatorPos;
	int vIndicatorSize;
	int vIndicatorPos;
	int indicatorWidth;
}; // class AbstractScrollAreaPrivate

void
AbstractScrollAreaPrivate::init()
{
	QStyleOption opt;
	opt.initFrom( q );

	indicatorColor = opt.palette.color( QPalette::Highlight );

	viewport = new QWidget( q );
	viewport->setObjectName( QLatin1String( "qt_scrollarea_viewport" ) );
	viewport->setBackgroundRole( QPalette::Base );
	viewport->setAutoFillBackground( true );
	viewport->setFocusProxy( q );

	q->setFocusPolicy( Qt::WheelFocus );
	q->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	q->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

	layoutChildren( opt );
}

void
AbstractScrollAreaPrivate::layoutChildren( const QStyleOption & opt )
{
	QRect viewportRect = q->contentsRect();

	if( q->isRightToLeft() )
		viewportRect.adjust( right, top, -left, -bottom );
	else
		viewportRect.adjust( left, top, -right, -bottom );

	viewport->setGeometry( QStyle::visualRect( opt.direction, opt.rect,
		viewportRect ) );
}

void
AbstractScrollAreaPrivate::normalizePosition()
{
	if( topLeftCorner.x() < 0 )
		topLeftCorner.setX( 0 );

	if( topLeftCorner.y() < 0 )
		topLeftCorner.setY( 0 );

	const QSize s = viewport->size();

	const QPoint maxPos = QPoint( scrolledAreaSize.width() - s.width(),
		scrolledAreaSize.height() - s.height() );

	if( scrolledAreaSize.width() > s.width() )
	{
		if( maxPos.x() < topLeftCorner.x() )
			topLeftCorner.setX( maxPos.x() );
	}
	else
		topLeftCorner.setX( 0 );

	if( scrolledAreaSize.height() > s.height() )
	{
		if( maxPos.y() < topLeftCorner.y() )
			topLeftCorner.setY( maxPos.y() );
	}
	else
		topLeftCorner.setY( 0 );
}

void
AbstractScrollAreaPrivate::drawHorizontalScrollIndicator( QPainter * p )
{
	p->setPen( QPen( indicatorColor, indicatorWidth,
		Qt::SolidLine, Qt::RoundCap ) );

	const int x = indicatorWidth + hIndicatorPos;
	const int y = viewport->height() - indicatorWidth;

	p->drawLine( x, y, x + hIndicatorSize, y );
}

void
AbstractScrollAreaPrivate::drawVerticalScrollIndicator( QPainter * p )
{
	p->setPen( QPen( indicatorColor, indicatorWidth,
		Qt::SolidLine, Qt::RoundCap ) );

	const int x = viewport->width() - indicatorWidth;
	const int y = indicatorWidth + vIndicatorPos;

	p->drawLine( x, y, x, y + vIndicatorSize );
}

void
AbstractScrollAreaPrivate::calcIndicators()
{
	if( scrolledAreaSize.isValid() && scrolledAreaSize.width() != 0
		&& scrolledAreaSize.height() != 0 )
	{
		calcIndicator( scrolledAreaSize.width(), topLeftCorner.x(),
			viewport->width(), hIndicatorSize, hIndicatorPos );

		calcIndicator( scrolledAreaSize.height(), topLeftCorner.y(),
			viewport->height(), vIndicatorSize, vIndicatorPos );
	}
}

void
AbstractScrollAreaPrivate::calcIndicator( int scrolledSize, int scrolledPos,
	int viewportSize, int & indicatorSize, int & indicatorPos )
{
	const int totalIndicatorSize = viewportSize - 4 * indicatorWidth;
	const double ratio = (double) viewportSize / (double) scrolledSize;

	if( ratio < 1.0 )
	{
		indicatorSize = (double) totalIndicatorSize * ratio;

		const double posRatio = (double) scrolledPos / (double) scrolledSize;

		indicatorPos = (double) totalIndicatorSize * posRatio;
	}
	else
	{
		indicatorSize = totalIndicatorSize;
		indicatorPos = 0;
	}
}


//
// AbstractScrollArea
//

AbstractScrollArea::AbstractScrollArea( QWidget * parent )
	:	QFrame( parent )
	,	d( new AbstractScrollAreaPrivate( this ) )
{
	d->init();
}

AbstractScrollArea::AbstractScrollArea( AbstractScrollAreaPrivate * dd,
	QWidget * parent )
	:	QFrame( parent )
	,	d( dd )
{
	d->init();
}

AbstractScrollArea::~AbstractScrollArea()
{
}

QWidget *
AbstractScrollArea::viewport() const
{
	return d->viewport;
}

void
AbstractScrollArea::setViewport( QWidget * widget )
{
	if( widget != d->viewport )
	{
		QWidget *oldViewport = d->viewport;

		if( !widget )
			widget = new QWidget;

		d->viewport = widget;
		d->viewport->setParent( this );
		d->viewport->setFocusProxy( this );

		QStyleOption opt;
		opt.initFrom( this );
		d->layoutChildren( opt );

		if( isVisible() )
			d->viewport->show();

		setupViewport( widget );

		delete oldViewport;
	}
}

void
AbstractScrollArea::setupViewport( QWidget * )
{
}

QColor
AbstractScrollArea::indicatorColor() const
{
	return d->indicatorColor;
}

void
AbstractScrollArea::setIndicatorColor( const QColor & c )
{
	if( d->indicatorColor != c )
	{
		d->indicatorColor = c;
	}
}

AbstractScrollArea::ScrollIndicatorPolicy
AbstractScrollArea::verticalScrollIndicatorPolicy() const
{
	return d->verticalScrollIndicatorPolicy;
}

void
AbstractScrollArea::setVerticalScrollIndicatorPolicy(
	ScrollIndicatorPolicy policy )
{
	d->verticalScrollIndicatorPolicy = policy;

	update();
}


AbstractScrollArea::ScrollIndicatorPolicy
AbstractScrollArea::horizontalScrollIndicatorPolicy() const
{
	return d->horizontalScrollIndicatorPolicy;
}

void
AbstractScrollArea::setHorizontalScrollIndicatorPolicy(
	AbstractScrollArea::ScrollIndicatorPolicy policy )
{
	d->horizontalScrollIndicatorPolicy = policy;

	update();
}

QSize
AbstractScrollArea::minimumSizeHint() const
{
	QSize viewportSizeHint = d->viewport->minimumSizeHint();
	const QSize marginsSize = QSize( d->left + d->right, d->top + d->bottom );
	const QSize frameSize = QSize( frameWidth() * 2, frameWidth() * 2 );

	if( !viewportSizeHint.isValid() )
	{
		const int h = qMax( 10, fontMetrics().height() );
		viewportSizeHint = QSize( 6 * h, 4 * h );
	}

	return viewportSizeHint + marginsSize + frameSize;
}

QSize
AbstractScrollArea::sizeHint() const
{
	QSize viewportSizeHint = d->viewport->sizeHint();
	const QSize marginsSize = QSize( d->left + d->right, d->top + d->bottom );
	const QSize frameSize = QSize( frameWidth() * 2, frameWidth() * 2 );

	if( !viewportSizeHint.isValid() )
	{
		const int h = qMax( 10, fontMetrics().height() );
		viewportSizeHint = QSize( 6 * h, 4 * h );
	}

	return viewportSizeHint + marginsSize + frameSize;
}

void
AbstractScrollArea::setViewportMargins( int left, int top,
	int right, int bottom )
{
	d->left = left;
	d->top = top;
	d->right = right;
	d->bottom = bottom;

	QStyleOption opt;
	opt.initFrom( this );
	d->layoutChildren( opt );
}

void
AbstractScrollArea::setViewportMargins( const QMargins & margins )
{
	setViewportMargins( margins.left(), margins.top(),
		margins.right(), margins.bottom() );
}

void
AbstractScrollArea::scrollContentsBy( int dx, int dy )
{
	d->topLeftCorner += QPoint( dx, dy );

	d->normalizePosition();

	d->calcIndicators();

	if( dx != 0 ) d->paintHorizontalScrollIndicator = true;

	if( dy != 0 ) d->paintVerticalScrollIndicator = true;

	update();
}

const QSize &
AbstractScrollArea::scrolledAreaSize() const
{
	return d->scrolledAreaSize;
}

void
AbstractScrollArea::setScrolledAreaSize( const QSize & s )
{
	d->scrolledAreaSize = s;

	d->normalizePosition();
}

const QPoint &
AbstractScrollArea::topLeftPointShownArea() const
{
	return d->topLeftCorner;
}

void
AbstractScrollArea::setTopLeftPointShownArea( const QPoint & p )
{
	d->topLeftCorner = p;

	d->normalizePosition();
}

void
AbstractScrollArea::resizeEvent( QResizeEvent * e )
{
	QStyleOption opt;
	opt.initFrom( this );

	d->layoutChildren( opt );

	d->normalizePosition();

	d->calcIndicators();

	update();

	e->accept();
}

void
AbstractScrollArea::paintEvent( QPaintEvent * e )
{
	QFrame::paintEvent( e );

	QPainter p( this );
	p.setViewport( d->viewport->rect() );

	switch( d->horizontalScrollIndicatorPolicy )
	{
		case ScrollIndicatorAsNeeded :
			if( !d->paintHorizontalScrollIndicator )
				break;
		case ScrollIndicatorAlwaysOn :
			d->drawHorizontalScrollIndicator( &p );
		break;
	}

	switch( d->verticalScrollIndicatorPolicy )
	{
		case ScrollIndicatorAsNeeded :
			if( !d->paintVerticalScrollIndicator )
				break;
		case ScrollIndicatorAlwaysOn :
			d->drawVerticalScrollIndicator( &p );
		break;
	}
}

void
AbstractScrollArea::mousePressEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
	{
		d->mousePos = e->pos();
		d->leftMouseButtonPressed = true;
	}

	e->accept();
}

void
AbstractScrollArea::mouseReleaseEvent( QMouseEvent * e )
{
	d->leftMouseButtonPressed = false;
	d->paintHorizontalScrollIndicator = false;
	d->paintVerticalScrollIndicator = false;

	update();

	e->accept();
}

void
AbstractScrollArea::mouseMoveEvent( QMouseEvent * e )
{
	if( d->leftMouseButtonPressed )
	{
		const int dy = e->pos().y() - d->mousePos.y();
		const int dx = e->pos().x() - d->mousePos.x();

		d->mousePos = e->pos();

		scrollContentsBy( dx, dy );
	}

	e->accept();
}

void
AbstractScrollArea::wheelEvent( QWheelEvent * e )
{
	QPoint numPixels = e->pixelDelta();
	QPoint numDegrees = e->angleDelta();

	if( !numPixels.isNull() )
	{
		if( e->modifiers() == Qt::ShiftModifier )
			scrollContentsBy( numPixels.x(), 0 );
		else
			scrollContentsBy( 0, numPixels.y() );
	}
	else if( !numDegrees.isNull() )
	{
		if( e->modifiers() == Qt::ShiftModifier )
			scrollContentsBy( numDegrees.x() / 8, 0 );
		else
			scrollContentsBy( 0, numDegrees.y() / 8 );
	}

	e->accept();
}

} /* namespace QtMWidgets */
