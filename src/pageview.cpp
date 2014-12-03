
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
#include "pageview.hpp"
#include "pagecontrol.hpp"


namespace QtMWidgets {

//
// PageViewPrivate
//

class PageViewPrivate {
public:
	PageViewPrivate( PageView * parent )
		:	q( parent )
		,	currentIndex( -1 )
		,	count( 0 )
		,	showPageControl( true )
	{
	}

	//! Parent.
	PageView * q;
	//! Current index.
	int currentIndex;
	//! Count of pages.
	int count;
	//! Is PageControl visible?
	bool showPageControl;
}; // class PageViewPrivate


//
// PageView
//

PageView::PageView( QWidget * parent )
	:	QFrame( parent )
	,	d( new PageViewPrivate( this ) )
{
}

PageView::~PageView()
{
}

int
PageView::addWidget( QWidget * widget )
{
	return -1;
}

int
PageView::count() const
{
	return d->count;
}

int
PageView::currentIndex() const
{
	return d->currentIndex;
}

QWidget *
PageView::currentWidget() const
{
	return 0;
}

int
PageView::indexOf( QWidget * widget ) const
{
	return -1;
}

int
PageView::insertWidget( int index, QWidget * widget )
{
	return -1;
}

void
PageView::removeWidget( QWidget * widget )
{

}

QWidget *
PageView::widget( int index ) const
{
	return 0;
}

bool
PageView::showPageControl() const
{
	return d->showPageControl;
}

void
PageView::setShowPageControl( bool show )
{
	if( d->showPageControl != show )
	{
		d->showPageControl = show;
	}
}

void
PageView::setCurrentIndex( int index )
{

}

void
PageView::setCurrentWidget( QWidget * widget )
{

}

} /* namespace QtMWidgets */
