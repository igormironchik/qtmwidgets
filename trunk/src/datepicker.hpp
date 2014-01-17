
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

#ifndef QTMWIDGETS__DATEPICKER_HPP__INCLUDED
#define QTMWIDGETS__DATEPICKER_HPP__INCLUDED

// QtMWidgets include.
#include "datetimepicker.hpp"


namespace QtMWidgets {

//
// DatePicker
//

/*!
	The DatePicker class provides widget for selecting
	date. DatePicker shows cylinders with separate
	components of date, i.e. year, month, day.
	By default format of the DatePicker is "yyyy MMMM dddd",
	that mean that widgets will show 3 cylinders with year, long month name
	and long day name with day number. \sa setFormat().

	DatePicker allows to select date from the given
	range. By default minimum date is September 14, 1752
	and maximum is December 31, 7999.
*/
class DatePicker
	:	public DateTimePicker
{
	Q_OBJECT
	Q_PROPERTY( QDate date READ date WRITE setDate USER true )

public:
	explicit DatePicker( QWidget * parent = 0 );
	explicit DatePicker( const QDate & date, QWidget * parent = 0 );

	~DatePicker();
}; // class DatePicker

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__DATEPICKER_HPP__INCLUDED
