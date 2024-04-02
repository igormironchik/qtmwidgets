
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// QtMWidgets include.
#include "datepicker.hpp"
#include "private/datetimeparser.hpp"


namespace QtMWidgets {

//
// DatePicker
//

DatePicker::DatePicker( QWidget * parent )
	:	DateTimePicker( DATETIMEPICKER_DATE_INITIAL, QMetaType::QDate, parent )
{
}

DatePicker::DatePicker( const QDate & date, QWidget * parent )
	:	DateTimePicker( date, QMetaType::QDate, parent )
{
}

DatePicker::~DatePicker()
{
}

} /* namespace QtMWidgets */
