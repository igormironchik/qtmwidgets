
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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

	virtual ~DatePicker();
}; // class DatePicker

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__DATEPICKER_HPP__INCLUDED
