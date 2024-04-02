
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// QtMWidgets include.
#include "stepper.hpp"
#include "fingergeometry.hpp"
#include "color.hpp"

// Qt include.
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QStyleOption>
#include <QTimer>


namespace QtMWidgets {

static const int defaultTimeout = 500;
static const int fastTimeout = 250;
static const unsigned int autorepeatChangeTimeoutCount = 5;

//
// StepperPrivate
//

class StepperPrivate {
public:
	StepperPrivate( Stepper * parent )
		:	q( parent )
		,	minimum( 0 )
		,	maximum( 99 )
		,	value( 0 )
		,	singleStep( 1 )
		,	wrapping( false )
		,	autorepeat( true )
		,	minusButtonEnabled( false )
		,	plusButtonEnabled( true )
		,	button( NoButton )
		,	timer( 0 )
		,	timeout( defaultTimeout )
		,	autorepeatCount( 0 )
	{
		init();
	}

	//! Button.
	enum Button {
		//! No button.
		NoButton = 0,
		//! Minus.
		Minus = 1,
		//! Plus.
		Plus = 2
	}; // enum Button

	//! Bound value.
	int bound( int val ) const;
	//! Init.
	void init();
	//! Enable/disable buttons.
	void enableButtons();
	//! \return Button pressed.
	Button buttonPressed( const QPoint & pos );
	//! Make click.
	void makeClick();

	//! Parent.
	Stepper * q;
	//! Minimum value.
	int minimum;
	//! Maximum value.
	int maximum;
	//! Current value.
	int value;
	//! Single step.
	int singleStep;
	//! Is wrapping mode enabled?
	bool wrapping;
	//! Is uatorepeat mode enabled?
	bool autorepeat;
	//! Color.
	QColor color;
	//! Is minus button enabled?
	bool minusButtonEnabled;
	//! Is plus button enabled?
	bool plusButtonEnabled;
	//! Pressed button.
	Button button;
	//! Timer.
	QTimer * timer;
	//! Autorepeat timeout.
	int timeout;
	//! Autorepeat count.
	unsigned int autorepeatCount;
}; // class StepperPrivate

int
StepperPrivate::bound( int val ) const
{
	int v = val;

	if( v < minimum )
		v = wrapping ? maximum : minimum;
	else if( v > maximum )
		v = wrapping ? minimum : maximum;

	return v;
}

void
StepperPrivate::init()
{
	QStyleOption opt;
	opt.initFrom( q );

	color = opt.palette.color( QPalette::Highlight );

	timer = new QTimer( q );
}

void
StepperPrivate::enableButtons()
{
	bool needUpdate = false;

	if( !wrapping )
	{
		if( value == minimum )
		{
			needUpdate = minusButtonEnabled || !plusButtonEnabled;

			minusButtonEnabled = false;
			plusButtonEnabled = true;

			if( button == Minus )
				timer->stop();
		}
		else if( value == maximum )
		{
			needUpdate = !minusButtonEnabled || plusButtonEnabled;

			minusButtonEnabled = true;
			plusButtonEnabled = false;

			if( button == Plus )
				timer->stop();
		}
		else
		{
			needUpdate = !minusButtonEnabled || !plusButtonEnabled;

			minusButtonEnabled = true;
			plusButtonEnabled = true;
		}
	}
	else
	{
		needUpdate = !minusButtonEnabled || !plusButtonEnabled;

		minusButtonEnabled = true;
		plusButtonEnabled = true;
	}

	if( needUpdate )
		q->update();
}

StepperPrivate::Button
StepperPrivate::buttonPressed( const QPoint & pos )
{
	QRect minus = q->rect();
	minus.setRight( minus.left() + minus.width() / 2 - 1 );
	QRect plus = q->rect();
	plus.setLeft( minus.right() + 1 );

	if( minus.contains( pos ) )
		return Minus;
	else if( plus.contains( pos ) )
		return Plus;
	else
		return NoButton;
}

void
StepperPrivate::makeClick()
{
	switch( button )
	{
		case StepperPrivate::Minus :
		{
			if( minusButtonEnabled )
				q->stepBy( -1 );
		}
			break;

		case StepperPrivate::Plus :
		{
			if( plusButtonEnabled )
				q->stepBy( 1 );
		}
			break;

		default:
			break;
	}
}



//
// Stepper
//

Stepper::Stepper( QWidget * parent )
	:	QWidget( parent )
	,	d( new StepperPrivate( this ) )
{
	setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );

	connect( d->timer, &QTimer::timeout,
		this, &Stepper::_q_autorepeat );
}

Stepper::~Stepper()
{
}

int
Stepper::maximum() const
{
	return d->maximum;
}

void
Stepper::setMaximum( int m )
{
	setRange( d->minimum, m );
}

int
Stepper::minimum() const
{
	return d->minimum;
}

void
Stepper::setMinimum( int m )
{
	setRange( m, d->maximum );
}

void
Stepper::setRange( int min, int max )
{
	d->minimum = min < max ? min : max;
	d->maximum = max < min ? min : max;

	const int oldValue = d->value;

	d->value = d->bound( d->value );

	d->enableButtons();

	if( d->value != oldValue )
		emit valueChanged( d->value );
}

