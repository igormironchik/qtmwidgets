
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2015 Igor Mironchik

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
#include "messageboxbutton.hpp"
#include "fingergeometry.hpp"

// Qt include.
#include <QPainter>
#include <QFontMetrics>


namespace QtMWidgets {

//
// MsgBoxButtonPrivate
//

class MsgBoxButtonPrivate {
public:
	MsgBoxButtonPrivate( const QString & t, MsgBoxButton * parent )
		:	q( parent )
		,	text( t )
		,	pressed( false )
	{
	}

	void init();

	MsgBoxButton * q;
	QString text;
	bool pressed;
}; // class MsgBoxButtonPrivate

void
MsgBoxButtonPrivate::init()
{
	q->setBackgroundRole( QPalette::Window );
	q->setAutoFillBackground( true );
	q->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
}


//
// MsgBoxButton
//

MsgBoxButton::MsgBoxButton( const QString & text, QWidget * parent )
	:	QAbstractButton( parent )
	,	d( new MsgBoxButtonPrivate( text, this ) )
{
	d->init();

	connect( this, &QAbstractButton::pressed,
		this, &MsgBoxButton::_q_pressed );

	connect( this, &QAbstractButton::released,
		this, &MsgBoxButton::_q_released );
}

MsgBoxButton::~MsgBoxButton()
{
}

QSize
MsgBoxButton::minimumSizeHint() const
{
	const int margin = fontMetrics().height() / 3;

	const QSize s = fontMetrics()
		.boundingRect( QRect(), Qt::AlignCenter, d->text )
		.marginsAdded( QMargins( margin, margin, margin, margin ) )
		.size();

	return QSize( qMax( s.width(), FingerGeometry::width() ),
		qMax( s.height(), FingerGeometry::height() ) );
}

QSize
MsgBoxButton::sizeHint() const
{
	return minimumSizeHint();
}

void
MsgBoxButton::paintEvent( QPaintEvent * )
{
	QPainter p( this );

	p.setPen( palette().color( QPalette::WindowText ) );

	p.drawText( rect(), d->text, QTextOption( Qt::AlignCenter ) );

	if( d->pressed )
	{
		QColor c = palette().color( QPalette::Highlight );
		c.setAlpha( 75 );

		p.setPen( Qt::NoPen );
		p.setBrush( c );
		p.drawRect( rect() );
	}
}

void
MsgBoxButton::_q_pressed()
{
	d->pressed = true;

	update();
}

void
MsgBoxButton::_q_released()
{
	d->pressed = false;

	update();
}

} /* namespace QtMWidgets */
