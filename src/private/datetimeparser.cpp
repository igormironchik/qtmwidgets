
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
	,	sectionWidth( 0 )
	,	currentIndex( -1 )
	,	offset( 0 )
{
}

Section::Section( Type t )
	:	type( t )
	,	zeroesAdded( false )
	,	sectionWidth( 0 )
	,	currentIndex( -1 )
	,	offset( 0 )
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
			QLocale::system().dayName( i, QLocale::ShortFormat ) + QLatin1Char( ' ' ) ).width();

		if( tmpWidth > width  )
		{
			index = i;
			width = tmpWidth;
		}
	}

	return QLocale::system().dayName( index, QLocale::ShortFormat );
}

static inline QString
maxLongDay( const QStyleOption & opt )
{
	int index = 1;
	int width = 0;

	for( int i = 1; i < 8; ++i )
	{
		const int tmpWidth = opt.fontMetrics.boundingRect(
			QLocale::system().dayName( i ) + QLatin1Char( ' ' ) ).width();

		if( tmpWidth > width  )
		{
			index = i;
			width = tmpWidth;
		}
	}

	return QLocale::system().dayName( index );
}

static inline QString
maxShortMonth( const QStyleOption & opt )
{
	int index = 1;
	int width = 0;

	for( int i = 1; i < 13; ++i )
	{
		const int tmpWidth = opt.fontMetrics.boundingRect(
			QLocale::system().monthName( i, QLocale::ShortFormat ) + QLatin1Char( ' ' ) ).width();

		if( tmpWidth > width  )
		{
			index = i;
			width = tmpWidth;
		}
	}

	return QLocale::system().monthName( index, QLocale::ShortFormat );
}

static inline QString
maxLongMonth( const QStyleOption & opt )
{
	int index = 1;
	int width = 0;

	for( int i = 1; i < 13; ++i )
	{
		const int tmpWidth = opt.fontMetrics.boundingRect(
			QLocale::system().monthName( i ) + QLatin1Char( ' ' ) ).width();

		if( tmpWidth > width  )
		{
			index = i;
			width = tmpWidth;
		}
	}

	return QLocale::system().monthName( index );
}

int
Section::maxWidth( const QStyleOption & opt ) const
{
	int width = opt.fontMetrics.boundingRect( value(
		DATETIMEPICKER_DATETIME_MAX ) ).width();

	width += opt.fontMetrics.averageCharWidth() / 3;

	switch( type )
	{
		case DaySectionShort :
		{
			width += opt.fontMetrics.boundingRect( maxShortDay( opt ) ).width();
			width += opt.fontMetrics.averageCharWidth();
		}
		break;

		case DaySectionLong :
		{
			width += opt.fontMetrics.boundingRect( maxLongDay( opt ) ).width();
			width += opt.fontMetrics.averageCharWidth();
		}
		break;

		case MonthSectionShort :
			width += opt.fontMetrics.boundingRect( maxShortMonth( opt ) ).width();
		break;

		case MonthSectionLong :
			width += opt.fontMetrics.boundingRect( maxLongMonth( opt ) ).width();
		break;

		default:
			break;
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
		break;

		case SecondSection :
		{
			makeSectionValue( v, dt.time().second(), zeroesAdded );
			return v;
		}
		break;

		case MinuteSection :
		{
			makeSectionValue( v, dt.time().minute(), zeroesAdded );
			return v;
		}
		break;

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
		break;

		case Hour24Section :
		{
			makeSectionValue( v, dt.time().hour(), zeroesAdded );
			return v;
		}
		break;

		case DaySection :
		case DaySectionShort :
		case DaySectionLong :
		{
			makeSectionValue( v, dt.date().day(), zeroesAdded );
			return v;
		}
		break;

		case MonthSection :
		{
			makeSectionValue( v, dt.date().month(), zeroesAdded );
			return v;
		}
		break;

		case MonthSectionShort :
		case MonthSectionLong :
			return v;
		break;

		case YearSection :
		{
			makeSectionValue( v, dt.date().year(), zeroesAdded );
			return v;
		}
		break;

		case YearSection2Digits :
		{
			makeSectionValue( v, dt.date().year(), zeroesAdded );
			return v.right( 2 );
		}
		break;

		default :
			return v;
		break;
	}

	return v;
}

