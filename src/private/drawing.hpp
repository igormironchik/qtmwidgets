
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef QTMWIDGETS__DRAWING_HPP__INCLUDED
#define QTMWIDGETS__DRAWING_HPP__INCLUDED

// Qt include.
#include <QtGlobal>

QT_BEGIN_NAMESPACE
class QPainter;
class QRect;
class QColor;
QT_END_NAMESPACE


namespace QtMWidgets {

//
// drawCylinder
//

//! Draw cylinder with rect \a r.
void drawCylinder( QPainter * p, const QRect & r, const QColor & baseColor,
	bool roundLeftCorner = true, bool roundRightCorner = true );


//
// drawSliderHandle
//

//! Draw slider's handle.
void drawSliderHandle( QPainter * p, const QRect & r,
	int xRadius, int yRadius, const QColor & borderColor,
	const QColor & lightColor );


//
// drawArrow
//

//! Draw horizontal arrow looks to the right.
void drawArrow( QPainter * p, const QRect & r,
	const QColor & color );


//
// drawArrow2
//

//! Draw vertical arrow look to the bottom.
void drawArrow2( QPainter * p, const QRect & r,
	const QColor & color );

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__DRAWING_HPP__INCLUDED
