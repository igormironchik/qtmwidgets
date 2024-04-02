
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef QTMWIDGETS__BUSYINDICATOR_HPP__INCLUDED
#define QTMWIDGETS__BUSYINDICATOR_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>


namespace QtMWidgets {

//
// BusyIndicator
//

class BusyIndicatorPrivate;

/*!
	BusyIndicator is a widget that shows activity of
	the application, more precisely busy of the application.
	This widget is a ring that spinning.
*/
class BusyIndicator
	:	public QWidget
{
	Q_OBJECT

	/*!
		\property running

		This property holds whether the busy indicator is currently
		indicating activity.

		\note The indicator is only visible when this property
		is set to true.

		The default value is true.
	*/
	Q_PROPERTY( bool running READ isRunning WRITE setRunning )
	/*!
		\property color

		\brief color used to paint indicator

		By default this color is QPalette::Highlight.
	*/
	Q_PROPERTY( QColor color READ color WRITE setColor )
	/*!
		\property radius

		\brief radius of the busy indicator.

		By default, this property is 10.
	*/
	Q_PROPERTY( int radius READ radius WRITE setRadius )

public:
	BusyIndicator( QWidget * parent = 0 );
	virtual ~BusyIndicator();

	//! \return Is busy indicator running?
	bool isRunning() const;
	//! Set busy indicator running property.
	void setRunning( bool on );

	//! \return Color used to paint indicator.
	const QColor & color() const;
	//! Set color used to paint indicator.
	void setColor( const QColor & c );

	//! \return Radius.
	int radius() const;
	//! Set radius.
	void setRadius( int r );

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

protected:
	void paintEvent( QPaintEvent * ) override;

private slots:
	void _q_update( const QVariant & );

private:
	friend class BusyIndicatorPrivate;

	Q_DISABLE_COPY( BusyIndicator )

	QScopedPointer< BusyIndicatorPrivate > d;
}; // class BusyIndicator

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__BUSYINDICATOR_HPP__INCLUDED
