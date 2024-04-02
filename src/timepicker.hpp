
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
