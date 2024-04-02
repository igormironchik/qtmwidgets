
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// QtMWidgets include.
#include "busyindicator.hpp"

// Qt include.
#include <QPainter>
#include <QVariantAnimation>
#include <QPainterPath>


namespace QtMWidgets {

//
// BusyIndicatorPrivate
//

class BusyIndicatorPrivate {
public:
	BusyIndicatorPrivate( BusyIndicator * parent )
		:	q( parent )
		,	outerRadius( 10 )
		,	innerRadius( outerRadius * 0.6 )
		,	size( outerRadius * 2, outerRadius * 2 )
		,	running( true )
		,	animation( 0 )
	{
	}

	void init();

	BusyIndicator * q;
	int outerRadius;
	int innerRadius;
	QSize size;
	bool running;
	QVariantAnimation * animation;
	QColor color;
}; // class BusyIndicatorPrivate

void
BusyIndicatorPrivate::init()
{
	animation = new QVariantAnimation( q );
	animation->setStartValue( 0.0 );
	animation->setEndValue( 359.0 );
	animation->setDuration( 1000 );
	animation->setLoopCount( -1 );

	QObject::connect( animation, &QVariantAnimation::valueChanged,
		q, &BusyIndicator::_q_update );

	color = q->palette().color( QPalette::Highlight );

	animation->start();
}


//
// BusyIndicator
//

BusyIndicator::BusyIndicator( QWidget * parent )
	:	QWidget( parent )
	,	d( new BusyIndicatorPrivate( this ) )
{
	d->init();
}

BusyIndicator::~BusyIndicator()
{
	d->animation->stop();
}

bool
BusyIndicator::isRunning() const
{
	return d->running;
}

void
BusyIndicator::setRunning( bool on )
{
	if( on != d->running )
	{
		d->running = on;

		if( d->running )
		{
			show();
			d->animation->start();
		}
		else
		{
			hide();
			d->animation->stop();
		}
	}
}

const QColor &
BusyIndicator::color() const
{
	return d->color;
}

void
BusyIndicator::setColor( const QColor & c )
{
	if( d->color != c )
	{
		d->color = c;
		update();
	}
}

int
BusyIndicator::radius() const
{
	return d->outerRadius;
}

void
BusyIndicator::setRadius( int r )
{
	if( d->outerRadius != r )
	{
		d->outerRadius = r;
		d->innerRadius = d->outerRadius * 0.6;
		d->size = QSize( d->outerRadius * 2, d->outerRadius * 2 );

		updateGeometry();
	}
}

QSize
BusyIndicator::minimumSizeHint() const
{
	return d->size;
}

QSize
BusyIndicator::sizeHint() const
{
	return d->size;
}

void
BusyIndicator::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	p.setRenderHint( QPainter::Antialiasing );
	p.translate( width() / 2, height() / 2 );

	QPainterPath path;
	path.setFillRule( Qt::OddEvenFill );
	path.addEllipse( - d->outerRadius, - d->outerRadius,
		d->outerRadius * 2, d->outerRadius * 2 );
	path.addEllipse( - d->innerRadius, - d->innerRadius,
		d->innerRadius * 2, d->innerRadius * 2 );

	p.setPen( Qt::NoPen );

	QConicalGradient gradient( 0, 0, - d->animation->currentValue().toReal() );
	gradient.setColorAt( 0.0, Qt::transparent );
	gradient.setColorAt( 0.05, d->color );
	gradient.setColorAt( 1.0, Qt::transparent );

	p.setBrush( gradient );

	p.drawPath( path );
}

void
BusyIndicator::_q_update( const QVariant & )
{
	update();
}

} /* namespace QtMWidgets */
