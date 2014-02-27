
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
#include "slider.hpp"


namespace QtMWidgets {

//
// SliderPrivate
//

class SliderPrivate {
public:
	SliderPrivate( Slider * parent )
		:	q( parent )
		,	orientation( Qt::Vertical )
		,	radius( 0 )
		,	grooveHeight( 10 )
	{
	}

	void init();

	Slider * q;
	Qt::Orientation orientation;
	int radius;
	int grooveHeight;
}; // class SliderPrivate;

void
SliderPrivate::init()
{
	radius = q->fontMetrics().ascent();

	QSizePolicy sp( QSizePolicy::Expanding, QSizePolicy::Fixed );

	if( orientation == Qt::Vertical )
		sp.transpose();

	q->setSizePolicy( sp );
}


//
// Slider
//

Slider::Slider( QWidget * parent )
	:	QAbstractSlider( parent )
	,	d( new SliderPrivate( this ) )
{
	d->init();
}

Slider::Slider( Qt::Orientation orientation, QWidget * parent )
	:	QAbstractSlider( parent )
	,	d( new SliderPrivate( this ) )
{
	d->orientation = orientation;
	d->init();
}

Slider::~Slider()
{
}

int
Slider::handleRadius() const
{
	return d->radius;
}

void
Slider::setHandleRadius( int r )
{
	if( d->radius != r && r >= d->grooveHeight )
	{
		d->radius = r;
		updateGeometry();
		update();
	}
}

QSize
Slider::sizeHint() const
{
	return minimumSizeHint();
}

QSize
Slider::minimumSizeHint() const
{
	const int minWidth = d->radius * 10;
	const int height = d->radius;

	int w = minWidth;
	int h = height;

	if( d->orientation == Qt::Vertical )
	{
		w = height;
		h = minWidth;
	}

	return QSize( w, h );
}

void
Slider::paintEvent( QPaintEvent * e )
{

}

void
Slider::mousePressEvent( QMouseEvent * e )
{

}

void
Slider::mouseReleaseEvent( QMouseEvent * e )
{

}

void
Slider::mouseMoveEvent( QMouseEvent * e )
{

}

} /* namespace QtMWidgets */
