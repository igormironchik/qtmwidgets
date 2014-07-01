
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

#ifndef QTMWIDGETS__PRIVATE__SCROLLER_HPP__INCLUDED
#define QTMWIDGETS__PRIVATE__SCROLLER_HPP__INCLUDED

// Qt include.
#include <QObject>
#include <QScopedPointer>


namespace QtMWidgets {

//
// Scroller
//

class ScrollerPrivate;

/*!
	Scroller is an auxiliary tool for scrolling something
	on swipe gesture. It calculates velocity of the swipe gesture
	and send signal to scroll.
*/
class Scroller
	:	public QObject
{
	Q_OBJECT

	/*!
		\property minRecognizedVelocity

		This is minimum velocity that should be for scroller take an
		effect. Measured in pixels per second.

		Default value is depend on the platform, but actually is
		equal to 3 fingers per second ( 3 sm/s ).
	*/
	Q_PROPERTY( uint minRecognizedVelocity READ minRecognizedVelocity
		WRITE setMinRecognizedVelocity )

signals:
	/*!
		Emitted when target should be scrolled.
	*/
	void scroll( int dx, int dy );

public:
	explicit Scroller( QObject * target, QObject * parent = 0 );
	virtual ~Scroller();

	//! \return Minimum recognized velocity.
	uint minRecognizedVelocity() const;
	//! Set minimum recognized velocity.
	void setMinRecognizedVelocity( uint v );

protected:
	virtual bool eventFilter( QObject * obj, QEvent * event );

private slots:
	void _q_animation( const QVariant & v );

private:
	Q_DISABLE_COPY( Scroller )

	QScopedPointer< ScrollerPrivate > d;
}; // class Scroller

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PRIVATE__SCROLLER_HPP__INCLUDED
