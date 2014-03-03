
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

public:
	BusyIndicator( QWidget * parent = 0 );
	virtual ~BusyIndicator();

	//! \return Is busy indicator running?
	bool isRunning() const;
	//! Set busy indicator running property.
	void setRunning( bool on );

	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;

protected:
	virtual void paintEvent( QPaintEvent * );

private slots:
	void _q_update( const QVariant & );

private:
	Q_DISABLE_COPY( BusyIndicator )

	QScopedPointer< BusyIndicatorPrivate > d;
}; // class BusyIndicator

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__BUSYINDICATOR_HPP__INCLUDED
