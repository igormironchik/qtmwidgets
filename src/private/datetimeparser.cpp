
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
#include "datetimeparser.hpp"

// Qt include.
#include <QStyleOption>


namespace QtMWidgets {

//
// Section
//

Section::Section()
	:	type( NoSection )
	,	zeroesAdded( false )
{
}

Section::Section( Type t )
	:	type( t )
	,	zeroesAdded( false )
{
}

static inline QString
maxShortDay( const QStyleOption & opt )
{
	int index = 1;
	int width = 0;

	for( int i = 1; i < 8; ++i )
	{
		const int tmpWidth = opt.fontMetrics.boundingRect(
			QDate::shortDayName( i ) + QLatin1Char( ' ' ) ).width();

		if( tmpWidth > width  )
		{
			index = i;
			width = tmpWidth;
		}
	}

	return QDate::shortDayName( index );
}

static inline QString
maxLongDay( const QStyleOption & opt )
{
	int index = 1;
	int width = 0;

	for( int i = 1; i < 8; ++i )
	{
		const int tmpWidth = opt.fontMetrics.boundingRect(
			QDate::longDayName( i ) + QLatin1Char( ' ' ) ).width();

		if( tmpWidth > width  )
		{
			index = i;
			width = tmpWidth;
		}
	}

	return QDate::longDayName( index );
}

static inline QString
maxShortMonth( const QStyleOption & opt )
{
	int index = 1;
	int width = 0;

	for( int i = 1; i < 13; ++i )
	{
		const int tmpWidth = opt.fontMetrics.boundingRect(
			QDate::shortMonthName( i ) + QLatin1Char( ' ' ) ).width();

		if( tmpWidth > width  )
		{
			index = i;
			width = tmpWidth;
		}
	}

	return QDate::shortMonthName( index );
}

static inline QString
maxLongMonth( const QStyleOption & opt )
{
	int index = 1;
	int width = 0;

	for( int i = 1; i < 13; ++i )
	{
		const int tmpWidth = opt.fontMetrics.boundingRect(
			QDate::longMonthName( i ) + QLatin1Char( ' ' ) ).width();

		if( tmpWidth > width  )
		{
			index = i;
			width = tmpWidth;
		}
	}

	return QDate::longMonthName( index );
}

int
Section::maxWidth( const QStyleOption & opt ) const
{
	int width = opt.fontMetrics.boundingRect( value(
		QDATETIMEPICKER_DATETIME_MAX ) ).width();

	switch( type )
	{
		case DaySectionShort :
			width += opt.fontMetrics.boundingRect( maxShortDay( opt ) +
				QLatin1Char( ' ' ) ).width();
		case DaySectionLong :
			width += opt.fontMetrics.boundingRect( maxLongDay( opt ) +
				QLatin1Char( ' ' ) ).width();
		case MonthSectionShort :
			width += opt.fontMetrics.boundingRect( maxShortMonth( opt ) +
				QLatin1Char( ' ' ) ).width();
		case MonthSectionLong :
			width += opt.fontMetrics.boundingRect( maxLongMonth( opt ) +
				QLatin1Char( ' ' ) ).width();
	}

	return width;
}

static inline void
makeSectionValue( QString & v, int section, bool zeroesAdded )
{
	if( zeroesAdded )
	{
		if( section >= 0 && section < 10 )
			v.append( QLatin1String( "0" ) );
	}

	v.append( QString::number( section ) );
}

QString
Section::value( const QDateTime & dt ) const
{
	QString v;

	switch( type )
	{
		case AmPmSection :
			if( ( dt.time().hour() > 12 && dt.time().hour() <= 23 ) ||
				dt.time().hour() == 0 )
					return QLatin1String( "PM" );
			else
				return QLatin1String( "AM" );
		case SecondSection :
		{
			makeSectionValue( v, dt.time().second(), zeroesAdded );
			return v;
		}
		case MinuteSection :
		{
			makeSectionValue( v, dt.time().minute(), zeroesAdded );
			return v;
		}
		case Hour12Section :
		{
			int hour = dt.time().hour();

			if( hour > 12 )
				hour -= 12;
			else if( hour == 0 )
				hour = 12;

			makeSectionValue( v, hour, zeroesAdded );

			return v;
		}
		case Hour24Section :
		{
			makeSectionValue( v, dt.time().hour(), zeroesAdded );
			return v;
		}
		case DaySection :
		{
			makeSectionValue( v, dt.date().day(), zeroesAdded );
			return v;
		}
		case DaySectionShort :
		case DaySectionLong :
		{
			makeSectionValue( v, dt.date().day(), zeroesAdded );
			return v;
		}
		case MonthSection :
		{
			makeSectionValue( v, dt.date().month(), zeroesAdded );
			return v;
		}
		case MonthSectionShort :
		case MonthSectionLong :
			return v;
		case YearSection :
		{
			makeSectionValue( v, dt.date().year(), zeroesAdded );
			return v;
		}
		case YearSection2Digits :
		{
			makeSectionValue( v, dt.date().year(), zeroesAdded );
			return v.right( 2 );
		}
		default :
			return v;
	}
}


//
// DateTimeParser
//

DateTimeParser::DateTimeParser( QVariant::Type t )
	:	type( t )
{
	parseFormat( QLatin1String( "yyyy MMMM dddd hh mm" ) );
}

DateTimeParser::~DateTimeParser()
{
}

static inline int
countRepeat( const QString &str, int index, int maxCount )
{
	int count = 1;

	const QChar ch( str.at( index ) );
	const int max = qMin( index + maxCount, str.size() );

	while( index + count < max && str.at( index + count ) == ch )
		++count;

	return count;
}

bool
DateTimeParser::parseFormat( const QString & fmt )
{
	if( fmt.isEmpty() )
		return false;

	const int max = fmt.size();

	int amPmSectIndex = -1;
	int secondSectIndex = -1;
	int minuteSectIndex = -1;
	int hourSectIndex = -1;
	int daySectIndex = -1;
	int monthSectIndex = -1;
	int yearSectIndex = -1;

	QVector< Section > newSections;

	for( int i = 0; i < max; ++i )
	{
		switch( fmt.at( i ).toLatin1() )
		{
			case 'h' :
			{
				if( type != QVariant::Date )
				{
					const int count = countRepeat( fmt, i, 2 );

					Section::Type t = ( amPmSectIndex != -1 ) ?
						Section::Hour12Section : Section::Hour24Section;

					Section s( t );
					s.zeroesAdded = ( count == 2 ) ? true : false;

					i += ( count - 1 );

					if( hourSectIndex == -1 )
					{
						hourSectIndex = newSections.size();
						newSections.append( s );
					}
					else
						return false;
				}
			} break;

			case 'm' :
			{
				if( type != QVariant::Date )
				{
					const int count = countRepeat( fmt, i, 2 );

					Section s( Section::MinuteSection );
					s.zeroesAdded = ( count == 2 ) ? true : false;

					i += ( count - 1 );

					if( minuteSectIndex == -1 )
					{
						minuteSectIndex = newSections.size();
						newSections.append( s );
					}
					else
						return false;
				}
			} break;

			case 's' :
			{
				if( type != QVariant::Date )
				{
					const int count = countRepeat( fmt, i, 2 );

					Section s( Section::SecondSection );
					s.zeroesAdded = ( count == 2 ) ? true : false;

					i += ( count - 1 );

					if( secondSectIndex == -1 )
					{
						secondSectIndex = newSections.size();
						newSections.append( s );
					}
					else
						return false;
				}
			} break;

			case 'a' :
			{
				if( type != QVariant::Date )
				{
					Section s( Section::AmPmSection );

					if( hourSectIndex != -1 )
						newSections[ hourSectIndex ].type = Section::Hour12Section;

					if( amPmSectIndex == -1 )
					{
						amPmSectIndex = newSections.size();
						newSections.append( s );
					}
					else
						return false;
				}
			} break;

			case 'y' :
			{
				if( type != QVariant::Time )
				{
					const int count = countRepeat( fmt, i, 4 );

					if( count >= 2 && count != 3 )
					{
						Section::Type t = ( count == 2 ?
							Section::YearSection2Digits : Section::YearSection );
						Section s( t );

						i += ( count - 1 );

						if( yearSectIndex == -1 )
						{
							yearSectIndex = newSections.size();
							newSections.append( s );
						}
						else
							return false;
					}
					else
						return false;
				}
			} break;

			case 'M' :
			{
				if( type != QVariant::Time )
				{
					const int count = countRepeat( fmt, i, 4 );

					Section::Type t = Section::MonthSection;

					if( count == 3 )
						t = Section::MonthSectionShort;
					else if( count == 4 )
						t = Section::MonthSectionLong;

					Section s( t );
					s.zeroesAdded = ( count == 2 ) ? true : false;

					i += ( count - 1 );

					if( monthSectIndex == -1 )
					{
						monthSectIndex = newSections.size();
						newSections.append( s );
					}
					else
						return false;
				}
			} break;

			case 'd' :
			{
				if( type != QVariant::Time )
				{
					const int count = countRepeat( fmt, i, 4 );

					Section::Type t = Section::DaySection;

					if( count == 3 )
						t = Section::DaySectionShort;
					else if( count == 4 )
						t = Section::DaySectionLong;

					Section s( t );
					s.zeroesAdded = ( count == 2 ) ? true : false;

					i += ( count - 1 );

					if( daySectIndex == -1 )
					{
						daySectIndex = newSections.size();
						newSections.append( s );
					}
					else
						return false;
				}
			} break;
		}
	}

	sections.swap( newSections );

	return true;
}

} /* namespace QtMWidgets */
