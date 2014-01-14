
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
#include "drawing.hpp"

// Qt include.
#include <QPainter>
#include <QStyleOption>
#include <QLinearGradient>
#include <QBrush>
#include <QPen>


namespace QtMWidgets {

//
// drawCylinder
//

void
drawCylinder( QPainter * p, const QRect & r,
	bool roundLeftCorner, bool roundRightCorner )
{
	p->setRenderHint( QPainter::Antialiasing );

	QLinearGradient firstVertLineGradient( QPointF( 0.0, 0.0 ),
		QPointF( 0.0, 1.0 ) );
	firstVertLineGradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	firstVertLineGradient.setColorAt( 0.0, QColor( 50, 50, 60 ) );
	firstVertLineGradient.setColorAt( 0.5, QColor( 140, 140, 150 ) );
	firstVertLineGradient.setColorAt( 1.0, QColor( 50, 50, 60 ) );

	QLinearGradient secondVertLineGradient( QPointF( 0.0, 0.0 ),
		QPointF( 0.0, 1.0 ) );
	secondVertLineGradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	secondVertLineGradient.setColorAt( 0.0, QColor( 60, 60, 80 ) );
	secondVertLineGradient.setColorAt( 0.5, QColor( 200, 205, 225 ) );
	secondVertLineGradient.setColorAt( 1.0, QColor( 60, 60, 80 ) );

	p->setPen( Qt::NoPen );
	p->setBrush( firstVertLineGradient );

	p->drawRect( r.x(), roundLeftCorner ? 2 : 0,
		1, roundLeftCorner ? r.height() - 4 : r.height() );
	p->drawRect( r.x() + r.width() - 1, roundRightCorner ? 2 : 0,
		1, roundRightCorner ? r.height() - 4 : r.height() );

	p->setBrush( secondVertLineGradient );

	p->drawRect( r.x() + 1, roundLeftCorner ? 1 : 0,
		1, roundLeftCorner ? r.height() - 2 : r.height() );
	p->drawRect( r.x() + r.width() - 2, roundRightCorner ? 1 : 0,
		1, roundRightCorner ? r.height() - 2 : r.height() );

	p->drawRect( r.x() + 2, 0, 1, r.height() );
	p->drawRect( r.x() + r.width() - 3, 0,
		1, r.height() );

	QLinearGradient backgroundGradient( QPointF( 0.0, 0.0 ),
		QPointF( 0.0, 1.0 ) );
	backgroundGradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	backgroundGradient.setColorAt( 0.0, QColor( 80, 80, 80 ) );
	backgroundGradient.setColorAt( 0.15, QColor( 215, 215, 220 ) );
	backgroundGradient.setColorAt( 0.5, QColor( 255, 255, 255 ) );
	backgroundGradient.setColorAt( 0.85, QColor( 215, 215, 220 ) );
	backgroundGradient.setColorAt( 1.0, QColor( 80, 80, 80 ) );

	p->setPen( Qt::NoPen );
	p->setBrush( backgroundGradient );
	p->drawRect( r.x() + 3, 0, r.width() - 2 * 3, r.height() );
}

} /* namespace QtMWidgets */
