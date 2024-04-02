
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef QTMWIDGETS_UTILS_HPP_INCLUDED
#define QTMWIDGETS_UTILS_HPP_INCLUDED

// Qt include.
#include <QString>
#include <QStyleOption>
#include <QRect>


namespace QtMWidgets {

QString
accomodateString( const QString & text, const QRect & r,
	int flags, const QStyleOption & opt );

} /* namespace QtMWidgets */

#endif // QTMWIDGETS_UTILS_HPP_INCLUDED
