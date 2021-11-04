
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
#include "utils.hpp"


namespace QtMWidgets {

QString
accomodateString( const QString & text, const QRect & r,
	int flags, const QStyleOption & opt )
{
	const QRect & b = opt.fontMetrics.boundingRect( r, flags, text );

	QString res = text;

	if( b.width() > r.width() )
	{
		int w = 0;
		int x = 0;

		res.clear();

		while( w <= ( r.width() ) / 2 )
		{
			res.append( text.at( x ) );
			++x;
			w = opt.fontMetrics.boundingRect( r, flags, res ).width();
		}

		res.append( QStringLiteral( "..." ) );

		x = text.length() - 1;

		QString tmp = text.at( x );

		while( opt.fontMetrics.boundingRect( r, flags, res + tmp ).width() <= r.width() )
		{
			--x;
			tmp.prepend( text.at( x ) );
		}

		res.append( text.right( text.length() - x - 1 ) );
	}

	return res;
}

} /* namespace QtMWidgets */
