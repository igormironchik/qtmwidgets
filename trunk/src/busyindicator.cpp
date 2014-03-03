
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
#include "busyindicator.hpp"

// Qt include.
#include <QPainter>
#include <QPixmap>
#include <QVariantAnimation>


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
		,	pixmap( size )
	{
	}

	void init();
	void preparePixmap();

	BusyIndicator * q;
	int outerRadius;
	int innerRadius;
	QSize size;
	bool running;
	QVariantAnimation * animation;
	QPixmap pixmap;
}; // class BusyIndicatorPrivate

void
BusyIndicatorPrivate::init()
{
	animation = new QVariantAnimation( q );
	animation->setStartValue( 0.0 );
	animation->setEndValue( 359.0 );
	animation->setDuration( 1000 );
	animation->setLoopCount( -1 );

	QObject::connect( animation, SIGNAL( valueChanged( const QVariant & ) ),
		q, SLOT( _q_update( const QVariant & ) ) );

	preparePixmap();

	animation->start();
}

void
BusyIndicatorPrivate::preparePixmap()
{
	QPainterPath path;
	path.setFillRule( Qt::OddEvenFill );
	path.addEllipse( 0, 0, outerRadius * 2, outerRadius * 2 );
	path.addEllipse( outerRadius - innerRadius, outerRadius - innerRadius,
		innerRadius * 2, innerRadius * 2 );

	pixmap.fill( Qt::transparent );
	QPainter p( &pixmap );

	p.setPen( Qt::NoPen );
	p.setRenderHint( QPainter::Antialiasing );

	QConicalGradient gradient( outerRadius, outerRadius, 0.0 );
	gradient.setColorAt( 0.0, q->palette().color( QPalette::Highlight ) );
	gradient.setColorAt( 1.0, Qt::transparent );

	p.setBrush( gradient );

	p.drawPath( path );

	p.end();
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
	p.rotate( d->animation->currentValue().toReal() );
	p.drawPixmap( -d->size.width() / 2,
		-d->size.height() / 2, d->pixmap );
}

void
BusyIndicator::_q_update( const QVariant & )
{
	update();
}

} /* namespace QtMWidgets */
