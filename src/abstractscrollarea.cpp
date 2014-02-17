
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
#include "private/abstractscrollarea_p.hpp"

// Qt include.
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QTimer>


namespace QtMWidgets {

//
// ScrollIndicator
//

ScrollIndicator::ScrollIndicator( const QColor & c, Qt::Orientation o,
	QWidget * parent )
	:	QWidget( parent )
	,	policy( AbstractScrollArea::ScrollIndicatorAsNeeded )
	,	minimumSize( 10 )
	,	size( minimumSize )
	,	width( 3 )
	,	orientation( o )
	,	needPaint( false )
	,	color( c )
	,	animate( false )
	,	alpha( 255 )
	,	shown( false )
{
}

QSize
ScrollIndicator::minimumSizeHint() const
{
	if( orientation == Qt::Horizontal )
		return QSize( minimumSize, width );
	else
		return QSize( width, minimumSize );
}

QSize
ScrollIndicator::sizeHint() const
{
	if( orientation == Qt::Horizontal )
		return QSize( size, width );
	else
		return QSize( width, size );
}

void
ScrollIndicator::paintEvent( QPaintEvent * )
{
	QPainter p( this );

	switch( policy )
	{
		case AbstractScrollArea::ScrollIndicatorAsNeeded :
			if( !needPaint && !animate )
			{
				shown = false;
				break;
			}
		case AbstractScrollArea::ScrollIndicatorAlwaysOn :
			drawIndicator( &p, color );
		break;
	}
}

void
ScrollIndicator::drawIndicator( QPainter * p, const QColor & c )
{
	if( policy != AbstractScrollArea::ScrollIndicatorAlwaysOff )
	{
		shown = true;

		QColor paintColor = c;

		if( animate && policy != AbstractScrollArea::ScrollIndicatorAlwaysOn )
			paintColor.setAlpha( alpha );

		p->setPen( QPen( paintColor, width, Qt::SolidLine, Qt::RoundCap ) );

		const int middle = width / 2;

		if( orientation == Qt::Horizontal )
			p->drawLine( 0, middle, size, middle );
		else
			p->drawLine( middle, 0, middle, size );
	}
}


//
// AbstractScrollAreaPrivate
//

void
AbstractScrollAreaPrivate::init()
{
	QStyleOption opt;
	opt.initFrom( q );

	const QColor ic = opt.palette.color( QPalette::Highlight );

	viewport = new QWidget( q );
	viewport->setObjectName( QLatin1String( "qt_scrollarea_viewport" ) );
	viewport->setBackgroundRole( QPalette::Base );
	viewport->setAutoFillBackground( true );
	viewport->setFocusProxy( q );

	horIndicator = new ScrollIndicator( ic, Qt::Horizontal, viewport );
	vertIndicator = new ScrollIndicator( ic, Qt::Vertical, viewport );

	animationTimer = new QTimer( q );

	QObject::connect( animationTimer, SIGNAL( timeout() ),
		q, SLOT( _q_animateScrollIndicators() ) );

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
AbstractScrollAreaPrivate::calcIndicators()
{
	if( scrolledAreaSize.isValid() && scrolledAreaSize.width() != 0
		&& scrolledAreaSize.height() != 0 )
	{
		calcIndicator( Qt::Horizontal, horIndicator->minimumSize,
			horIndicator->width, horIndicator->needPaint,
			horIndicator->size, horIndicator->pos );

		horIndicator->move( horIndicator->pos );
		horIndicator->resize( horIndicator->size, horIndicator->width );

		calcIndicator( Qt::Vertical, vertIndicator->minimumSize,
			vertIndicator->width, vertIndicator->needPaint,
			vertIndicator->size, vertIndicator->pos );

		vertIndicator->move( vertIndicator->pos );
		vertIndicator->resize( vertIndicator->width, vertIndicator->size );
	}
}

void
AbstractScrollAreaPrivate::calcIndicator( Qt::Orientation orient,
	int minSize, int width, bool & needPaint,
	int & indicatorSize, QPoint & indicatorPos )
{
	int viewportSize = 0;
	int scrolledSize = 0;
	int x = 0;
	int y = 0;
	double posRatio = 0.0;
	int totalIndicatorSize = 0;
	double ratio = 0.0;
	double deltaRatio = 0.0;

	switch( orient )
	{
		case Qt::Horizontal :
		{
			viewportSize = viewport->width();
			scrolledSize = scrolledAreaSize.width();
			x = 2 * width;
			y = qMin( viewport->height(), scrolledAreaSize.height() ) - x;
			y += ( horIndicator->parent() == viewport ? 0 : topLeftCorner.y() );
			posRatio = (double) topLeftCorner.x() / (double) scrolledSize;
			totalIndicatorSize = qMin( viewportSize, scrolledSize ) - 4 * width;
			ratio = (double) viewportSize / (double) scrolledSize;
			deltaRatio = posRatio;

			if( ratio < 1.0 )
			{
				indicatorSize = (double) totalIndicatorSize * ratio;

				if( indicatorSize < minSize )
				{
					deltaRatio *= ( (double) indicatorSize / (double) minSize );
					indicatorSize = minSize;
				}

				x += totalIndicatorSize * deltaRatio
					+ ( horIndicator->parent() == viewport ? 0 : scrolledSize )
					* posRatio;
			}
			else
			{
				needPaint = false;
				indicatorSize = totalIndicatorSize;
			}

			indicatorPos = QPoint( x, y );
		}
		break;

		case Qt::Vertical :
		{
			viewportSize = viewport->height();
			scrolledSize = scrolledAreaSize.height();
			y = 2 * width;
			x = qMin( viewport->width(), scrolledAreaSize.width() ) - y;
			x += ( vertIndicator->parent() == viewport ? 0 : topLeftCorner.x() );
			posRatio = (double) topLeftCorner.y() / (double) scrolledSize;
			totalIndicatorSize = qMin( viewportSize, scrolledSize ) - 4 * width;
			ratio = (double) viewportSize / (double) scrolledSize;
			deltaRatio = posRatio;

			if( ratio < 1.0 )
			{
				indicatorSize = (double) totalIndicatorSize * ratio;

				if( indicatorSize < minSize )
				{
					deltaRatio *= ( (double) indicatorSize / (double) minSize );
					indicatorSize = minSize;
				}

				y += totalIndicatorSize * deltaRatio
					+ ( vertIndicator->parent() == viewport ? 0 : scrolledSize )
					* posRatio;
			}
			else
			{
				needPaint = false;
				indicatorSize = totalIndicatorSize;
			}

			indicatorPos = QPoint( x, y );
		}
		break;
	}
}

void
AbstractScrollAreaPrivate::scrollContentsBy( int dx, int dy )
{
	topLeftCorner -= QPoint( dx, dy );

	normalizePosition();

	if( dx != 0 )
	{
		horIndicator->shown = true;
		horIndicator->needPaint = true;
		horIndicator->raise();
	}

	if( dy != 0 )
	{
		vertIndicator->shown = true;
		vertIndicator->needPaint = true;
		vertIndicator->raise();
	}

	calcIndicators();

	q->update();
	horIndicator->update();
	vertIndicator->update();
}

void
AbstractScrollAreaPrivate::animateScrollIndicators()
{
	animationTimer->stop();
	horIndicator->alpha = horIndicator->color.alpha();
	vertIndicator->alpha = vertIndicator->color.alpha();
	animationTimer->start( animationTimeout );

	if( horIndicator->shown )
	{
		horIndicator->animate = true;
		horIndicator->update();
	}

	if( vertIndicator->shown )
	{
		vertIndicator->animate = true;
		vertIndicator->update();
	}
}

void
AbstractScrollAreaPrivate::stopScrollIndicatorsAnimation()
{
	animationTimer->stop();
	horIndicator->needPaint = false;
	vertIndicator->needPaint = false;
	horIndicator->animate = false;
	vertIndicator->animate = false;
	horIndicator->shown = false;
	vertIndicator->shown = false;
	horIndicator->update();
	vertIndicator->update();
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
		d->horIndicator->setParent( d->viewport );
		d->vertIndicator->setParent( d->viewport );

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
	return d->horIndicator->color;
}

void
AbstractScrollArea::setIndicatorColor( const QColor & c )
{
	if( d->horIndicator->color != c )
	{
		d->horIndicator->color = c;
		d->vertIndicator->color = c;
	}
}

AbstractScrollArea::ScrollIndicatorPolicy
AbstractScrollArea::verticalScrollIndicatorPolicy() const
{
	return d->vertIndicator->policy;
}

void
AbstractScrollArea::setVerticalScrollIndicatorPolicy(
	ScrollIndicatorPolicy policy )
{
	d->vertIndicator->policy = policy;

	d->vertIndicator->update();
}


AbstractScrollArea::ScrollIndicatorPolicy
AbstractScrollArea::horizontalScrollIndicatorPolicy() const
{
	return d->horIndicator->policy;
}

void
AbstractScrollArea::setHorizontalScrollIndicatorPolicy(
	AbstractScrollArea::ScrollIndicatorPolicy policy )
{
	d->horIndicator->policy = policy;

	d->horIndicator->update();
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
	d->normalizePosition();
	d->calcIndicators();

	update();
	d->horIndicator->update();
	d->vertIndicator->update();
}

void
AbstractScrollArea::setViewportMargins( const QMargins & margins )
{
	setViewportMargins( margins.left(), margins.top(),
		margins.right(), margins.bottom() );
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

	d->calcIndicators();

	update();
	d->horIndicator->update();
	d->vertIndicator->update();
}

const QPoint &
AbstractScrollArea::topLeftPointShownArea() const
{
	return d->topLeftCorner;
}

void
AbstractScrollArea::setTopLeftPointShownArea( const QPoint & p )
{	
	const int dx = p.x() - d->topLeftCorner.x();
	const int dy = p.y() - d->topLeftCorner.y();

	d->scrollContentsBy( dx, dy );
	scrollContentsBy( dx, dy );
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
AbstractScrollArea::mousePressEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
	{
		d->mousePos = e->pos();
		d->leftMouseButtonPressed = true;
		d->stopScrollIndicatorsAnimation();
	}

	e->accept();
}

void
AbstractScrollArea::mouseReleaseEvent( QMouseEvent * e )
{
	d->leftMouseButtonPressed = false;
	d->horIndicator->needPaint = false;
	d->vertIndicator->needPaint = false;

	if( ( d->horIndicator->shown || d->vertIndicator->shown ) &&
		( d->horIndicator->policy == ScrollIndicatorAsNeeded ||
			d->vertIndicator->policy == ScrollIndicatorAsNeeded ) )
				d->animateScrollIndicators();
	else
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

		d->scrollContentsBy( dx, dy );

		scrollContentsBy( dx, dy );
	}

	e->accept();
}

void
AbstractScrollArea::wheelEvent( QWheelEvent * e )
{
	QPoint numPixels = e->pixelDelta();
	QPoint numDegrees = e->angleDelta();

	d->stopScrollIndicatorsAnimation();

	if( !numPixels.isNull() )
	{
		if( e->modifiers() == Qt::ShiftModifier )
		{
			d->scrollContentsBy( numPixels.y(), 0 );
			scrollContentsBy( numPixels.y(), 0 );
		}
		else
		{
			d->scrollContentsBy( 0, numPixels.y() );
			scrollContentsBy( 0, numPixels.y() );
		}
	}
	else if( !numDegrees.isNull() )
	{
		if( e->modifiers() == Qt::ShiftModifier )
		{
			d->scrollContentsBy( numDegrees.y() / 8, 0 );
			scrollContentsBy( numDegrees.y() / 8, 0 );
		}
		else
		{
			d->scrollContentsBy( 0, numDegrees.y() / 8 );
			scrollContentsBy( 0, numDegrees.y() / 8 );
		}
	}

	e->accept();

	if( d->horIndicator->policy == ScrollIndicatorAsNeeded ||
		d->vertIndicator->policy == ScrollIndicatorAsNeeded )
			d->animateScrollIndicators();
}

void
AbstractScrollArea::_q_animateScrollIndicators()
{
	d->horIndicator->alpha -= d->animationAlphaDelta;
	d->vertIndicator->alpha -= d->animationAlphaDelta;

	if( d->horIndicator->alpha <= 0 )
		d->stopScrollIndicatorsAnimation();
	else if( d->horIndicator->shown || d->vertIndicator->shown )
		d->animationTimer->start( d->animationTimeout );

	if( d->horIndicator->shown )
		d->horIndicator->update();

	if( d->vertIndicator->shown )
		d->vertIndicator->update();
}

} /* namespace QtMWidgets */
