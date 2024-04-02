
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef QTMWIDGETS__PRIVATE__SCROLLER_HPP__INCLUDED
#define QTMWIDGETS__PRIVATE__SCROLLER_HPP__INCLUDED

// Qt include.
#include <QObject>
#include <QScopedPointer>
#include <QEasingCurve>


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
	/*!
		\property maxReachedVelocity

		This is the maximum velocity that can be reached in pixels per second.

		Default value is depend on the platform, but actually is
		equal to 2 fingers per second ( 2 sm/s ).
	*/
	Q_PROPERTY( uint maxReachedVelocity READ maxReachedVelocity
		WRITE setMaxReachedVelocity )
	/*!
		\property dragStartDistance

		This is the minimum distance the touch or mouse point needs
		to be moved before the flick gesture is triggered in pixels.

		Default value is depend on the platform, but actually is
		equal to 1 finger ( 1 sm ).
	*/
	Q_PROPERTY( uint dragStartDistance READ dragStartDistance
		WRITE setDragStartDistance )
	/*!
		\property scrollTime

		This is the time that scroller will work after activating
		in milliseconds.

		Default value is 3000 ms.
	*/
	Q_PROPERTY( uint scrollTime READ scrollTime WRITE setScrollTime )
	/*!
		\property scrollingCurve

		The QEasingCurve used when decelerating the scrolling velocity
		after an user initiated flick. Please note that this is the easing
		curve for the positions, not the velocity.

		Default value is QEasingCurve::OutCirc.
	*/
	Q_PROPERTY( QEasingCurve scrollingCurve READ scrollingCurve
		WRITE setScrollingCurve )

signals:
	/*!
		Emitted when target should be scrolled.
	*/
	void scroll( int dx, int dy );
	/*!
		Emitted when kinetic scrolling is about to start.
	*/
	void aboutToStart();
	/*!
		Emitted when kinetic scrolling finished.
	*/
	void finished();

public:
	explicit Scroller( QObject * target, QObject * parent = 0 );
	virtual ~Scroller();

	//! \return Minimum recognized velocity.
	uint minRecognizedVelocity() const;
	//! Set minimum recognized velocity.
	void setMinRecognizedVelocity( uint v );

	//! \return Maximum reached velocity.
	uint maxReachedVelocity() const;
	//! Set maximum reached velocity.
	void setMaxReachedVelocity( uint v );

	//! \return Drag start distance.
	uint dragStartDistance() const;
	//! Set drag start distance.
	void setDragStartDistance( uint v );

	//! \return Scroll time.
	uint scrollTime() const;
	//! Set scroll time.
	void setScrollTime( uint v );

	//! \return Scrolling curve.
	const QEasingCurve & scrollingCurve() const;
	//! Set scrolling curve.
	void setScrollingCurve( const QEasingCurve & c );

protected:
	bool eventFilter( QObject * obj, QEvent * event ) override;

private slots:
	void _q_animation( const QVariant & v );
	void _q_animationFinished();

private:
	Q_DISABLE_COPY( Scroller )

	QScopedPointer< ScrollerPrivate > d;
}; // class Scroller

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PRIVATE__SCROLLER_HPP__INCLUDED
