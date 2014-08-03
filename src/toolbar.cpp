
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
#include "toolbar.hpp"


namespace QtMWidgets {

//
// ToolBarPrivate
//

class ToolBarPrivate {
public:
	explicit ToolBarPrivate( ToolBar * parent )
		:	q( parent )
		,	orientation( Qt::Horizontal )
	{
	}

	ToolBar * q;
	Qt::Orientation orientation;
	QSize iconSize;
}; // class ToolBarPrivate


//
// ToolBar
//

ToolBar::ToolBar( QWidget * parent )
	:	QWidget( parent )
	,	d( new ToolBarPrivate( this ) )
{
}

ToolBar::~ToolBar()
{
}

void
ToolBar::setOrientation( Qt::Orientation orientation )
{
	if( d->orientation != orientation )
	{
		d->orientation = orientation;

		emit orientationChanged( d->orientation );
	}
}

Qt::Orientation
ToolBar::orientation() const
{
	return d->orientation;
}

void clear();

QAction *
ToolBar::addAction( const QIcon & icon )
{
	return 0;
}

QAction *
ToolBar::addAction( const QIcon & icon,
	const QObject * receiver, const char * member )
{
	return 0;
}

QRect
ToolBar::actionGeometry( QAction * action ) const
{
	return QRect();
}

QAction *
ToolBar::actionAt( const QPoint & p ) const
{
	return 0;
}

QSize
ToolBar::iconSize() const
{
	return d->iconSize;
}

void
ToolBar::setIconSize( const QSize & iconSize )
{
	if( d->iconSize != iconSize )
	{
		d->iconSize = iconSize;

		emit iconSizeChanged( d->iconSize );
	}
}

void
ToolBar::actionEvent( QActionEvent * event )
{

}

} /* namespace QtMWidgets */
