
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

#ifndef QTMWIDGETS__PRIVATE__ABSTRACTSCROLLAREA_P_HPP__INCLUDED
#define QTMWIDGETS__PRIVATE__ABSTRACTSCROLLAREA_P_HPP__INCLUDED

// QtMWidgets include.
#include "../abstractscrollarea.hpp"

QT_BEGIN_NAMESPACE
class QStyleOption;
QT_END_NAMESPACE


namespace QtMWidgets {

//
// AbstractScrollAreaPrivate
//

class AbstractScrollAreaPrivate {
public:
	explicit AbstractScrollAreaPrivate( AbstractScrollArea * parent )
		:	q( parent )
		,	viewport( 0 )
		,	scrolledAreaSize( 0, 0 )
		,	topLeftCorner( 0, 0 )
		,	top( 0 )
		,	bottom( 0 )
		,	right( 0 )
		,	left( 0 )
		,	verticalScrollIndicatorPolicy(
				AbstractScrollArea::ScrollIndicatorAsNeeded )
		,	horizontalScrollIndicatorPolicy(
				AbstractScrollArea::ScrollIndicatorAsNeeded )
		,	paintVerticalScrollIndicator( false )
		,	paintHorizontalScrollIndicator( false )
		,	leftMouseButtonPressed( false )
		,	hIndicatorSize( 0 )
		,	hIndicatorPos( 0 )
		,	vIndicatorSize( 0 )
		,	vIndicatorPos( 0 )
		,	indicatorWidth( 3 )
	{
	}

	void init();
	void layoutChildren( const QStyleOption & opt );
	void normalizePosition();
	void drawHorizontalScrollIndicator( QPainter * p );
	void drawVerticalScrollIndicator( QPainter * p );
	void calcIndicators();
	void calcIndicator( int scrolledSize, int scrolledPos,
		int viewportSize, int & indicatorSize,
		int & indicatorPos );
	void scrollContentsBy( int dx, int dy );

	virtual ~AbstractScrollAreaPrivate()
	{
	}

	AbstractScrollArea * q;
	QColor indicatorColor;
	QWidget * viewport;
	QSize scrolledAreaSize;
	QPoint topLeftCorner;
	int top;
	int bottom;
	int right;
	int left;
	AbstractScrollArea::ScrollIndicatorPolicy verticalScrollIndicatorPolicy;
	AbstractScrollArea::ScrollIndicatorPolicy horizontalScrollIndicatorPolicy;
	bool paintVerticalScrollIndicator;
	bool paintHorizontalScrollIndicator;
	bool leftMouseButtonPressed;
	QPoint mousePos;
	int hIndicatorSize;
	int hIndicatorPos;
	int vIndicatorSize;
	int vIndicatorPos;
	int indicatorWidth;
}; // class AbstractScrollAreaPrivate

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PRIVATE__ABSTRACTSCROLLAREA_P_HPP__INCLUDED