void
Stepper::stepBy( int step )
{
	const int old = d->value;

	d->value = d->bound( step * d->singleStep + d->value );

	d->enableButtons();

	if( old != d->value )
		emit valueChanged( d->value );
}

int
Stepper::singleStep() const
{
	return d->singleStep;
}

void
Stepper::setSingleStep( int step )
{
	if( step >= 0 )
		d->singleStep = step;
}

int
Stepper::value() const
{
	return d->value;
}

bool
Stepper::wrapping() const
{
	return d->wrapping;
}

void
Stepper::setWrapping( bool enabled )
{
	d->wrapping = enabled;

	if( d->wrapping )
	{
		const bool needUpdate = !d->minusButtonEnabled || !d->plusButtonEnabled;

		d->minusButtonEnabled = true;
		d->plusButtonEnabled = true;

		if( needUpdate )
			update();
	}
}

bool
Stepper::autorepeat() const
{
	return d->autorepeat;
}

void
Stepper::setAutorepeat( bool enabled )
{
	d->autorepeat = enabled;
}

const QColor &
Stepper::color() const
{
	return d->color;
}

void
Stepper::setColor( const QColor & c )
{
	if( d->color != c )
	{
		d->color = c;

		update();
	}
}

QSize
Stepper::sizeHint() const
{
	return minimumSizeHint();
}

QSize
Stepper::minimumSizeHint() const
{
	return QSize( qRound( (qreal) FingerGeometry::width() * 0.75 * 2.0 ),
		qRound( (qreal) FingerGeometry::height() * 0.5 ) );
}

void
Stepper::setValue( int val )
{
	const int old = d->value;

	d->value = d->bound( val );

	d->enableButtons();

	if( old != d->value )
		emit valueChanged( d->value );
}

void
Stepper::stepDown()
{
	stepBy( -1 );
}

void
Stepper::stepUp()
{
	stepBy( 1 );
}

void
Stepper::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	p.setRenderHint( QPainter::Antialiasing );
	p.translate( 1.0, 1.0 );

	const qreal width = rect().width() - 2;
	const qreal height = rect().height() - 2;
	const qreal arcSize = width * 0.2;

	QPainterPath minusButton;
	minusButton.moveTo( width / 2.0, height );
	minusButton.lineTo( arcSize / 2.0, height );
	minusButton.arcTo( 0.0, height - arcSize, arcSize, arcSize,
		-90.0, -90.0 );
	minusButton.lineTo( 0.0, arcSize / 2.0 );
	minusButton.arcTo( 0.0, 0.0, arcSize, arcSize, -180.0, -90.0 );
	minusButton.lineTo( width / 2.0, 0.0 );
	minusButton.closeSubpath();

	QPainterPath plusButton;
	plusButton.moveTo( width / 2.0, 0.0 );
	plusButton.lineTo( width - arcSize / 2.0, 0.0 );
	plusButton.arcTo( width - arcSize, 0.0, arcSize, arcSize,
		90.0, -90.0 );
	plusButton.lineTo( width, height - arcSize / 2.0 );
	plusButton.arcTo( width - arcSize, height - arcSize,
		arcSize, arcSize, 0.0, -90.0 );
	plusButton.lineTo( width / 2.0, height );
	plusButton.closeSubpath();

	p.setPen( d->color );
	p.setBrush( Qt::white );

	p.drawPath( minusButton );
	p.drawPath( plusButton );

	const int signSize = qRound( height / 4.0 );
	const int minusCenterX = qRound( width / 4.0 );
	const int centerY = qRound( height / 2.0 );
	const int plusCenterX = minusCenterX * 3;

	if( !d->minusButtonEnabled )
		p.setPen( QPen( lighterColor( d->color, 50 ), 2 ) );
	else
		p.setPen( QPen( d->color, 2 ) );

	p.drawLine( minusCenterX - signSize, centerY,
		minusCenterX + signSize, centerY );

	if( !d->plusButtonEnabled )
		p.setPen( QPen( lighterColor( d->color, 50 ), 2 ) );
	else
		p.setPen( QPen( d->color, 2 ) );

	p.drawLine( plusCenterX - signSize, centerY,
		plusCenterX + signSize, centerY );
	p.drawLine( plusCenterX, centerY - signSize,
		plusCenterX, centerY + signSize );
}

void
Stepper::mousePressEvent( QMouseEvent * event )
{
	if( event->button() == Qt::LeftButton )
	{
		d->timer->stop();

		d->autorepeatCount = 0;
		d->timeout = defaultTimeout;

		d->button = d->buttonPressed( event->pos() );

		d->makeClick();

		if( d->autorepeat )
			d->timer->start( d->timeout );

		event->accept();
	}
	else
		event->ignore();
}

void
Stepper::mouseReleaseEvent( QMouseEvent * event )
{
	if( event->button() == Qt::LeftButton )
	{
		d->button = StepperPrivate::NoButton;

		d->timer->stop();

		event->accept();
	}
	else
		event->ignore();
}

void
Stepper::_q_autorepeat()
{
	++d->autorepeatCount;

	if( d->autorepeatCount == autorepeatChangeTimeoutCount )
	{
		d->timeout = fastTimeout;

		d->timer->stop();

		d->timer->start( d->timeout );
	}

	d->makeClick();
}

} /* namespace QtMWidgets */
