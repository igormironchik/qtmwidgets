
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
#include "scroller.hpp"
#include "fingergeometry.hpp"

// Qt include.
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QLinearGradient>
#include <QVariantAnimation>


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
			if( !needPaint )
				break;
			[[fallthrough]];
		case AbstractScrollArea::ScrollIndicatorAlwaysOn :
			drawIndicator( &p, color );
		break;

		default:
			break;
	}
}

void
ScrollIndicator::drawIndicator( QPainter * p, const QColor & c )
{
	if( policy != AbstractScrollArea::ScrollIndicatorAlwaysOff )
	{
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
// BlurEffect
//

BlurEffect::BlurEffect( const QColor & c, Qt::Orientation o, QWidget * parent )
	:	QWidget( parent )
	,	policy( AbstractScrollArea::BlurAlwaysOff )
	,	orientation( o )
	,	color( c )
	,	pressure( 0 )
	,	darkBlurWidth( 4 )
	,	maxPressure( 20.0 )
{
}

QSize
BlurEffect::minimumSizeHint() const
{
	static const QSize size( FingerGeometry::width() / 2,
		FingerGeometry::height() / 2 );

	return ( orientation == Qt::Horizontal ? size : size.transposed() );
}

QSize
BlurEffect::sizeHint() const
{
	return minimumSizeHint();
}

void
BlurEffect::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	p.setRenderHint( QPainter::Antialiasing );

	switch( policy )
	{
		case AbstractScrollArea::BlurAlwaysOff :
			return;

		case AbstractScrollArea::BlurHorizontalOnly :
		{
			if( orientation == Qt::Vertical )
				drawBlur( &p, color );
		}
			break;

		case AbstractScrollArea::BlurVerticalOnly :
		{
			if( orientation == Qt::Horizontal )
				drawBlur( &p, color );
		}
			break;

		case AbstractScrollArea::BlurBothDirections :
			drawBlur( &p, color );
			break;

		default :
			break;
	}
}

void
BlurEffect::drawBlur( QPainter * p, const QColor & c )
{
	const QRect r = rect();

	const qreal realPressure = qMin(
		(qreal) qAbs( pressure ) / maxPressure, 1.0 );

	QColor c1 = c;
	c1.setAlpha( 255 * realPressure );

	const QRect dark(
		( orientation == Qt::Vertical ? r.x() + r.width() / 2 - 2 : r.x() ),
		( orientation == Qt::Horizontal ? r.y() + r.height() / 2 - 2 : r.y() ),
		( orientation == Qt::Vertical ? 4 : r.width() ),
		( orientation == Qt::Horizontal ? 4 : r.height() ) );

	p->setPen( Qt::NoPen );
	p->setBrush( c1 );
	p->drawEllipse( dark );
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
	viewport->setContentsMargins( 0, 0, 0, 0 );

	horIndicator = new ScrollIndicator( ic, Qt::Horizontal, viewport );
	vertIndicator = new ScrollIndicator( ic, Qt::Vertical, viewport );

	horBlur = new BlurEffect( ic, Qt::Vertical, viewport );
	vertBlur = new BlurEffect( ic, Qt::Horizontal, viewport );

	horBlurAnim = new QVariantAnimation( q );
	horBlurAnim->setDuration( 300 );
	horBlurAnim->setLoopCount( 1 );

	vertBlurAnim = new QVariantAnimation( q );
	vertBlurAnim->setDuration( 300 );
	vertBlurAnim->setLoopCount( 1 );

	animationTimer = new QTimer( q );
	animationTimer->setSingleShot( true );

	startBlurAnimTimer = new QTimer( q );
	startBlurAnimTimer->setSingleShot( true );

	scroller = new Scroller( q, q );

	q->setFocusPolicy( Qt::WheelFocus );
	q->setFrameStyle( QFrame::NoFrame | QFrame::Plain );
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

	horBlur->resize( horBlur->sizeHint().width(),
		viewportRect.height() * 0.75 );

	vertBlur->resize( viewportRect.width() * 0.75,
		vertBlur->sizeHint().height() );
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

	if( dx != 0 )
	{
		horIndicator->needPaint = true;
		horIndicator->alpha = horIndicator->color.alpha();
		horIndicator->animate = false;
		vertIndicator->alpha = vertIndicator->color.alpha();
		vertIndicator->animate = false;
		animationTimer->stop();
		horIndicator->raise();
	}

	if( dy != 0 )
	{
		vertIndicator->needPaint = true;
		vertIndicator->alpha = vertIndicator->color.alpha();
		vertIndicator->animate = false;
		horIndicator->alpha = horIndicator->color.alpha();
		horIndicator->animate = false;
		animationTimer->stop();
		vertIndicator->raise();
	}

	makeBlurEffectIfNeeded();

	calcIndicators();

	q->update();
	horIndicator->update();
	vertIndicator->update();
}

void
AbstractScrollAreaPrivate::makeBlurEffectIfNeeded()
{
	if( horBlur->policy != AbstractScrollArea::BlurAlwaysOff )
	{
		const QRect r = viewport->rect();

		const QPoint p = topLeftCorner;

		normalizePosition();

		const QSize s = viewport->size();

		const QPoint maxPos = QPoint( scrolledAreaSize.width() - s.width(),
			scrolledAreaSize.height() - s.height() );

		if( p.x() < 0 || ( horBlur->pressure < 0 && maxPos.x() >= p.x() ) )
		{
			if( horBlur->pressure <= 0 && horBlur->pressure + p.x() > 0 )
				horBlur->pressure = 0;
			else if( qAbs( horBlur->pressure + p.x() ) > horBlur->maxPressure )
				horBlur->pressure = ( horBlur->pressure < 0 ?
					- horBlur->maxPressure : horBlur->maxPressure );
			else
				horBlur->pressure += p.x();

			horBlur->move(
				( horBlur->parent() == viewport ? r.x() : topLeftCorner.x() )
					- horBlur->width() / 2,
				( horBlur->parent() == viewport ? r.y() : topLeftCorner.y() )
					+ ( r.height() - horBlur->height() ) / 2 );
		}

		if( p.y() < 0 || ( vertBlur->pressure < 0 && maxPos.y() >= p.y() ) )
		{
			if( vertBlur->pressure <= 0 && vertBlur->pressure + p.y() > 0 )
				vertBlur->pressure = 0;
			else if( qAbs( vertBlur->pressure + p.y() ) > vertBlur->maxPressure )
				vertBlur->pressure = ( vertBlur->pressure < 0 ?
					- vertBlur->maxPressure : vertBlur->maxPressure );
			else
				vertBlur->pressure += p.y();

			vertBlur->move(
				( vertBlur->parent() == viewport ? r.x() : topLeftCorner.x() )
					+ ( r.width() - vertBlur->width() ) / 2,
				( vertBlur->parent() == viewport ? r.y() : topLeftCorner.y() )
					- vertBlur->height() / 2 );
		}

		if( maxPos.x() < p.x() || ( horBlur->pressure > 0 && p.x() > 0 ) )
		{
			if( horBlur->pressure >= 0 &&
				horBlur->pressure + p.x() - maxPos.x() < 0 )
					horBlur->pressure = 0;
			else if( qAbs( horBlur->pressure + p.x() - maxPos.x() ) > horBlur->maxPressure )
				horBlur->pressure = ( horBlur->pressure < 0 ?
					- horBlur->maxPressure : horBlur->maxPressure );
			else
				horBlur->pressure += ( p.x() - maxPos.x() );

			horBlur->move(
				( horBlur->parent() == viewport ? r.x() : topLeftCorner.x() )
					+ r.width() - horBlur->width() / 2,
				( horBlur->parent() == viewport ? r.y() : topLeftCorner.y() )
					+ ( r.height() - horBlur->height() ) / 2 );
		}

		if( maxPos.y() < p.y() || ( vertBlur->pressure > 0 && p.y() > 0 ) )
		{
			if( vertBlur->pressure >= 0 &&
				vertBlur->pressure + p.y() - maxPos.y() < 0 )
					vertBlur->pressure = 0;
			else if( qAbs( vertBlur->pressure + p.y() - maxPos.y() ) > vertBlur->maxPressure )
				vertBlur->pressure = ( vertBlur->pressure < 0 ?
					- vertBlur->maxPressure : vertBlur->maxPressure );
			else
				vertBlur->pressure += ( p.y() - maxPos.y() );

			vertBlur->move(
				( vertBlur->parent() == viewport ? r.x() : topLeftCorner.x() )
					+ ( r.width() - vertBlur->width() ) / 2,
				( vertBlur->parent() == viewport ? r.y() : topLeftCorner.y() )
					+ r.height() - vertBlur->height() / 2 );
		}

		if( horBlur->pressure != 0 )
		{
			horBlur->raise();
			horBlur->show();
			horBlur->update();
		}
		else
			horBlur->hide();

		if( vertBlur->pressure != 0 )
		{
			vertBlur->raise();
			vertBlur->show();
			vertBlur->update();
		}
		else
			vertBlur->hide();
	}
	else
		normalizePosition();
}

void
AbstractScrollAreaPrivate::animateHiddingBlurEffect()
{
	horBlurAnim->setStartValue( horBlur->pressure );
	horBlurAnim->setEndValue( 0 );
	vertBlurAnim->setStartValue( vertBlur->pressure );
	vertBlurAnim->setEndValue( 0 );

	if( horBlur->pressure != 0 )
		horBlurAnim->start();

	if( vertBlur->pressure != 0 )
		vertBlurAnim->start();
}

void
AbstractScrollAreaPrivate::stopAnimatingBlurEffect()
{
	horBlurAnim->stop();
	vertBlurAnim->stop();
}

void
AbstractScrollAreaPrivate::animateScrollIndicators()
{
	animationTimer->stop();
	horIndicator->alpha = horIndicator->color.alpha();
	vertIndicator->alpha = vertIndicator->color.alpha();
	animationTimer->start( animationTimeout );

	if( horIndicator->needPaint )
	{
		horIndicator->animate = true;
		horIndicator->update();
	}

	if( vertIndicator->needPaint )
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

	connect( d->animationTimer, &QTimer::timeout,
		this, &AbstractScrollArea::_q_animateScrollIndicators );

	connect( d->startBlurAnimTimer, &QTimer::timeout,
		this, &AbstractScrollArea::_q_startBlurAnim );

	connect( d->scroller, &Scroller::scroll,
		this, &AbstractScrollArea::_q_kineticScrolling );

	connect( d->scroller, &Scroller::aboutToStart,
		this, &AbstractScrollArea::_q_kineticScrollingAboutToStart );

	connect( d->scroller, &Scroller::finished,
		this, &AbstractScrollArea::_q_kineticScrollingFinished );

	connect( d->horBlurAnim, &QVariantAnimation::valueChanged,
		this, &AbstractScrollArea::_q_horBlurAnim );

	connect( d->horBlurAnim, &QVariantAnimation::finished,
		this, &AbstractScrollArea::_q_horBlurAnimFinished );

	connect( d->vertBlurAnim, &QVariantAnimation::valueChanged,
		this, &AbstractScrollArea::_q_vertBlurAnim );

	connect( d->vertBlurAnim, &QVariantAnimation::finished,
		this, &AbstractScrollArea::_q_vertBlurAnimFinished );
}

AbstractScrollArea::AbstractScrollArea( AbstractScrollAreaPrivate * dd,
	QWidget * parent )
	:	QFrame( parent )
	,	d( dd )
{
	d->init();

	connect( d->animationTimer, &QTimer::timeout,
		this, &AbstractScrollArea::_q_animateScrollIndicators );

	connect( d->startBlurAnimTimer, &QTimer::timeout,
		this, &AbstractScrollArea::_q_startBlurAnim );

	connect( d->scroller, &Scroller::scroll,
		this, &AbstractScrollArea::_q_kineticScrolling );

	connect( d->scroller, &Scroller::aboutToStart,
		this, &AbstractScrollArea::_q_kineticScrollingAboutToStart );

	connect( d->scroller, &Scroller::finished,
		this, &AbstractScrollArea::_q_kineticScrollingFinished );

	connect( d->horBlurAnim, &QVariantAnimation::valueChanged,
		this, &AbstractScrollArea::_q_horBlurAnim );

	connect( d->horBlurAnim, &QVariantAnimation::finished,
		this, &AbstractScrollArea::_q_horBlurAnimFinished );

	connect( d->vertBlurAnim, &QVariantAnimation::valueChanged,
		this, &AbstractScrollArea::_q_vertBlurAnim );

	connect( d->vertBlurAnim, &QVariantAnimation::finished,
		this, &AbstractScrollArea::_q_vertBlurAnimFinished );
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
		d->horBlur->setParent( d->viewport );
		d->vertBlur->setParent( d->viewport );

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

Scroller *
AbstractScrollArea::scroller() const
{
	return d->scroller;
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

const QColor &
AbstractScrollArea::blurColor() const
{
	return d->horBlur->color;
}

void
AbstractScrollArea::setBlurColor( const QColor & c )
{
	if( d->horBlur->color != c )
	{
		d->horBlur->color = c;
		d->vertBlur->color = c;
	}
}

AbstractScrollArea::BlurPolicy
AbstractScrollArea::blurPolicy() const
{
	return d->horBlur->policy;
}

void
AbstractScrollArea::setBlurPolicy( BlurPolicy policy )
{
	if( d->horBlur->policy != policy )
	{
		d->horBlur->policy = policy;
		d->vertBlur->policy = policy;
	}
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
	const int dx = d->topLeftCorner.x() - p.x();
	const int dy = d->topLeftCorner.y() - p.y();

	d->scrollContentsBy( dx, dy );

	scrollContentsBy( dx, dy );

	d->animateScrollIndicators();
}

void
AbstractScrollArea::startScrollIndicatorsAnimation()
{
	d->animateScrollIndicators();
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

		e->accept();
	}
	else
		e->ignore();
}

void
AbstractScrollArea::mouseReleaseEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
	{
		d->leftMouseButtonPressed = false;

		if( ( d->horIndicator->needPaint || d->vertIndicator->needPaint ) &&
			( d->horIndicator->policy == ScrollIndicatorAsNeeded ||
				d->vertIndicator->policy == ScrollIndicatorAsNeeded ) )
					d->animateScrollIndicators();
		else
			d->stopScrollIndicatorsAnimation();

		d->animateHiddingBlurEffect();

		e->accept();
	}
	else
		e->ignore();
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

		e->accept();
	}
	else
		e->ignore();
}

