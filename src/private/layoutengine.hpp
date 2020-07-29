
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
