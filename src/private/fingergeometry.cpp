
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

} /* namespace QtMWidgets */
