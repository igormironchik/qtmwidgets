
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// QtMWidgets include.
#include "fingergeometry.hpp"

// Qt include.
#include <QApplication>
#include <QScreen>
#include <QByteArray>


namespace QtMWidgets {

static const qreal fingerSize = 0.0393700787 * 10;


//
// FingerGeometry
//

int
FingerGeometry::width()
{
#ifdef Q_OS_ANDROID

	static const int w =
		qRound( qgetenv( "QT_ANDROID_THEME_DISPLAY_DPI" ).toDouble() *
			fingerSize );

	return w;

#else

	static const int w =
		qRound(
			(qreal) qMax( QApplication::primaryScreen()->logicalDotsPerInchX(),
				QApplication::primaryScreen()->physicalDotsPerInchX() ) *
			fingerSize );

	return w;

#endif
}

int
FingerGeometry::height()
{
#ifdef Q_OS_ANDROID

	static const int h =
		qRound( qgetenv( "QT_ANDROID_THEME_DISPLAY_DPI" ).toDouble() *
			fingerSize );

	return h;

#else

	static const int h =
		qRound(
			(qreal) qMax( QApplication::primaryScreen()->logicalDotsPerInchY(),
				QApplication::primaryScreen()->physicalDotsPerInchY() ) *
			fingerSize );

	return h;

#endif
}

int
FingerGeometry::touchBounce()
{
	static const int bounce = qRound(
		(qreal) qMax( FingerGeometry::height(), FingerGeometry::width() ) * 0.2 );

	return bounce;
}

int
FingerGeometry::longTouchBounce()
{
	static const int bounce = qRound(
		(qreal) qMax( FingerGeometry::height(), FingerGeometry::width() ) * 0.6 );

	return bounce;
}

} /* namespace QtMWidgets */
