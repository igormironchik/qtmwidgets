
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
#include "pagecontrol.hpp"
#include "fingergeometry.hpp"
#include "color.hpp"

// Qt include.
#include <QStyleOption>
#include <QVector>
#include <QResizeEvent>
#include <QPainter>
#include <QMouseEvent>


namespace QtMWidgets {

//
// PageControlPrivate
//

class PageControlPrivate {
public:
	PageControlPrivate( PageControl * parent )
		:	q( parent )
		,	count( 0 )
		,	currentIndex( -1 )
		,	radius( 0 )
		,	smallRadius( 0 )
		,	buttonSize( 0 )
		,	linesCount( 0 )
		,	countInOneLine( 0 )
		,	countInLastLine( 0 )
		,	widgetWidth( 0 )
		,	leftButtonPressed( false )
	{
		init();
	}

	//! Init.
	void init();
	//! Update buttons info.
	void updateButtonsInfo( int width );
	//! Update buttons geometry.
	void updateButtonsGeometry();
	//! Update buttons geometry for the given line.
	void updateButtonGeometryForLine( int line );
	//! \return Index of the button for the given pos.
	int findButton( const QPoint & pos );

	//! Parent.
	PageControl * q;
	//! Count.
	int count;
	//! Current index.
	int currentIndex;
	//! Page indicator color.
	QColor pageIndicatorColor;
	//! Current page indicator color.
	QColor currentPageIndicatorColor;
	//! Indicator radius.
	int radius;
	//! Small indicator radius.
	int smallRadius;
	//! Size of the button with indicator.
	int buttonSize;
	//! Count of lines.
	int linesCount;
	//! Count of items in one line.
	int countInOneLine;
	//! Count of items in the last line.
	int countInLastLine;
	//! Width of the widget.
	int widgetWidth;
	//! Rectangles of the buttons.
	QVector< QRect > rectangles;
	//! Click position.
	QPoint clickPos;
	//! Left mouse button was pressed.
	bool leftButtonPressed;
}; // class PageControlPrivate

void
PageControlPrivate::init()
{
	QStyleOption opt;
	opt.initFrom( q );

	currentPageIndicatorColor = opt.palette.color( QPalette::Highlight );
	pageIndicatorColor = lighterColor( currentPageIndicatorColor, 75 );

	radius = FingerGeometry::width() * 0.3 / 2;

	smallRadius = radius * 0.75;

	buttonSize = radius * 3;
}

void
PageControlPrivate::updateButtonsInfo( int width )
{
	if( width <= 0 )
		widgetWidth = q->rect().width();
	else
		widgetWidth = width;

	countInOneLine = widgetWidth / buttonSize;

	if( countInOneLine > 1 )
	{
		countInLastLine = count % countInOneLine;

		linesCount = count / countInOneLine + ( countInLastLine > 0 ? 1 : 0 );

		if( countInLastLine == 0 )
			countInLastLine = countInOneLine;
	}
}

void
PageControlPrivate::updateButtonsGeometry()
{
	for( int line = 0; line < linesCount; ++line )
		updateButtonGeometryForLine( line );
}

void
PageControlPrivate::updateButtonGeometryForLine( int line )
{
	int count = countInOneLine;

	if( line == linesCount - 1 )
		count = countInLastLine;

	int index = line * countInOneLine;

	const int offset = ( widgetWidth - count * buttonSize ) / 2;

	const int y = line * buttonSize;

	const int size = buttonSize - 1;

	for( int i = 0; i < count; ++i )
	{
		rectangles[ index ] = QRect( offset + i * buttonSize, y,
			size, size );

		++index;
	}
}

int
PageControlPrivate::findButton( const QPoint & pos )
{
	for( int i = 0; i < count; ++i )
	{
		if( rectangles[ i ].contains( pos ) )
			return i;
	}

	return -1;
}


//
// PageControl
//

PageControl::PageControl( QWidget * parent )
	:	QWidget( parent )
	,	d( new PageControlPrivate( this ) )
{
}

PageControl::~PageControl()
{
}

int
PageControl::count() const
{
	return d->count;
}

int
PageControl::currentIndex() const
{
	return d->currentIndex;
}

const QColor &
PageControl::pageIndicatorColor() const
{
	return d->pageIndicatorColor;
}

void
PageControl::setPageIndicatorColor( const QColor & c )
{
	if( d->pageIndicatorColor != c )
	{
		d->pageIndicatorColor = c;

		update();
	}
}

const QColor &
PageControl::currentPageIndicatorColor() const
{
	return d->currentPageIndicatorColor;
}

void
PageControl::setCurrentPageIndicatorColor( const QColor & c )
{
	if( d->currentPageIndicatorColor != c )
	{
		d->currentPageIndicatorColor = c;

		update();
	}
}

bool
PageControl::hasHeightForWidth() const
{
	return true;
}

int
PageControl::heightForWidth( int width ) const
{
	d->updateButtonsInfo( width );

	return d->linesCount * d->buttonSize;
}

QSize
PageControl::minimumSizeHint() const
{
	return QSize( d->count * d->buttonSize, d->buttonSize );
}

QSize
PageControl::sizeHint() const
{
	return minimumSizeHint();
}

void
PageControl::setCurrentIndex( int index )
{
	if( d->currentIndex != index && index >= 0 && index < d->count )
	{
		const int prev = d->currentIndex;

		d->currentIndex = index;

		emit currentChanged( d->currentIndex, prev );

		update();
	}
}

void
PageControl::setCount( int c )
{
	if( d->count != c && c >= 0 )
	{
		d->count = c;

		d->updateButtonsInfo( rect().width() );

		d->rectangles.clear();

		if( d->count > 0 )
		{
			d->rectangles.reserve( d->count );

			d->rectangles.fill( QRect(), d->count );

			d->updateButtonsGeometry();

			if( d->currentIndex >= d->count )
				setCurrentIndex( d->count - 1 );
		}
		else
		{
			const int prev = d->currentIndex;

			d->currentIndex = -1;

			emit currentChanged( d->currentIndex, prev );
		}
	}
}

void
PageControl::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	p.setRenderHint( QPainter::Antialiasing );
	p.setPen( d->pageIndicatorColor );

	for( int i = 0; i < d->count; ++i )
	{
		if( i == d->currentIndex )
		{
			p.setBrush( d->currentPageIndicatorColor );

			p.drawEllipse( d->rectangles[ i ].center(), d->radius, d->radius );
		}
		else
		{
			p.setBrush( d->pageIndicatorColor );

			p.drawEllipse( d->rectangles[ i ].center(),
				d->smallRadius, d->smallRadius );
		}
	}
}

void
PageControl::resizeEvent( QResizeEvent * e )
{
	d->updateButtonsInfo( e->size().width() );

	d->updateButtonsGeometry();

	update();

	e->accept();
}

void
PageControl::mousePressEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
	{
		d->leftButtonPressed = true;

		d->clickPos = e->pos();

		e->accept();
	}
	else
		e->ignore();
}

void
PageControl::mouseReleaseEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
	{
		if( d->leftButtonPressed )
		{
			const QPoint pos = e->pos() - d->clickPos;

			if( pos.manhattanLength() <= FingerGeometry::touchBounce() )
			{
				const int index = d->findButton( e->pos() );

				if( index >= 0 )
					setCurrentIndex( index );
			}
		}

		d->leftButtonPressed = false;

		e->accept();
	}
	else
		e->ignore();
}

} /* namespace QtMWidgets */
