
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// QtMWidgets include.
#include "progressbar.hpp"

// Qt include.
#include <QPainter>
#include <QVariantAnimation>
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
		,	grooveHeight( 3 )
		,	animation( 0 )
		,	animate( true )
	{
	}

	//! Init.
	void init();
	//! \return Is repaint required?
	bool repaintRequired() const;
	//! \return Groove rect.
	QRect grooveRect() const;

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
	//! Color used for painting animation.
	QColor animationColor;
	//! Busy animation.
	QVariantAnimation * animation;
	//! Need paint animation?
	bool animate;
}; // class ProgressBarPrivate

void
ProgressBarPrivate::init()
{
	highlightColor = q->palette().color( QPalette::Highlight );
	grooveColor = q->palette().color( QPalette::Dark );
	animationColor = q->palette().color( QPalette::Base );

	QSizePolicy sp( QSizePolicy::Expanding, QSizePolicy::Fixed );
	if( orientation == Qt::Vertical )
		sp.transpose();

	q->setSizePolicy( sp );

	animation = new QVariantAnimation( q );
	animation->setDuration( 500 );
	animation->setLoopCount( -1 );
	animation->setStartValue( 0.0 );
	animation->setEndValue( 1.0 );
	animation->start();
}

bool
ProgressBarPrivate::repaintRequired() const
{
	if( value == lastPaintedValue )
		return false;

	const int valueDifference = qAbs( value - lastPaintedValue );

	const int grooveBlock = ( orientation == Qt::Horizontal ? grooveRect().width() :
		grooveRect().height() );

	// This expression is basically
	// ( valueDifference / 1 > ( maximum - minimum ) / grooveBlock )
	// transformed to avoid integer division.
	return ( valueDifference * grooveBlock > ( maximum - minimum ) );
}

QRect
ProgressBarPrivate::grooveRect() const
{
	return q->rect();
}


//
// ProgressBar
//

ProgressBar::ProgressBar( QWidget * parent )
	:	QWidget( parent )
	,	d( new ProgressBarPrivate( this ) )
{
	d->init();

	connect( d->animation, &QVariantAnimation::valueChanged,
		this, &ProgressBar::_q_animation );
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

	d->animate = true;

	d->animation->start();

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
	if( value == d->minimum - 1 )
	{
		reset();

		return;
	}

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

	if( d->animate )
	{
		d->animate = false;

		d->animation->stop();

		repaint();
	}
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
	QPainter p( this );

	const QRect r = d->grooveRect();

	if( !d->animate )
	{
		d->lastPaintedValue = d->value;

		const int offset = (qreal) d->value / (qreal) ( d->maximum - d->minimum ) *
			( d->orientation == Qt::Horizontal ? r.width() : r.height() );

		const int x = ( ( d->orientation == Qt::Horizontal && d->invertedAppearance ) ?
			r.x() + r.width() - offset - 1 : 0 );
		const int y = ( ( d->orientation == Qt::Vertical && d->invertedAppearance ) ?
			r.y() + r.height() - offset - 1 : 0 );
		const int width = ( d->orientation == Qt::Horizontal ?
			offset : d->grooveHeight );
		const int height = ( d->orientation == Qt::Horizontal ?
			d->grooveHeight : offset );

		const QRect highlightedRect( x, y, width, height );
		const QRect baseRect = r.adjusted(
			( d->orientation == Qt::Horizontal && !d->invertedAppearance ?
				  width : 0 ),
			( d->orientation == Qt::Vertical && !d->invertedAppearance ?
				  height : 0 ),
			( d->orientation == Qt::Horizontal && d->invertedAppearance ?
				  -width : 0 ),
			( d->orientation == Qt::Vertical && d->invertedAppearance ?
				  -height : 0 ) );

		p.setPen( d->grooveColor );
		p.setBrush( d->grooveColor );

		p.drawRect( baseRect );

		p.setPen( d->highlightColor );
		p.setBrush( d->highlightColor );

		p.drawRect( highlightedRect );
	}
	else
	{
		const double value = d->animation->currentValue().toDouble();

		p.setPen( d->highlightColor );
		p.setBrush( d->highlightColor );

		p.drawRect( r );

		const QRect a1(
			( d->orientation == Qt::Horizontal ?
				r.x() + r.width() * value / 3.0 + d->grooveHeight :
				r.x() ),
			( d->orientation == Qt::Horizontal ? r.y() :
				r.y() + r.height() * value / 3.0 + d->grooveHeight ),
			d->grooveHeight, d->grooveHeight );

		const QRect a2(
			( d->orientation == Qt::Horizontal ?
				r.x() + r.width() * value / 1.5 + r.width() / 3 :
				r.x() ),
			( d->orientation == Qt::Horizontal ? r.y() :
				r.y() + r.height() * value / 1.5 + r.height() / 3 ),
			d->grooveHeight, d->grooveHeight );

		p.setPen( d->animationColor );
		p.setBrush( d->animationColor );

		p.drawRect( a1 );
		p.drawRect( a2 );
	}
}

void
ProgressBar::_q_animation( const QVariant & value )
{
	Q_UNUSED( value )

	update();
}

} /* namespace QtMWidgets */
