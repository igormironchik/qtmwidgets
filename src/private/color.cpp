
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
#include "color.hpp"


namespace QtMWidgets {

//
// lighterColor
//

QColor
lighterColor( const QColor & c, int b )
{
	if( b <= 0 )
		return c;

	int h = 0;
	int s = 0;
	int v = 0;
	int a = 0;

	QColor hsv = c.toHsv();
	hsv.getHsv( &h, &s, &v, &a );

	v += b;

	if( v > 255 )
	{
		s -= v - 255;

		if( s < 0 ) s = 0;

		v = 255;
	}

	hsv.setHsv( h, s, v, a );

	return hsv.convertTo( c.spec() );
}

} /* namespace QtMWidgets */
