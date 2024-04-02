
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef QTMWIDGETS__PRIVATE__LAYOUTENGINE_HPP__INCLUDED
#define QTMWIDGETS__PRIVATE__LAYOUTENGINE_HPP__INCLUDED

// Qt include.
#include <QWidget>


namespace QtMWidgets {

static const int QLAYOUTSIZE_MAX = INT_MAX/256/16;

//
// qSmartMinSize
//

QSize qSmartMinSize( const QSize & sizeHint, const QSize & minSizeHint,
	const QSize & minSize, const QSize & maxSize,
	const QSizePolicy & sizePolicy );

//
// qSmartMinSize
//

QSize qSmartMinSize( const QWidget * w );

//
// qSmartMaxSize
//

QSize qSmartMaxSize( const QSize & sizeHint,
	const QSize & minSize, const QSize & maxSize,
	const QSizePolicy & sizePolicy, Qt::Alignment align = Qt::Alignment() );

//
// qSmartMaxSize
//

QSize qSmartMaxSize( const QWidget * w, Qt::Alignment align = Qt::Alignment() );

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PRIVATE__LAYOUTENGINE_HPP__INCLUDED
