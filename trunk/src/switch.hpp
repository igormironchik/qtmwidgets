
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

#ifndef QTMWIDGETS__SWITCH_HPP__INCLUDED
#define QTMWIDGETS__SWITCH_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>


namespace QtMWidgets {

//
// Switch
//

class SwitchPrivate;

/*!
	Switch is a button with two states: on and off. It's
	usefull widget for switching state of something, for example
	to turn on or off some functionality of the application,
	i.e. in settings menu.

	Switch can has four states. Main states are two: AcceptedUncheck
	and AcceptedCheck. The difference between not accepted and accepted
	states is that that, for example, user can toggle button with a finger
	to the checked state but application needs some time to turn of the
	functionality conjugated with this button. And while this functionality
	will not be turned on and setState() will not be
	called button will be in the NotAcceptedCheck state. In this state button
	will be turned on but will not be highlighted, that tell the user that
	his action is in process. When application is done with turning on
	the conjugated functionality programmaticaly should be called setState()
	slot with AcceptedCheck as argument. And at this point Switch will
	highlight his checked state.
*/
class Switch
	:	public QWidget
{
	Q_OBJECT

	Q_ENUMS( State )

	/*!
		\property state

		\brief the state of the Switch button.

		Switch can has four states. Main states are two: AcceptedUncheck
		and AcceptedCheck. The difference between not accepted and accepted
		states is that that, for example, user can toggle button with a finger
		to the checked state but application needs some time to turn of the
		functionality conjugated with this button. And while this functionality
		will not be turned on and setState() will not be
		called button will be in the NotAcceptedCheck state. In this state button
		will be turned on but will not be highlighted, that tell the user that
		his action is in process. When application is done with turning on
		the conjugated functionality programmaticaly should be called setState()
		slot with AcceptedCheck as argument. And at this point Switch will
		highlight his checked state.
	*/
	Q_PROPERTY( State state READ state WRITE setState NOTIFY stateChanged USER true )
	/*!
		\property checked

		\brief is this button checked or not.

		For NotAcceptedCheck and AcceptedCheck
		this property will be true, and for NotAcceptedUncheck and
		AcceptedUncheck this property will be false.
	*/
	Q_PROPERTY( bool checked READ isChecked NOTIFY toggled )
	/*!
		\property onText

		\brief text that displays when button is checked.

		By default button doesn't display any text in checked state.
		I.e. this property is empty.
	*/
	Q_PROPERTY( QString onText READ onText WRITE setOnText )
	/*!
		\property offText

		\brief text that displays when button is unchecked.

		By default button doesn't display any text in unchecked state.
		I.e. this property is empty.
	*/
	Q_PROPERTY( QString offText READ offText WRITE setOffText )

signals:
	/*!
		This signal emits when user switches state of the button.
		If state of the button toggled programmaticaly this signal
		emits too. This signal emits as for not accepted states as
		for accepted states. For NotAcceptedCheck and AcceptedCheck
		\a checked will be true, and for NotAcceptedUncheck and
		AcceptedUncheck \a checked will be false.
	*/
	void toggled( bool checked );
	/*!
		This signal emits when user switches state of the button or
		state of the button toggled programmaticaly, or button accepted
		it's new state. \sa State

		\a state contains the button's new State.
	*/
	void stateChanged( int state );

public:
	/*!
		State of the button.
	*/
	enum State {
		//! Button is in not accepted unchecked state.
		NotAcceptedUncheck = 0,
		//! Button is in accepted unchecked state.
		AcceptedUncheck = 1,
		//! Button is in not accepted checked state.
		NotAcceptedCheck = 2,
		//! Button is in accepted checked state.
		AcceptedCheck = 3
	}; // enum State

public:
	Switch( QWidget * parent = 0 );
	explicit Switch( State st, QWidget * parent = 0 );

	~Switch();

	//! \return State of the button.
	State state() const;

	//! \return Is button checked?
	bool isChecked() const;

	//! \return Text that displays in checked state.
	QString onText() const;
	//! Set text that will be displayed in checked state.
	void setOnText( const QString & text );

	//! \return Text that displays in unchecked state.
	QString offText() const;
	//! Set text that will be displayed in unchecked state.
	void setOffText( const QString & text );

	QSize sizeHint() const;

public slots:
	/*!
		Set state of the button.
	*/
	void setState( State st );

protected:
	void paintEvent( QPaintEvent * event );
	void mousePressEvent( QMouseEvent * event );
	void mouseReleaseEvent( QMouseEvent * event );
	void mouseMoveEvent( QMouseEvent * event );

private:
	friend class SwitchPrivate;

	Q_DISABLE_COPY( Switch )

	QScopedPointer< SwitchPrivate > d;
}; // class Switch

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__SWITCH_HPP__INCLUDED