void
AbstractScrollArea::wheelEvent( QWheelEvent * e )
{
	QPoint numPixels = e->pixelDelta();
	QPoint numDegrees = e->angleDelta();

	d->stopAnimatingBlurEffect();

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

	if( d->horIndicator->policy == ScrollIndicatorAsNeeded ||
		d->vertIndicator->policy == ScrollIndicatorAsNeeded )
			d->animateScrollIndicators();

	d->startBlurAnimTimer->stop();
	d->startBlurAnimTimer->start( d->animationTimeout );

	e->accept();
}

void
AbstractScrollArea::_q_animateScrollIndicators()
{
	d->horIndicator->alpha -= d->animationAlphaDelta;
	d->vertIndicator->alpha -= d->animationAlphaDelta;

	if( d->horIndicator->alpha <= 0 )
		d->stopScrollIndicatorsAnimation();
	else if( d->horIndicator->needPaint || d->vertIndicator->needPaint )
		d->animationTimer->start( d->animationTimeout );

	if( d->horIndicator->needPaint )
		d->horIndicator->update();

	if( d->vertIndicator->needPaint )
		d->vertIndicator->update();
}

void
AbstractScrollArea::_q_kineticScrolling( int dx, int dy )
{
	d->scrollContentsBy( dx, dy );
	scrollContentsBy( dx, dy );
}

