
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

#ifndef QTMWIDGETS__TIMEPICKER_HPP__INCLUDED
#define QTMWIDGETS__TIMEPICKER_HPP__INCLUDED

// QtMWidgets include.
#include "datetimepicker.hpp"


namespace QtMWidgets {

//
// TimePicker
//

/*!
	The TimePicker class provides widget for selecting
	time. TimePicker shows cylinders with separate
	components of time, i.e. hour, minute.
	By default format of the TimePicker is "hh mm",
	that mean that widgets will show 2 cylinders with hours
	and minutes. \sa setFormat().

	TimePicker allows to select time from the given
	range. By default minimum time is 00:00:00
	and maximum is 23:59:59.
*/
class TimePicker
	:	public DateTimePicker
{
	Q_OBJECT
	Q_PROPERTY( QTime time READ time WRITE setTime USER true )

public:
	explicit TimePicker( QWidget * parent = 0 );
	explicit TimePicker( const QTime & time, QWidget * parent = 0 );

	virtual ~TimePicker();
}; // class TimePicker

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__TIMEPICKER_HPP__INCLUDED
