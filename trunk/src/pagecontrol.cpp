
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
	{
		init();
	}

	//! Init.
	void init();

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
}; // class PageControlPrivate

void
PageControlPrivate::init()
{
	QStyleOption opt;
	opt.initFrom( q );

	currentPageIndicatorColor = opt.palette.color( QPalette::Highlight );
	pageIndicatorColor = lighterColor( currentPageIndicatorColor, 75 );
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
	return 0;
}

QSize
PageControl::minimumSizeHint() const
{
	return QSize();
}

QSize
PageControl::sizeHint() const
{
	return minimumSizeHint();
}

void
PageControl::setCurrentIndex( int index )
{

}

void
PageControl::setCount( int c )
{

}

void
PageControl::paintEvent( QPaintEvent * )
{

}

void
PageControl::resizeEvent( QResizeEvent * e )
{

}

} /* namespace QtMWidgets */
