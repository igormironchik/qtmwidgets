
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
#include "switch.hpp"
#include "private/color.hpp"

// Qt include.
#include <QStyleOption>
#include <QPainter>
#include <QBitmap>
#include <QRegion>
#include <QLinearGradient>
#include <QMouseEvent>


namespace QtMWidgets {

//
// SwitchPrivate
//

class SwitchPrivate {
public:
	explicit SwitchPrivate( Switch * parent )
		:	q( parent )
		,	state( Switch::AcceptedUncheck )
		,	radius( 0 )
		,	textWidth( 0 )
		,	offset( 0 )
		,	leftMouseButtonPressed( false )
	{
		init();
	}

	bool isChecked() const;
	void setState( Switch::State st );
	void init();
	void emitSignals();
	void initPaintRegion( const QRect & r );
	void drawText( QPainter * p, const QStyleOption & opt,
		const QColor & on, const QColor & off );
	void drawSlider( QPainter * p, const QStyleOption & opt,
		const QColor & lightColor, const QColor & borderColor );
	void initOffset( const QRect & r );

	Switch * q;
	Switch::State state;
	QString onText;
	QString offText;
	QColor onColor;
	int radius;
	int textWidth;
	QRegion paintRegion;
	int offset;
	bool leftMouseButtonPressed;
	QPoint mousePos;
}; // class SwitchPrivate

bool
SwitchPrivate::isChecked() const
{
	switch( state )
	{
		case Switch::NotAcceptedUncheck :
		case Switch::AcceptedUncheck :
			return false;

		case Switch::NotAcceptedCheck :
		case Switch::AcceptedCheck :
			return true;

		default :
			return false;
	}
}

void
SwitchPrivate::setState( Switch::State st )
{
	if( state != st )
	{
		state = st;

		QStyleOption opt;
		opt.initFrom( q );

		initOffset( opt.rect );

		emitSignals();

		q->update();
	}
}

void
SwitchPrivate::init()
{
	QStyleOption opt;
	opt.initFrom( q );

	onColor = opt.palette.color( QPalette::Highlight );
}

void
SwitchPrivate::emitSignals()
{
	emit q->toggled( isChecked() );
	emit q->stateChanged( (int) state );
}

void
SwitchPrivate::initPaintRegion( const QRect & r )
{
	QBitmap mask( r.width(), r.height() );
	mask.clear();

	QPainter p( &mask );
	p.setRenderHint( QPainter::Antialiasing );
	p.setPen( Qt::color1 );
	p.setBrush( Qt::color1 );
	p.drawRoundedRect( -1, -1, r.width() + 2,
		radius * 2 + 2, radius + 1, radius + 1 );

	paintRegion = QRegion( mask );
}

void
SwitchPrivate::drawText( QPainter * p, const QStyleOption & opt,
	const QColor & on, const QColor & off )
{
	const QRect r( radius, 0, opt.rect.width() - radius * 2, radius * 2 );

	switch( state )
	{
		case Switch::NotAcceptedCheck :
		case Switch::AcceptedCheck :
		{
			if( !onText.isEmpty() )
			{
				p->setPen( on );
				p->drawText( r,
					Qt::AlignLeft | Qt::TextSingleLine | Qt::AlignVCenter,
					onText );
			}
		}
		break;

		case Switch::NotAcceptedUncheck :
		case Switch::AcceptedUncheck :
		{
			if( !offText.isEmpty() )
			{
				p->setPen( off );
				p->drawText( r,
					Qt::AlignRight | Qt::TextSingleLine | Qt::AlignVCenter,
					offText );
			}
		}
		break;
	}
}

void
SwitchPrivate::drawSlider( QPainter * p, const QStyleOption & opt,
	const QColor & lightColor, const QColor & borderColor )
{
	Q_UNUSED( opt )

	p->setPen( QPen( borderColor, 2 ) );
	p->setBrush( lightColor );
	p->drawRoundedRect( offset + 1, 1, radius * 2 - 2, radius * 2 - 2,
		radius, radius );

	QLinearGradient g( QPointF( 0.0, 0.0 ), QPointF( 0.0, 1.0 ) );
	g.setCoordinateMode( QGradient::ObjectBoundingMode );
	g.setColorAt( 0.0, darkerColor( lightColor, 75 ) );
	g.setColorAt( 1.0, darkerColor( lightColor, 10 ) );

	p->setPen( Qt::NoPen );
	p->setBrush( g );

	p->drawRoundedRect( offset + 2 + 1, 2 + 1, radius * 2 - 4 - 2,
		radius * 2 - 4 - 2, radius - 4, radius - 4 );
}

void
SwitchPrivate::initOffset( const QRect & r )
{
	switch( state )
	{
		case Switch::NotAcceptedCheck :
		case Switch::AcceptedCheck :
			offset = r.width() - radius * 2;
		break;

		case Switch::NotAcceptedUncheck :
		case Switch::AcceptedUncheck :
			offset = 0;
		break;
	}
}


//
// Switch
//

Switch::Switch( QWidget * parent )
	:	QWidget( parent )
	,	d( new SwitchPrivate( this ) )
{
	setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
}

Switch::Switch( State st, QWidget * parent )
	:	QWidget( parent )
	,	d( new SwitchPrivate( this ) )
{
	setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );

