
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// QtMWidgets include.
#include "navigationbutton.hpp"
#include "fingergeometry.hpp"
#include "private/drawing.hpp"
#include "color.hpp"
#include "private/utils.hpp"

// Qt include.
#include <QStyleOption>
#include <QPainter>


namespace QtMWidgets {

//
// NavigationButtonPrivate
//

class NavigationButtonPrivate {
public:
	NavigationButtonPrivate( NavigationButton * parent,
		NavigationButton::Direction d )
		:	q( parent )
		,	direction( d )
	{
	}

	void init();
	QString makeString( const QString & text, const QRect & r,
		int flags, const QStyleOption & opt );

	NavigationButton * q;
	NavigationButton::Direction direction;
	QColor color;
	QColor baseColor;
	QColor textColor;
}; // class NavigationButtonPrivate


void
NavigationButtonPrivate::init()
{
	q->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
	q->setAutoFillBackground( true );
	q->setBackgroundRole( QPalette::Base );

	baseColor = q->palette().color( QPalette::Highlight );
	color = baseColor;
	textColor = baseColor;
}

QString
NavigationButtonPrivate::makeString( const QString & text, const QRect & r,
	int flags, const QStyleOption & opt )
{
	auto res = accomodateString( text, r, flags, opt );
	res.replace( QLatin1String( "&..." ), QLatin1String( "..." ) );

	return res;
}


//
// NavigationButton
//

NavigationButton::NavigationButton( Direction direction, QWidget * parent )
	:	QAbstractButton( parent )
	,	d( new NavigationButtonPrivate( this, direction ) )
{
	d->init();

	connect( this, &QAbstractButton::pressed,
		this, &NavigationButton::_q_pressed );
	connect( this, &QAbstractButton::released,
		this, &NavigationButton::_q_released );
}

NavigationButton::NavigationButton( Direction direction, const QString & text,
	QWidget * parent )
	:	QAbstractButton( parent )
	,	d( new NavigationButtonPrivate( this, direction ) )
{
	d->init();

	setText( text );

	connect( this, &QAbstractButton::pressed,
		this, &NavigationButton::_q_pressed );
	connect( this, &QAbstractButton::released,
		this, &NavigationButton::_q_released );
}

NavigationButton::~NavigationButton()
{
}

NavigationButton::Direction
NavigationButton::direction() const
{
	return d->direction;
}

void
NavigationButton::setDirection( Direction direct )
{
	if( d->direction != direct )
	{
		d->direction = direct;

		update();
	}
}

const QColor &
NavigationButton::textColor() const
{
	return d->textColor;
}

void
NavigationButton::setTextColor( const QColor & c )
{
	if( d->textColor != c )
	{
		d->textColor = c;

		update();
	}
}

const QColor &
NavigationButton::arrowColor() const
{
	return d->baseColor;
}

void
NavigationButton::setArrowColor( const QColor & c )
{
	if( d->baseColor != c )
	{
		d->baseColor= c;

		update();
	}
}

QSize
NavigationButton::minimumSizeHint() const
{
	return QSize( FingerGeometry::width() / 3,
		FingerGeometry::height() );
}

const int c_offset = 10;

QSize
NavigationButton::sizeHint() const
{
	const QRect & b = fontMetrics().boundingRect( text() );

	return QSize( minimumSizeHint().width() + b.width() + c_offset * 2,
		( minimumSizeHint().height() > b.height() ?
			minimumSizeHint().height() : b.height() ) );
}

void
NavigationButton::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	p.setRenderHint( QPainter::Antialiasing );

	const QRect r = rect();
	QRect arrowRect;
	QRect textRect;

	const int arrowWidth = qRound( (qreal) FingerGeometry::width() / 5.0 );
	const int arrowHeight = qRound( (qreal) FingerGeometry::height() / 3.0 );
	const int delta = ( r.height() - arrowHeight ) /2;

	int flags = Qt::TextSingleLine | Qt::TextShowMnemonic | Qt::AlignVCenter;

	switch( d->direction )
	{
		case Left :
		{
			arrowRect.setRect( r.width() - arrowWidth, r.y() + delta,
				arrowWidth, arrowHeight );

			textRect.setRect( r.x() + arrowWidth + c_offset, r.y(),
				r.width() - arrowWidth - c_offset, r.height() );

			flags |= Qt::AlignLeft;
		}
			break;

		case Right :
		{
			arrowRect.setRect( r.x() + r.width() - arrowWidth, r.y() + delta,
				arrowWidth, arrowHeight );

			textRect.setRect( r.x(), r.y(),
				r.width() - arrowWidth - c_offset, r.height() );

			flags |= Qt::AlignRight;
		}
			break;

		default :
			break;
	}

	QStyleOption opt;
	opt.initFrom( this );

	const QString t = d->makeString( text(), textRect, flags, opt );

	p.setPen( d->textColor );
	p.drawText( textRect, flags, t );

	if( d->direction == Left )
	{
		p.rotate( 180 );
		p.translate( - r.width(), - r.height() );
	}

	drawArrow( &p, arrowRect, d->color );
}

void
NavigationButton::_q_pressed()
{
	d->color = lighterColor( d->baseColor, 50 );

	update();
}

void
NavigationButton::_q_released()
{
	d->color = d->baseColor;

	update();
}

} /* namespace QtMWidgets */
