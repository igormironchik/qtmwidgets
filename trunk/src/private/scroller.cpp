
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
#include "scroller.hpp"
#include "fingergeometry.hpp"

// Qt include.
#include <QEvent>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QVariantAnimation>


namespace QtMWidgets {

//
// ScrollerPrivate
//

class ScrollerPrivate {
public:
	ScrollerPrivate( Scroller * parent, QObject * t )
		:	q( parent )
		,	target( t )
		,	minVelocity( 0 )
		,	scrollTime( 3000 )
		,	xVelocity( 0.0 )
		,	yVelocity( 0.0 )
		,	mousePressed( false )
		,	maxPause( 300 )
		,	scrollAnimation( 0 )
	{
	}

	void init();

	Scroller * q;
	QObject * target;
	uint minVelocity;
	QElapsedTimer elapsed;
	QPoint pos;
	uint scrollTime;
	qreal xVelocity;
	qreal yVelocity;
	bool mousePressed;
	qint64 maxPause;
	QVariantAnimation * scrollAnimation;
}; // class ScrollerPrivate

void
ScrollerPrivate::init()
{
	minVelocity = qMax( FingerGeometry::height(), FingerGeometry::width() ) * 3;

	target->installEventFilter( q );

	scrollAnimation = new QVariantAnimation( q );
	scrollAnimation->setEasingCurve( QEasingCurve::OutCirc );
	scrollAnimation->setDuration( scrollTime );
}


//
// Scroller
//

Scroller::Scroller( QObject * target, QObject * parent )
	:	QObject( parent )
	,	d( new ScrollerPrivate( this, target ) )
{
	d->init();

	connect( d->scrollAnimation, &QVariantAnimation::valueChanged,
		this, &Scroller::_q_animation );

	connect( d->scrollAnimation, &QVariantAnimation::finished,
		this, &Scroller::_q_animationFinished );
}

Scroller::~Scroller()
{
}

uint
Scroller::minRecognizedVelocity() const
{
	return d->minVelocity;
}

void
Scroller::setMinRecognizedVelocity( uint v )
{
	d->minVelocity = v;
}

bool
Scroller::eventFilter( QObject * obj, QEvent * event )
{
	if( obj == d->target )
	{
		if( event->type() == QEvent::MouseButtonPress )
		{
			QMouseEvent * e = static_cast< QMouseEvent* > ( event );

			d->pos = e->pos();
			d->mousePressed = true;
			d->xVelocity = 0.0;
			d->yVelocity = 0.0;

			d->scrollAnimation->stop();

			d->elapsed.start();
		}
		else if( event->type() == QEvent::MouseButtonRelease )
		{
			if( d->elapsed.elapsed() <= d->maxPause )
			{
				if( qAbs( d->xVelocity ) >= d->minVelocity ||
					qAbs( d->yVelocity ) >= d->minVelocity )
				{
					const QPoint newPos = QPoint(
						d->pos.x() + qRound( d->xVelocity * d->scrollTime / 1000 ),
						d->pos.y() + qRound( d->yVelocity * d->scrollTime / 1000 ) );

					d->scrollAnimation->setStartValue( d->pos );
					d->scrollAnimation->setEndValue( newPos );

					emit aboutToStart();

					d->scrollAnimation->start();
				}
			}

			d->elapsed.invalidate();
			d->mousePressed = false;
		}
		else if( event->type() == QEvent::MouseMove )
		{
			if( d->mousePressed )
			{
				QMouseEvent * e = static_cast< QMouseEvent* > ( event );

				const qreal time = (qreal) d->elapsed.elapsed() / 1000.0;

				const QPoint p = e->pos() - d->pos;

				d->pos = e->pos();

				if( p.manhattanLength() > 5 )
				{
					d->xVelocity = (qreal) p.x() / time;
					d->yVelocity = (qreal) p.y() / time;

					d->elapsed.restart();
				}
			}
		}

		return false;
	}
	else
		return QObject::eventFilter( obj, event );
}

void
Scroller::_q_animation( const QVariant & v )
{
	if( d->scrollAnimation->state() == QAbstractAnimation::Running )
	{
		const QPoint p = v.toPoint() - d->pos;
		d->pos = v.toPoint();

		emit scroll( p.x(), p.y() );
	}
}

void
Scroller::_q_animationFinished()
{
	emit finished();
}

} /* namespace QtMWidgets */
