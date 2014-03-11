
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
#include "cursorshifter.hpp"
#include "color.hpp"
#include "fingergeometry.hpp"

// Qt include.
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>


namespace QtMWidgets {

//
// CursorShifterPrivate
//

class CursorShifterPrivate {
public:
	CursorShifterPrivate( CursorShifter * parent )
		:	q( parent )
		,	leftMouseButtonPressed( false )
	{
	}

	void init();

	CursorShifter * q;
	QColor color;
	QColor light;
	int basicSize;
	QPoint cursorPos;
	QTimer * timer;
	bool leftMouseButtonPressed;
	QPoint offset;
}; // class CursorShifterPrivate

void
CursorShifterPrivate::init()
{
	color = q->palette().color( QPalette::Highlight );
	light = lighterColor( color, 75 );
	basicSize = qRound( (qreal) FingerGeometry::width() * 0.2 );
	timer = new QTimer( q );
}


//
// CursorShifter
//

CursorShifter::CursorShifter( QWidget * parent )
	:	QWidget( parent, Qt::Popup )
	,	d( new CursorShifterPrivate( this ) )
{
	d->init();
}

CursorShifter::~CursorShifter()
{
}

QPoint
CursorShifter::cursorPos() const
{
	return d->cursorPos;
}

QSize
CursorShifter::minimumSizeHint() const
{
	return QSize( d->basicSize * 2, d->basicSize * 3 );
}

QSize
CursorShifter::sizeHint() const
{
	return minimumSizeHint();
}

void
CursorShifter::setCursorPos( const QPoint & pos )
{
	if( d->cursorPos != pos )
	{
		d->cursorPos = pos;

		move( d->cursorPos.x() - d->basicSize, d->cursorPos.y() );
	}
}

void
CursorShifter::popup()
{
	show();
	d->timer->start( 1000 );
}

void
CursorShifter::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	p.setRenderHint( QPainter::Antialiasing );

	QPainterPath roof;
	roof.moveTo( d->basicSize, 0 );
	roof.lineTo( d->basicSize * 2, d->basicSize );
	roof.lineTo( 0, d->basicSize );
	roof.lineTo( d->basicSize, 0 );

	p.setPen( d->color );
	p.setBrush( d->light );
	p.drawPath( roof );

	p.setBrush( d->color );
	p.drawRect( 0, d->basicSize, d->basicSize * 2, d->basicSize * 2 );
}

void
CursorShifter::mousePressEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
	{
		d->leftMouseButtonPressed = true;
		d->timer->stop();
		d->offset = QPoint( d->basicSize, 0 ) - e->pos();
	}
}

void
CursorShifter::mouseMoveEvent( QMouseEvent * e )
{
	if( d->leftMouseButtonPressed )
		emit posChanged( e->globalPos() - d->offset );
}

void
CursorShifter::mouseReleaseEvent( QMouseEvent * )
{
	if( d->leftMouseButtonPressed )
	{
		d->leftMouseButtonPressed = false;
		d->timer->start( 1000 );
	}
}

void
CursorShifter::_q_hideTimer()
{
	hide();
}

} /* namespace QtMWidgets */
