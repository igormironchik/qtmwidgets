
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
#include "private/color.hpp"
#include "private/drawing.hpp"

// Qt include.
#include <QPainter>
#include <QStyle>
#include <QPalette>


namespace QtMWidgets {

//
// SliderPrivate
//

class SliderPrivate {
public:
	SliderPrivate( Slider * parent )
		:	q( parent )
		,	radius( 0 )
		,	grooveHeight( 5 )
	{
	}

	void init();
	inline void drawHandle( QPainter * p, const QRect & r,
		const QColor & borderColor, const QColor & lightColor );

	Slider * q;
	int radius;
	int grooveHeight;
}; // class SliderPrivate;

void
SliderPrivate::init()
{
	radius = q->fontMetrics().ascent();

	QSizePolicy sp( QSizePolicy::Expanding, QSizePolicy::Fixed );

	if( q->orientation() == Qt::Vertical )
		sp.transpose();

	q->setSizePolicy( sp );
}

void
SliderPrivate::drawHandle( QPainter * p, const QRect & r,
	const QColor & borderColor, const QColor & lightColor )
{
	drawSliderHandle( p, r, radius, radius, borderColor, lightColor );
}


//
// Slider
//

Slider::Slider( QWidget * parent )
	:	QAbstractSlider( parent )
	,	d( new SliderPrivate( this ) )
{
	setOrientation( Qt::Vertical );
	d->init();
}

Slider::Slider( Qt::Orientation orientation, QWidget * parent )
	:	QAbstractSlider( parent )
	,	d( new SliderPrivate( this ) )
{
	setOrientation( orientation );
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
	const int minWidth = d->radius * 20;
	const int height = d->radius * 2;

	int w = minWidth;
	int h = height;

	if( orientation() == Qt::Vertical )
	{
		w = height;
		h = minWidth;
	}

	return QSize( w, h );
}

void
Slider::paintEvent( QPaintEvent * )
{
	const QRect cr = contentsRect();

	// Position and size of the groove.
	int gx = 0, gy = 0, gw = 0, gh = 0;
	// Size of the highlighted groove.
	int grhw = 0, grhh = 0;
	// Position of the slider's handle.
	int hx = 0, hy = 0;

	if( orientation() == Qt::Vertical )
	{
		gx = cr.topLeft().x() + cr.width() / 2 - d->grooveHeight / 2;
		gy = cr.topLeft().y();
		gw = d->grooveHeight;
		gh = cr.height() - 1;

		hx = cr.topLeft().x() + cr.width() / 2 - d->radius;
		hy = QStyle::sliderPositionFromValue( minimum(), maximum(),
			sliderPosition(), gh - d->radius * 2,
			invertedAppearance() );

		grhw = 1;
		grhh = gh - hy - d->radius;
	}
	else
	{
		gx = cr.topLeft().x();
		gy = cr.topLeft().y() + cr.height() / 2 - d->grooveHeight / 2;
		gw = cr.width() - 1;
		gh = d->grooveHeight;

		hx = QStyle::sliderPositionFromValue( minimum(), maximum(),
			sliderPosition(), gw - d->radius * 2,
			invertedAppearance() );
		hy = cr.topLeft().y() + cr.height() / 2 - d->radius;

		grhw = gw - hx - d->radius;
		grhh = 1;
	}

	const QRect gr = QRect( gx, gy, gw, gh );

	const QRect grh = gr.marginsRemoved( QMargins( 1, 1, grhw, grhh ) );

	const QRect sh = QRect( hx, hy, d->radius * 2, d->radius * 2 );

	QPainter p( this );

	const QColor borderColor = palette().color( QPalette::Shadow );
	const QColor lightColor = palette().color( QPalette::Base );

	QLinearGradient g( QPointF( 0.0, 0.0 ), QPointF( 0.0, 1.0 ) );
	g.setCoordinateMode( QGradient::ObjectBoundingMode );
	g.setColorAt( 0.0, darkerColor( lightColor, 75 ) );
	g.setColorAt( 0.1, darkerColor( lightColor, 25 ) );
	g.setColorAt( 1.0, darkerColor( lightColor, 10 ) );

	p.setPen( borderColor );
	p.setBrush( g );

	p.drawRect( gr );

	p.setPen( Qt::NoPen );
	p.setBrush( palette().color( QPalette::Highlight ) );

	p.drawRect( grh );

	p.setRenderHint( QPainter::Antialiasing );
	d->drawHandle( &p, sh, borderColor, lightColor );
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
