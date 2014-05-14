
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
#include "navigationbar.hpp"


namespace QtMWidgets {

//
// NavigationBarPrivate
//

class NavigationBarPrivate {
public:
	NavigationBarPrivate( NavigationBar * parent )
		:	q( parent )
	{
	}

	NavigationBar * q;
}; // class NavigationBarPrivate


//
// NavigationBar
//

NavigationBar::NavigationBar( QWidget * parent )
	:	QStackedWidget( parent )
	,	d( new NavigationBarPrivate( this ) )
{
}

NavigationBar::~NavigationBar()
{
}

int
NavigationBar::setMainWidget( const QString & title,
	QWidget * widget )
{
	return 0;
}

int
NavigationBar::addWidget( int parentIndex, const QString & title,
	QWidget * widget )
{
	return 0;
}

void
NavigationBar::removeWidget( QWidget * widget )
{

}

int
NavigationBar::currentIndex() const
{
	return 0;
}

QWidget *
NavigationBar::currentWidget() const
{
	return 0;
}

int
NavigationBar::indexOf( QWidget * widget ) const
{
	return 0;
}

QWidget *
NavigationBar::widget( int index ) const
{
	return 0;
}

QSize
NavigationBar::minimumSizeHint() const
{
	return QSize();
}

QSize
NavigationBar::sizeHint() const
{
	return QSize();
}

void
NavigationBar::showScreen( int index )
{

}

void
NavigationBar::showPreviousScreen()
{

}

void
NavigationBar::showNextScreen()
{

}

} /* namespace QtMWidgets */
