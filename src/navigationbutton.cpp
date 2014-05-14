
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

	NavigationButton * q;
	NavigationButton::Direction direction;
}; // class NavigationButtonPrivate


//
// NavigationButton
//

NavigationButton::NavigationButton( Direction direction, QWidget * parent )
	:	QAbstractButton( parent )
	,	d( new NavigationButtonPrivate( this, direction ) )
{
	connect( this, SIGNAL( pressed() ), this, SLOT( _q_pressed() ) );
	connect( this, SIGNAL( released() ), this, SLOT( _q_released() ) );
	connect( this, SIGNAL( clicked() ), this, SLOT( _q_clicked() ) );
}

NavigationButton::~NavigationButton()
{
}

QSize
NavigationButton::minimumSizeHint() const
{
	return QSize();
}

QSize
NavigationButton::sizeHint() const
{
	return QSize();
}

void
NavigationButton::paintEvent( QPaintEvent * )
{

}

void
NavigationButton::_q_pressed()
{

}

void
NavigationButton::_q_released()
{

}

void
NavigationButton::_q_clicked()
{

}

} /* namespace QtMWidgets */
