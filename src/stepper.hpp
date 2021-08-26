
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

#ifndef QTMWIDGETS__STEPPER_HPP__INCLUDED
#define QTMWIDGETS__STEPPER_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>


namespace QtMWidgets {

//
// Stepper
//

class StepperPrivate;

/*!
	A stepper lets the user adjust a value by increasing and decreasing
	it in small steps. Steppers are used in situations where a user needs
	to adjust a value by a small amount.
*/
class Stepper
	:	public QWidget
{
	Q_OBJECT

	/*!
		\property maximum
		\brief Maximum value of the stepper

		When setting this property the minimum is adjusted if necessary,
		to ensure that the range remains valid.

		The default maximum value is 99.
	*/
	Q_PROPERTY( int maximum READ maximum WRITE setMaximum )
	/*!
		\property minimum
		\brief Minimum value of the stepper

		When setting this property the maximum is adjusted if necessary
		to ensure that the range remains valid.

		The default minimum value is 0.
	*/
	Q_PROPERTY( int minimum READ minimum WRITE setMinimum )
	/*!
		\property singleStep
		\brief This property holds the step value

		When the user uses the buttons to change the stepper's value the
		value will be incremented/decremented by the amount of the singleStep.
		The default value is 1. Setting a singleStep value of less than 0
		does nothing.
	*/
	Q_PROPERTY( int singleStep READ singleStep WRITE setSingleStep )
	/*!
		\proerty value
		\brief This property holds the value of the stepper

		setValue() will emit valueChanged() if the new value is different
		from the old one.
	*/
	Q_PROPERTY( int value READ value WRITE setValue )
	/*!
		\property wrapping
		\brief This property holds whether the stepper is circular

		If wrapping is true stepping up from maximum() value will take
		you to the minimum() value and vica versa. Wrapping only make sense
		if you have minimum() and maximum() values set.
	*/
	Q_PROPERTY( bool wrapping READ wrapping WRITE setWrapping )
	/*!
		\property autorepeat
		\brief This property holds whether the stepper is autorepeat

		A stepper defaults to autorepeat, which means that pressing and
		holding one of its buttons increments or decrements the stepper’s
		value repeatedly. The rate of change depends on how long the user
		continues pressing the control. The user can hold the stepper to
		quickly approach a desired value, and then increment or decrement
		to the desired value.
	*/
	Q_PROPERTY( bool autorepeat READ autorepeat WRITE setAutorepeat )
	/*!
		\property color
		\brief Color of the stepper

		By default color is QPalette::Highlight.
	*/
	Q_PROPERTY( QColor color READ color WRITE setColor )

signals:
	/*!
		This signal emits when value changes.
	*/
	void valueChanged( int i );

public:
	Stepper( QWidget * parent = 0 );
	virtual ~Stepper();

	//! \return Maximum value of the stepper.
	int maximum() const;
	//! Set maximum value of the stepper.
	void setMaximum( int m );

	//! \return Minimum value of the stepper.
	int minimum() const;
	//! Set minimum value of the stepper.
	void setMinimum( int m );

	/*!
		Convenience function to set the minimum, and maximum values with
		a single function call.
	*/
	void setRange( int min, int max );

	//! Step value by step.
	void stepBy( int step );

	//! \return Single step.
	int singleStep() const;
	//! Set single step.
	void setSingleStep( int step );

	//! \return Current value.
	int value() const;

	//! \return Is wrapping enabled?
	bool wrapping() const;
	//! Enable/disable wrapping mode.
	void setWrapping( bool enabled );

	//! \return Is autorepeat enabled?
	bool autorepeat() const;
	//! Enable/disable autorepeat mode.
	void setAutorepeat( bool enabled );

	//! \return Color.
	const QColor & color() const;
	//! Set color.
	void setColor( const QColor & c );

	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;

public slots:
	//! Set value.
	void setValue( int val );
	//! Decrement value.
	void stepDown();
	//! Increment value.
	void stepUp();

protected:
	void paintEvent( QPaintEvent * ) override;
	void mousePressEvent( QMouseEvent * event ) override;
	void mouseReleaseEvent( QMouseEvent * event ) override;

private slots:
	void _q_autorepeat();

private:
	friend class StepperPrivate;

	Q_DISABLE_COPY( Stepper )

	QScopedPointer< StepperPrivate > d;
}; // class Stepper

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__STEPPER_HPP__INCLUDED
