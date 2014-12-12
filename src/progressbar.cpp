
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
#include "progressbar.hpp"

// Qt include.
#ifndef QT_NO_ACCESSIBILITY
#include <QAccessible>
#endif


namespace QtMWidgets {

//
// ProgressBarPrivate
//

class ProgressBarPrivate {
public:
	ProgressBarPrivate( ProgressBar * parent )
		:	q( parent )
		,	minimum( 0 )
		,	maximum( 0 )
		,	value( -1 )
		,	lastPaintedValue( -1 )
		,	orientation( Qt::Horizontal )
		,	invertedAppearance( false )
		,	grooveHeight( 2 )
	{
	}

	//! Init.
	void init();
	//! \return Is repaint required?
	bool repaintRequired() const;

	//! Parent;
	ProgressBar * q;
	//! Minimum.
	int minimum;
	//! Maximum.
	int maximum;
	//! Value.
	int value;
	//! Last painted value.
	int lastPaintedValue;
	//! Orientation.
	Qt::Orientation orientation;
	//! Inverted appearance?
	bool invertedAppearance;
	//! Groove height.
	int grooveHeight;
	//! Highlight color.
	QColor highlightColor;
	//! Groove color.
	QColor grooveColor;
}; // class ProgressBarPrivate

void
ProgressBarPrivate::init()
{
	highlightColor = q->palette().color( QPalette::Highlight );
	grooveColor = q->palette().color( QPalette::Dark );
}

bool
ProgressBarPrivate::repaintRequired() const
{
	if( value == lastPaintedValue )
		return false;

	const int valueDifference = qAbs( value - lastPaintedValue );

	int distance = 0;

	if( minimum <= 0 && maximum <= 0 )
		distance = qAbs( minimum - maximum );
	else
		distance = maximum - minimum;

	const int grooveBlock = ( orientation == Qt::Horizontal ? q->rect().width() :
		q->rect().height() );

	// This expression is basically
	// ( valueDifference / 1 > distance / grooveBlock )
	// transformed to avoid integer division.
	return ( valueDifference * grooveBlock > distance );
}


//
// ProgressBar
//

ProgressBar::ProgressBar( QWidget * parent )
	:	QWidget( parent )
	,	d( new ProgressBarPrivate( this ) )
{
	d->init();
}

ProgressBar::~ProgressBar()
{
}

int
ProgressBar::minimum() const
{
	return d->minimum;
}

int
ProgressBar::maximum() const
{
	return d->maximum;
}

int
ProgressBar::value() const
{
	return d->value;
}

Qt::Orientation
ProgressBar::orientation() const
{
	return d->orientation;
}

bool
ProgressBar::invertedAppearance() const
{
	return d->invertedAppearance;
}

void
ProgressBar::setInvertedAppearance( bool invert )
{
	if( d->invertedAppearance != invert )
	{
		d->invertedAppearance = invert;

		update();
	}
}

int
ProgressBar::grooveHeight() const
{
	return d->grooveHeight;
}

void
ProgressBar::setGrooveHeight( int h )
{
	if( h > 0 && d->grooveHeight != h )
	{
		d->grooveHeight = h;

		updateGeometry();
		update();
	}
}

const QColor &
ProgressBar::highlightColor() const
{
	return d->highlightColor;
}

void
ProgressBar::setHighlightColor( const QColor & c )
{
	if( d->highlightColor != c )
	{
		d->highlightColor = c;

		update();
	}
}

const QColor &
ProgressBar::grooveColor() const
{
	return d->grooveColor;
}

void
ProgressBar::setGrooveColor( const QColor & c )
{
	if( d->grooveColor != c )
	{
		d->grooveColor = c;

		update();
	}
}

QSize
ProgressBar::sizeHint() const
{
	return minimumSizeHint();
}

QSize
ProgressBar::minimumSizeHint() const
{
	QSize size( d->grooveHeight * 10, d->grooveHeight );

	if( d->orientation == Qt::Vertical )
		size.transpose();

	return size;
}

void
ProgressBar::reset()
{
	d->value = d->minimum - 1;

	if( d->minimum == INT_MIN )
		d->value = INT_MIN;

	repaint();
}

void
ProgressBar::setRange( int minimum, int maximum )
{
	if( minimum != d->minimum || maximum != d->maximum )
	{
		d->minimum = minimum;
		d->maximum = qMax( minimum, maximum );

		if( d->value < ( d->minimum - 1 ) || d->value > d->maximum )
			reset();
		else
			update();
	}
}

void
ProgressBar::setMinimum( int minimum )
{
	setRange( minimum, qMax( d->maximum, minimum ) );
}

void
ProgressBar::setMaximum( int maximum )
{
	setRange( qMin( d->minimum, maximum ), maximum );
}

void
ProgressBar::setValue( int value )
{
	if( d->value == value
		|| ( ( value > d->maximum || value < d->minimum )
			&& ( d->maximum != 0 || d->minimum != 0 ) ) )
		return;

	d->value = value;

	emit valueChanged( value );

#ifndef QT_NO_ACCESSIBILITY
	if( isVisible() )
	{
		QAccessibleValueChangeEvent event( this, value );
		QAccessible::updateAccessibility( &event );
	}
#endif

	if( d->repaintRequired() )
		repaint();
}

void
ProgressBar::setOrientation( Qt::Orientation orientation )
{
	if( d->orientation == orientation )
		return;

	d->orientation = orientation;

	QSizePolicy sp = sizePolicy();
	sp.transpose();
	setSizePolicy( sp );

	updateGeometry();
	update();
}

void
ProgressBar::paintEvent( QPaintEvent * )
{

}

} /* namespace QtMWidgets */
