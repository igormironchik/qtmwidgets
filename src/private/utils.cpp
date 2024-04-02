
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
