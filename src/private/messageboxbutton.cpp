
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
