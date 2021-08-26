
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

#ifndef QTMWIDGETS__PROGRESSBAR_HPP__INCLUDED
#define QTMWIDGETS__PROGRESSBAR_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>


namespace QtMWidgets {

//
// ProgressBar
//

class ProgressBarPrivate;

/*!
	\brief The ProgressBar widget provides a horizontal or vertical progress bar.

	A progress bar is used to give the user an indication of the
	progress of an operation and to reassure them that the application
	is still running.

	The progress bar uses the concept of \e steps. You set it up by
	specifying the minimum and maximum possible step values, and it
	will display the percentage of steps that have been completed
	when you later give it the current step value. The percentage is
	calculated by dividing the progress (value() - minimum()) divided
	by maximum() - minimum().

	You can specify the minimum and maximum number of steps with
	setMinimum() and setMaximum(). The current number of steps is set
	with setValue(). The progress bar can be rewound to the
	beginning with reset().

	If minimum and maximum both are set to 0, the bar shows a busy
	indicator instead of a percentage of steps. This is useful, for
	example, when using QNetworkAccessManager to download items when
	they are unable to determine the size of the item being downloaded.
*/
class ProgressBar
	:	public QWidget
{
	Q_OBJECT

	/*!
		\property minimum
		\brief the progress bar's minimum value

		When setting this property, the \link maximum maximum \endlink is
		adjusted if necessary to ensure that the range remains valid. If the
		current value falls outside the new range, the progress bar is reset
		with reset().
	*/
	Q_PROPERTY( int minimum READ minimum WRITE setMinimum )
	/*!
		\property maximum
		\brief the progress bar's maximum value

		When setting this property, the \link minimum minimum \endlink is
		adjusted if necessary to ensure that the range remains valid. If the
		current value falls outside the new range, the progress bar is reset
		with reset().
	*/
	Q_PROPERTY( int maximum READ maximum WRITE setMaximum )
	/*!
		\property value
		\brief the progress bar's current value

		Attempting to change the current value to one outside
		the minimum-maximum range has no effect on the current value.
	*/
	Q_PROPERTY( int value READ value WRITE setValue NOTIFY valueChanged )
	/*!
		\property orientation
		\brief the orientation of the progress bar

		The orientation must be Qt::Horizontal (the default) or
		Qt::Vertical.

		\sa invertedAppearance
	*/
	Q_PROPERTY( Qt::Orientation orientation READ orientation
		WRITE setOrientation )
	/*!
		\property invertedAppearance
		\brief whether or not a progress bar shows its progress inverted

		If this property is \c true, the progress bar grows in the other
		direction (e.g. from right to left). By default, the progress bar
		is not inverted.

		\sa orientation
	*/
	Q_PROPERTY( bool invertedAppearance READ invertedAppearance
		WRITE setInvertedAppearance )
	/*!
		\property grooveHeight
		\brief Height for horizontal (width for vertical) of the groove.

		By default, this property is 3 px.
	*/
	Q_PROPERTY( int grooveHeight READ grooveHeight WRITE setGrooveHeight )
	/*!
		\property highlightColor
		\brief Color used for painting progress bar (filled with value)

		By default, this property is QPalette::Highlight.
	*/
	Q_PROPERTY( QColor highlightColor READ highlightColor
		WRITE setHighlightColor )
	/*!
		\property grooveColor
		\brief Color used for painting groove of the progress bar

		By default, this property is QPalette::Dark.
	*/
	Q_PROPERTY( QColor grooveColor READ grooveColor
		WRITE setGrooveColor )

signals:
	/*!
		This signal is emitted when the value shown in the progress bar changes.
		\a value is the new value shown by the progress bar.
	*/
	void valueChanged( int value );

public:
	ProgressBar( QWidget * parent = 0 );
	virtual ~ProgressBar();

	//! \return Minimum value.
	int minimum() const;
	//! \return Maximum value.
	int maximum() const;
	//! \return Current value.
	int value() const;
	//! \return Orientation.
	Qt::Orientation orientation() const;
	//! \return Is appearance inverted?
	bool invertedAppearance() const;
	//! Set inverted appearance.
	void setInvertedAppearance( bool invert );

	//! \return Groove height.
	int grooveHeight() const;
	//! Set groove height.
	void setGrooveHeight( int h );

	//! \return Color used for painting progress bar.
	const QColor & highlightColor() const;
	//! Set color used for painting progress bar.
	void setHighlightColor( const QColor & c );

	//! \return GGroove color.
	const QColor & grooveColor() const;
	//! Set groove color.
	void setGrooveColor( const QColor & c );

	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;

public slots:
	/*!
		Reset the progress bar. The progress bar "rewinds" and shows no
		progress.
	*/
	void reset();
	//! Set range.
	void setRange( int minimum, int maximum );
	//! Set minimum value.
	void setMinimum( int minimum );
	//! Set maximum value.
	void setMaximum( int maximum );
	//! Set current value.
	void setValue( int value );
	//! Set orientation.
	void setOrientation( Qt::Orientation orientation );

protected:
	void paintEvent( QPaintEvent * ) override;

private slots:
	void _q_animation( const QVariant & value );

private:
	Q_DISABLE_COPY( ProgressBar )

	QScopedPointer< ProgressBarPrivate > d;
}; // class ProgressBar

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PROGRESSBAR_HPP__INCLUDED
