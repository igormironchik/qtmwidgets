
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
#include "navigationbutton.hpp"
#include "fingergeometry.hpp"
#include "private/drawing.hpp"
#include "color.hpp"

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
	const QRect & b = opt.fontMetrics.boundingRect( r, flags, text );

	QString res = text;

	if( b.width() > r.width() )
	{
		const int averageCount = r.width() / opt.fontMetrics.averageCharWidth();

		QString tmp = res;
		res = QString();
		int length = averageCount - 3;
		int i = 0;

		for( ; i < length && i < tmp.length(); ++i )
		{
			if( tmp.at( i ) == QLatin1Char( '&' ) )
			{
				res.append( tmp.at( i ) );
				++i;
				++length;

				if( i < tmp.length() )
					res.append( tmp.at( i ) );
			}
			else
				res.append( tmp.at( i ) );
		}

		if( ( res.endsWith( QLatin1Char( '&' ) ) && i != tmp.length() ) &&
			!res.endsWith( QLatin1String( "&&" ) ) )
				res.remove( res.length() - 1, 1 );

		res.append( QLatin1String( "..." ) );
	}

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

QSize
NavigationButton::sizeHint() const
{
	return minimumSizeHint();
}

void
NavigationButton::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	p.setRenderHint( QPainter::Antialiasing );

	const QRect r = rect();
	QRect arrowRect;
	QRect textRect;

	const int arrowWidth = FingerGeometry::width() / 3;
	const int arrowHeight = FingerGeometry::height() / 2;
	const int delta = ( r.height() - arrowHeight ) /2;
	const int offset = 10;

	int flags = Qt::TextSingleLine | Qt::TextShowMnemonic | Qt::AlignVCenter;

	switch( d->direction )
	{
		case Left :
		{
			arrowRect.setRect( r.width() - arrowWidth, r.y() + delta,
				arrowWidth, arrowHeight );

			textRect.setRect( r.x() + arrowWidth + offset, r.y(),
				r.width() - arrowWidth - offset, r.height() );

			flags |= Qt::AlignLeft;
		}
			break;

		case Right :
		{
			arrowRect.setRect( r.x() + r.width() - arrowWidth, r.y() + delta,
				arrowWidth, arrowHeight );

			textRect.setRect( r.x(), r.y(),
				r.width() - arrowWidth - offset, r.height() );

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
