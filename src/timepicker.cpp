
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// QtMWidgets include.
#include "timepicker.hpp"
#include "private/datetimeparser.hpp"


namespace QtMWidgets {

//
// TimePicker
//

TimePicker::TimePicker( QWidget * parent )
	:	DateTimePicker( DATETIMEPICKER_TIME_MIN, QMetaType::QTime, parent )
{
}

TimePicker::TimePicker( const QTime & time, QWidget * parent )
	:	DateTimePicker( time, QMetaType::QTime, parent )
{
}

TimePicker::~TimePicker()
{
}

} /* namespace QtMWidgets */
