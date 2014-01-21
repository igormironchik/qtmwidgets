
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


namespace QtMWidgets {

//
// SwitchPrivate
//

class SwitchPrivate {
public:
	explicit SwitchPrivate( Switch * parent )
		:	q( parent )
		,	state( Switch::AcceptedUncheck )
	{
	}

	bool isChecked() const;
	void setState( Switch::State st );

	Switch * q;
	Switch::State state;
	QString onText;
	QString offText;
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

}


//
// Switch
//

Switch::Switch( QWidget * parent )
	:	QWidget( parent )
	,	d( new SwitchPrivate( this ) )
{
}

Switch::Switch( State st, QWidget * parent )
	:	QWidget( parent )
	,	d( new SwitchPrivate( this ) )
{
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

QSize
Switch::sizeHint() const
{
	return QSize();
}

void
Switch::paintEvent( QPaintEvent * )
{
}

void
Switch::mousePressEvent( QMouseEvent * event )
{
}

void
Switch::mouseReleaseEvent( QMouseEvent * event )
{
}

void
Switch::mouseMoveEvent( QMouseEvent * event )
{
}

} /* namespace QtMWidgets */