void
AbstractScrollArea::_q_kineticScrollingAboutToStart()
{
	d->stopScrollIndicatorsAnimation();
	d->stopAnimatingBlurEffect();
}

void
AbstractScrollArea::_q_kineticScrollingFinished()
{
	if( d->horIndicator->needPaint || d->vertIndicator->needPaint )
	{
		if( d->horIndicator->policy == ScrollIndicatorAsNeeded ||
			d->vertIndicator->policy == ScrollIndicatorAsNeeded )
				d->animateScrollIndicators();
	}

	d->animateHiddingBlurEffect();
}

void
AbstractScrollArea::_q_horBlurAnim( const QVariant & value )
{
	d->horBlur->pressure = value.toInt();
	d->horBlur->update();
}

void
AbstractScrollArea::_q_horBlurAnimFinished()
{
	d->horBlur->hide();
}

void
AbstractScrollArea::_q_vertBlurAnim( const QVariant & value )
{
	d->vertBlur->pressure = value.toInt();
	d->vertBlur->update();
}

void
AbstractScrollArea::_q_vertBlurAnimFinished()
{
	d->vertBlur->hide();
}

void
AbstractScrollArea::_q_startBlurAnim()
{
	d->animateHiddingBlurEffect();
}

} /* namespace QtMWidgets */