void
Section::fillValues( const QDateTime & current,
	const QDateTime & min, const QDateTime & max,
	bool updateIndex )
{
	values.clear();

	if( updateIndex )
		currentIndex = -1;

	switch( type )
	{
		case AmPmSection :
		{
			values.append( QLatin1String( "AM" ) );
			values.append( QLatin1String( "PM" ) );

			if( current.time().hour() >= 12 )
				currentIndex = 1;
			else
				currentIndex = 0;
		}
		break;

		case SecondSection :
		{
			const int s = current.time().second();

			for( int i = 0; i < 60; ++i )
			{
				QString v;

				makeSectionValue( v, i, zeroesAdded );

				if( s == i )
					currentIndex = values.size();

				values.append( v );
			}
		}
		break;

		case MinuteSection :
		{
			const int m = current.time().minute();

			for( int i = 0; i < 60; ++i )
			{
				QString v;

				makeSectionValue( v, i, zeroesAdded );

				if( m == i )
					currentIndex = values.size();

				values.append( v );
			}
		}
		break;

		case Hour12Section :
		{
			int h = 0;
			int currentHour = current.time().hour();

			if( currentHour == 0 )
				h = 12;
			else if( currentHour > 12 )
				h = currentHour - 12;
			else
				h = currentHour;

			for( int i = 1; i < 13; ++i )
			{
				QString v;

				makeSectionValue( v, i, zeroesAdded );

				if( h == i )
					currentIndex = values.size();

				values.append( v );
			}
		}
		break;

		case Hour24Section :
		{
			const int h = current.time().hour();

			for( int i = 0; i < 24; ++i )
			{
				QString v;

				makeSectionValue( v, i, zeroesAdded );

				if( h == i )
					currentIndex = values.size();

				values.append( v );
			}
		}
		break;

		case DaySection :
		{
			const int d = current.date().day();

			for( int i = 1; i <= current.date().daysInMonth(); ++i )
			{
				QString v;

				makeSectionValue( v, i, zeroesAdded );

				if( updateIndex && d == i )
					currentIndex = values.size();

				values.append( v );
			}
		}
		break;

		case DaySectionShort :
		{
			const int d = current.date().day();

			QDate date( current.date().year(), current.date().month(), 1 );

			for( int i = 1; i <= current.date().daysInMonth(); ++i )
			{
				QString v;

				makeSectionValue( v, i, zeroesAdded );

				if( updateIndex && d == i )
					currentIndex = values.size();

				v.prepend( QLatin1Char( ' ' ) );

				v.prepend( QLocale::system().dayName( date.dayOfWeek(), QLocale::ShortFormat ) );

				values.append( v );

				date = date.addDays( 1 );
			}
		}
		break;

		case DaySectionLong :
		{
			const int d = current.date().day();

			QDate date( current.date().year(), current.date().month(), 1 );

			for( int i = 1; i <= current.date().daysInMonth(); ++i )
			{
				QString v;

				makeSectionValue( v, i, zeroesAdded );

				if( updateIndex && d == i )
					currentIndex = values.size();

				v.prepend( QLatin1Char( ' ' ) );

				v.prepend( QLocale::system().dayName( date.dayOfWeek() ) );

				values.append( v );

				date = date.addDays( 1 );
			}
		}
		break;

		case MonthSection :
		{
			const int m = current.date().month();

			for( int i = 1; i < 13; ++i )
			{
				QString v;

				makeSectionValue( v, i, zeroesAdded );

				if( m == i )
					currentIndex = values.size();

				values.append( v );
			}
		}
		break;

		case MonthSectionShort :
		{
			const int m = current.date().month();

			for( int i = 1; i < 13; ++i )
			{
				if( m == i )
					currentIndex = values.size();

				values.append( QLocale::system().monthName( i, QLocale::ShortFormat ) );
			}
		}
		break;

		case MonthSectionLong :
		{
			const int m = current.date().month();

			for( int i = 1; i < 13; ++i )
			{
				if( m == i )
					currentIndex = values.size();

				values.append( QLocale::system().monthName( i ) );
			}
		}
		break;

		case YearSection :
		{
			int start = min.date().year();
			const int finish = max.date().year();
			const int y = current.date().year();

			while( start <= finish )
			{
				QString v;

				makeSectionValue( v, start, zeroesAdded );

				if( start == y )
					currentIndex = values.size();

				values.append( v );

				++start;
			}
		}
		break;

		case YearSection2Digits :
		{
			int start = min.date().year();
			const int finish = max.date().year();
			const int y = current.date().year();

			while( start <= finish )
			{
				QString v;

				makeSectionValue( v, start, zeroesAdded );

				if( start == y )
					currentIndex = values.size();

				values.append( v.right( 2 ) );

				++start;
			}
		}
		break;

		default:
			break;
	}
}


//
// DateTimeParser
//

DateTimeParser::DateTimeParser( QMetaType::Type t )
	:	type( t )
{
	parseFormat( QLatin1String( "dddd MMMM yyyy hh mm" ) );
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

	if( fmt == format )
		return true;

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
				if( type != QMetaType::QDate )
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
					{
						qWarning( "DateTimeParser: redefinition of the hours section." );
						return false;
					}
				}
			} break;

			case 'm' :
			{
				if( type != QMetaType::QDate )
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
					{
						qWarning( "DateTimeParser: redefinition of the minutes section" );
						return false;
					}
				}
			} break;

			case 's' :
			{
				if( type != QMetaType::QDate )
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
					{
						qWarning( "DateTimeParser: redefinition of the seconds section." );
						return false;
					}
				}
			} break;

			case 'a' :
			{
				if( type != QMetaType::QDate )
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
					{
						qWarning( "DateTimeParser: redefinition of the AM/PM section." );
						return false;
					}
				}
			} break;

			case 'y' :
			{
				if( type != QMetaType::QTime )
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
						{
							qWarning( "DateTimeParser: redefinition of the years section." );
							return false;
						}
					}
					else
					{
						qWarning( "DateTimeParser: wrong value of the years section." );
						return false;
					}
				}
			} break;

			case 'M' :
			{
				if( type != QMetaType::QTime )
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
					{
						qWarning( "DateTimeParser: redefinition of the monthes section." );
						return false;
					}
				}
			} break;

			case 'd' :
			{
				if( type != QMetaType::QTime )
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
					{
						qWarning( "DateTimeParser: redefinition of the days section." );
						return false;
					}
				}
			} break;

			case ' ' :
				break;

			default :
			{
				qWarning( "DateTimeParser: prohibited character in the format string." );
				return false;
			}
		}
	}

	sections.swap( newSections );
	format = fmt;

	return true;
}

} /* namespace QtMWidgets */