	setState( st );
}

Switch::~Switch()
{
}

Switch::State
Switch::state() const
{
	return d->state;
}

bool
Switch::isChecked() const
{
	return d->isChecked();
}

QString
Switch::onText() const
{
	return d->onText;
}

void
Switch::setOnText( const QString & text )
{
	if( d->onText != text )
	{
		d->onText = text;
		updateGeometry();
	}
}

QString
Switch::offText() const
{
	return d->offText;
}

void
Switch::setOffText( const QString & text )
{
	if( d->offText != text )
	{
		d->offText = text;
		updateGeometry();
	}
}

void
Switch::setState( State st )
{
	d->setState( st );
}

QColor
Switch::onColor() const
{
	return d->onColor;
}

void
Switch::setOnColor( const QColor & c )
{
	if( d->onColor != c )
	{
		d->onColor = c;

		switch( d->state )
		{
			case AcceptedCheck :
			case NotAcceptedUncheck :
				update();
			break;
		}
	}
}

QSize
Switch::sizeHint() const
{
	QStyleOption opt;
	opt.initFrom( this );

	d->radius = opt.fontMetrics.ascent();

	int offWidth = 0;
	int onWidth = 0;

	if( !d->offText.isEmpty() )
		offWidth = opt.fontMetrics.boundingRect( d->offText ).width();

	if( !d->onText.isEmpty() )
		onWidth = opt.fontMetrics.boundingRect( d->onText ).width();

	d->textWidth = qMax( offWidth, onWidth );

	const QRect r( 0, 0,
		d->textWidth + d->radius * 3 + d->radius / 2, d->radius * 2 );

	d->initPaintRegion( r );
	d->initOffset( r );

	return QSize( r.width(), r.height() );
}

void
Switch::paintEvent( QPaintEvent * )
{
	QStyleOption opt;
	opt.initFrom( this );

	QColor borderColor = opt.palette.color( QPalette::Shadow );
	QColor lightColor = opt.palette.color( QPalette::Base );

	QPainter p( this );
	p.setRenderHint( QPainter::Antialiasing );
	p.setClipRegion( d->paintRegion );

	switch( d->state )
	{
		case NotAcceptedUncheck :
		case AcceptedUncheck :
		{
			QLinearGradient g( QPointF( 0.0, 0.0 ), QPointF( 0.0, 1.0 ) );
			g.setCoordinateMode( QGradient::ObjectBoundingMode );
			g.setColorAt( 0.0, darkerColor( lightColor, 75 ) );
			g.setColorAt( 0.1, darkerColor( lightColor, 25 ) );
			g.setColorAt( 1.0, darkerColor( lightColor, 10 ) );

			p.setBrush( g );
		}
		break;

		case NotAcceptedCheck :
			p.setBrush( darkerColor( d->onColor, 50 ) );
		break;

		case AcceptedCheck :
			p.setBrush( d->onColor );
		break;
	}

	p.setPen( borderColor );

	p.drawRoundedRect( 0, 0, opt.rect.width(),
		d->radius * 2, d->radius, d->radius );

	d->drawText( &p, opt, lightColor, borderColor );

	QColor lightAlphaColor = lightColor;
	lightAlphaColor.setAlpha( 50 );

	p.setPen( Qt::NoPen );
	p.setBrush( lightAlphaColor );

	p.drawRoundedRect( d->radius / 4, d->radius,
		opt.rect.width() - d->radius / 2, d->radius * 2,
		d->radius / 2, d->radius / 2 );

	d->drawSlider( &p, opt, lightColor, borderColor );
}

void
Switch::mousePressEvent( QMouseEvent * event )
{
	if( event->button() == Qt::LeftButton )
	{
		d->mousePos = event->pos();
		d->leftMouseButtonPressed = true;
	}

	event->accept();
}

void
Switch::mouseReleaseEvent( QMouseEvent * event )
{
	d->leftMouseButtonPressed = false;

	const int distance = rect().width() - d->radius * 2;

	if( d->offset <= distance / 2 )
	{
		switch( d->state )
		{
			case NotAcceptedCheck :
			case AcceptedCheck :
				d->setState( NotAcceptedUncheck );
			break;

			default :
			{
				d->initOffset( rect() );
				update();
			}
			break;
		}
	}
	else
	{
		switch( d->state )
		{
			case NotAcceptedUncheck :
			case AcceptedUncheck :
				d->setState( NotAcceptedCheck );
			break;

			default :
			{
				d->initOffset( rect() );
				update();
			}
			break;
		}
	}

	event->accept();
}

void
Switch::mouseMoveEvent( QMouseEvent * event )
{
	if( d->leftMouseButtonPressed )
	{
		const int delta = event->pos().x() - d->mousePos.x();
		d->offset += delta;
		d->mousePos = event->pos();

		if( d->offset < 0 )
			d->offset = 0;
		else if( d->offset > rect().width() - d->radius * 2 )
			d->offset = rect().width() - d->radius * 2;

		update();
	}

	event->accept();
}

} /* namespace QtMWidgets */
