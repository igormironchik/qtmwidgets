
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
	{
	}

	void init();

	Scroller * q;
	QObject * target;
	uint minVelocity;
}; // class ScrollerPrivate

void
ScrollerPrivate::init()
{
	minVelocity = qMax( FingerGeometry::height(), FingerGeometry::width() ) * 3;

	target->installEventFilter( q );
}


//
// Scroller
//

Scroller::Scroller( QObject * target, QObject * parent )
	:	QObject( parent )
	,	d( new ScrollerPrivate( this, target ) )
{
	d->init();
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

		}
		else if( event->type() == QEvent::MouseButtonRelease )
		{

		}
		else if( event->type() == QEvent::MouseMove )
		{

		}

		return false;
	}
	else
		return QObject::eventFilter( obj, event );
}

} /* namespace QtMWidgets */
