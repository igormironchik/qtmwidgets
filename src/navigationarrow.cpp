
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// QtMWidgets include.
#include "navigationarrow.hpp"
#include "fingergeometry.hpp"
#include "private/drawing.hpp"
#include "color.hpp"

// Qt include.
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>


namespace QtMWidgets {

//
// NavigationArrowPrivate
//

class NavigationArrowPrivate {
public:
	NavigationArrowPrivate( NavigationArrow::Direction d,
		NavigationArrow * parent )
		:	q( parent )
		,	direction( d )
		,	leftButtonPressed( false )
	{
	}

	void init()
	{
		baseColor = q->palette().color( QPalette::Highlight );
		color = baseColor;
		timer = new QTimer( q );

		QObject::connect( timer, &QTimer::timeout,
			q, &NavigationArrow::_q_timer );

		q->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

		q->setBackgroundRole( QPalette::Base );
		q->setAutoFillBackground( true );
	}

	NavigationArrow * q;

	NavigationArrow::Direction direction;
	QColor color;
	QColor baseColor;
	QTimer * timer;
	bool leftButtonPressed;
}; // class NavigationArrowPrivate


//
// NavigationArrow
//

NavigationArrow::NavigationArrow( Direction direction, QWidget * parent )
	:	QWidget( parent )
	,	d( new NavigationArrowPrivate( direction, this ) )
{
	d->init();
}

NavigationArrow::~NavigationArrow()
{
}

NavigationArrow::Direction
NavigationArrow::direction() const
{
	return d->direction;
}

void
NavigationArrow::setDirection( Direction direct )
{
	if( d->direction != direct )
	{
		d->direction = direct;

		updateGeometry();
		update();
	}
}

const QColor &
NavigationArrow::color() const
{
	return d->baseColor;
}

void
NavigationArrow::setColor( const QColor & color )
{
	if( d->baseColor != color )
	{
		d->baseColor = color;

		update();
	}
}

QSize
NavigationArrow::minimumSizeHint() const
{
	const int width = qRound( (qreal) FingerGeometry::width() / 5.0 );
	const int height = qRound( (qreal) FingerGeometry::height() / 3.0 );

	return ( ( d->direction == Left || d->direction == Right ) ?
		QSize( width, height ) : QSize( height, width ) );
}

QSize
NavigationArrow::sizeHint() const
{
	return minimumSizeHint();
}

void
NavigationArrow::animate()
{
	if( d->timer->isActive() )
		d->timer->stop();

	d->color = lighterColor( d->baseColor, 25 );

	d->timer->start( 500 );

	update();
}

void
NavigationArrow::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	p.setRenderHint( QPainter::Antialiasing );

	const QRect r = rect();

	if( d->direction == Right )
		drawArrow( &p, r, d->color );
	if( d->direction == Left )
	{
		p.rotate( 180 );
		p.translate( - r.width(), - r.height() );

		drawArrow( &p, r, d->color );
	}
	else if( d->direction == Top )
	{
		p.rotate( 180 );
		p.translate( - r.width(), - r.height() );

		drawArrow2( &p, r, d->color );
	}
	else if( d->direction == Bottom )
		drawArrow2( &p, r, d->color );
}

void
NavigationArrow::mousePressEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
	{
		d->leftButtonPressed = true;

		e->accept();
	}
	else
		e->ignore();
}

void
NavigationArrow::mouseReleaseEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
	{
		if( d->leftButtonPressed && rect().contains( e->pos() ) )
			emit clicked();

		d->leftButtonPressed = false;

		e->accept();
	}
	else
		e->ignore();
}

void
NavigationArrow::_q_timer()
{
	d->color = d->baseColor;

	update();
}

} /* namespace QtMWidgets */
