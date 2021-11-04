
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
#include "datetimepicker.hpp"
#include "private/datetimeparser.hpp"
#include "private/drawing.hpp"
#include "color.hpp"
#include "scroller.hpp"

// Qt include.
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QBrush>
#include <QPen>
#include <QLinearGradient>


namespace QtMWidgets {

//
// DateTimePickerPrivate
//

class DateTimePickerPrivate
	:	public DateTimeParser
{
public:
	DateTimePickerPrivate( DateTimePicker * parent,
		QMetaType::Type parserType )
		:	DateTimeParser( parserType )
		,	q( parent )
		,	minimum( QDateTime( DATETIMEPICKER_COMPAT_DATE_MIN,
				DATETIMEPICKER_TIME_MIN ) )
		,	maximum( DATETIMEPICKER_DATETIME_MAX )
		,	value( QDateTime( DATETIMEPICKER_DATE_INITIAL,
				DATETIMEPICKER_TIME_MIN ) )
		,	spec( Qt::LocalTime )
		,	itemHeight( 0 )
		,	itemTopMargin( 0 )
		,	itemsMaxCount( 5 )
		,	itemSideMargin( 5 )
		,	widgetHeight( 0 )
		,	currentItemY( 0 )
		,	leftMouseButtonPressed( false )
		,	movableSection( -1 )
		,	daysSection( -1 )
		,	monthSection( -1 )
		,	yearSection( -1 )
		,	scroller( new Scroller( q, q ) )
		,	scrolling( false )
	{
		initDaysMonthYearSectionIndex();
		fillValues();
	}

	void updateTimeSpec();
	void setRange( const QDateTime & min, const QDateTime & max );
	void setValue( const QDateTime & dt, bool updateIndexes = true );
	void emitSignals();
	void normalizeOffset( int section );
	void normalizeOffsets();
	void drawSectionItems( int section, QPainter * p,
		const QStyleOption & opt );
	void drawWindow( QPainter * p, const QStyleOption & opt );
	void findMovableSection( const QPointF & pos );
	void updateOffset( int delta );
	void clearOffset();
	void updateDaysIfNeeded();
	void updateCurrentDateTime();
	void initDaysMonthYearSectionIndex();
	void fillValues( bool updateIndexes = true );
	void releaseScrolling();

	DateTimePicker * q;
	QDateTime minimum;
	QDateTime maximum;
	QDateTime value;
	Qt::TimeSpec spec;
	int itemHeight;
	int itemTopMargin;
	int itemsMaxCount; // Must be odd.
	int itemSideMargin;
	int widgetHeight;
	int currentItemY;
	QPoint mousePos;
	bool leftMouseButtonPressed;
	int movableSection;
	int daysSection;
	int monthSection;
	int yearSection;
	Scroller * scroller;
	bool scrolling;
}; // class DateTimePickerPrivate

void
DateTimePickerPrivate::updateTimeSpec()
{
	minimum = minimum.toTimeSpec( spec );
	maximum = maximum.toTimeSpec( spec );
	value = value.toTimeSpec( spec );
}

void
DateTimePickerPrivate::setRange( const QDateTime & min, const QDateTime & max )
{
	if( minimum != min || maximum != max )
	{
		minimum = min;
		maximum = max;

		if( value < minimum )
			value = minimum;
		else if( value > maximum )
			value = maximum;

		fillValues();

		q->updateGeometry();
		q->update();
	}
}

void
DateTimePickerPrivate::setValue( const QDateTime & dt, bool updateIndexes )
{
	if( value != dt )
	{
		if( dt >= minimum && dt <= maximum )
			value = dt;
		else if( dt < minimum )
			value = minimum;
		else if( dt > maximum )
			value = maximum;

		fillValues( updateIndexes );

		if( updateIndexes || value != dt )
		{
			for( int i = 0; i < sections.size(); ++i )
			{
				switch( sections.at( i ).type )
				{
					case Section::AmPmSection :
					{
						if( value.time().hour() == 0 ||
							value.time().hour() > 12 )
								sections[ i ].currentIndex = 1;
						else
							sections[ i ].currentIndex = 0;
					}
					break;

					case Section::SecondSection :
					{
						sections[ i ].currentIndex = value.time().second();
					}
					break;

					case Section::MinuteSection :
					{
						sections[ i ].currentIndex = value.time().minute();
					}
					break;

					case Section::Hour12Section :
					{
						if( value.time().hour() == 0 )
							sections[ i ].currentIndex = 11;
						else if( value.time().hour() > 12 )
							sections[ i ].currentIndex = value.time().hour() - 12 - 1;
						else
							sections[ i ].currentIndex = value.time().hour() - 1;
					}
					break;

					case Section::Hour24Section :
					{
						sections[ i ].currentIndex = value.time().hour();
					}
					break;

					case Section::DaySection :
					case Section::DaySectionShort :
					case Section::DaySectionLong :
					{
						sections[ i ].currentIndex = value.date().day() - 1;
					}
					break;

					case Section::MonthSection :
					case Section::MonthSectionShort :
					case Section::MonthSectionLong :
					{
						sections[ i ].currentIndex = value.date().month() - 1;
					}
					break;

					case Section::YearSection :
					case Section::YearSection2Digits :
					{
						sections[ i ].currentIndex = value.date().year() -
							minimum.date().year();
					}
					break;

					default:
						break;
				}
			}
		}

		emitSignals();
	}
}

void
DateTimePickerPrivate::emitSignals()
{
	emit q->dateTimeChanged( value );
	emit q->dateChanged( value.date() );
	emit q->timeChanged( value.time() );
}

static inline int prevIndex( int current, int size )
{
	if( current == 0 )
		return size - 1;
	else
		return current - 1;
}

static inline int nextIndex( int current, int size )
{
	if( current == size - 1 )
		return 0;
	else
		return current + 1;
}

void
DateTimePickerPrivate::normalizeOffset( int section )
{
	const int sectionValuesSize = sections.at( section ).values.size();
	const int totalItemHeight = itemHeight + itemTopMargin;

	while( qAbs( sections.at( section ).offset ) > totalItemHeight / 2 )
	{
		if( sections.at( section ).offset > 0 )
		{
			if( sectionValuesSize < itemsMaxCount &&
				sections[ section ].currentIndex == 0 )
			{
				sections[ section ].offset = 0;
				break;
			}

			sections[ section ].offset -= totalItemHeight;
			sections[ section ].currentIndex = prevIndex(
				sections.at( section ).currentIndex, sectionValuesSize );
		}
		else
		{
			if( sectionValuesSize < itemsMaxCount &&
				sections[ section ].currentIndex ==
					sections[ section ].values.size() - 1 )
			{
				sections[ section ].offset = 0;
				break;
			}

			sections[ section ].offset += totalItemHeight;
			sections[ section ].currentIndex = nextIndex(
				sections.at( section ).currentIndex, sectionValuesSize );
		}
	}
}

void
DateTimePickerPrivate::normalizeOffsets()
{
	for( int i = 0; i < sections.size(); ++i )
		normalizeOffset( i );
}

void
DateTimePickerPrivate::drawSectionItems( int section, QPainter * p,
	const QStyleOption & opt )
{
	int x = 0;

	for( int i = 0; i < section; ++i )
		x += sections.at( i ).sectionWidth;

	x += 3 + itemSideMargin;

	p->setPen( opt.palette.color( QPalette::WindowText ) );

	const int yOffset = sections.at( section ).offset;

	int makePrevIndexCount = itemsMaxCount / 2;

	if( yOffset > 0 )
		++makePrevIndexCount;

	if( sections.at( section ).values.size() < itemsMaxCount )
		makePrevIndexCount = sections.at( section ).currentIndex;

	int index = sections.at( section ).currentIndex;
	int y = currentItemY + yOffset;

	for( int i = 0; i < makePrevIndexCount; ++i )
	{
		index = prevIndex( index, sections.at( section ).values.size() );
		y -= ( itemHeight + itemTopMargin );
	}

	int iterationsCount = ( yOffset == 0 ) ? itemsMaxCount : itemsMaxCount + 1;

	if( sections.at( section ).values.size() < itemsMaxCount )
		iterationsCount = sections.at( section ).values.size();

	const int textWidth = sections.at( section ).sectionWidth - 6 -
		itemSideMargin * 2;

	Section::Type type = sections.at( section ).type;

	for( int i = 0; i < iterationsCount; ++i )
	{
		const QRect r( x, y, textWidth, itemHeight );

		const QString text = sections.at( section ).values.at( index );

		if( type == Section::DaySectionShort ||
			type == Section::DaySectionLong )
		{
			QStringList values = text.split( QLatin1Char( ' ' ) );

			p->setPen(
				lighterColor( opt.palette.color( QPalette::WindowText ), 75 ) );
			p->drawText( r, Qt::AlignLeft | Qt::TextSingleLine, values.at( 0 ) );

			p->setPen( opt.palette.color( QPalette::WindowText ) );
			p->drawText( r, Qt::AlignRight | Qt::TextSingleLine, values.at( 1 ) );
		}
		else
			p->drawText( r, Qt::AlignLeft | Qt::TextSingleLine, text );

		index = nextIndex( index, sections.at( section ).values.size() );
		y += itemHeight + itemTopMargin;
	}
}

void
DateTimePickerPrivate::drawWindow( QPainter * p, const QStyleOption & opt )
{
	const int windowOffset = itemHeight / 4;
	const int windowHeight = itemHeight + windowOffset * 2;
	const int windowMiddleHeight = windowHeight / 2;

	const int alpha = 150;
	const int alpha2 = 255;

	int yTop = currentItemY - windowOffset;
	int yBottom = yTop + windowMiddleHeight * 2;

	const QColor baseColor = q->palette().color( QPalette::Dark );

	QColor c1 = baseColor;
	c1.setAlpha( alpha2 );
	p->setPen( c1 );

	p->drawLine( 0, yTop, opt.rect.width(), yTop );
	p->drawLine( 0, yBottom, opt.rect.width(), yBottom );

	QColor c2 = lighterColor( baseColor, 110 );
	c2.setAlpha( alpha2 );
	p->setPen( c2 );

	p->drawLine( 0, yTop + 1, opt.rect.width(), yTop + 2 );

	QLinearGradient g( QPointF( 0.0, 0.0 ), QPointF( 0.0, 1.0 ) );
	g.setCoordinateMode( QGradient::ObjectBoundingMode );

	QColor c3 = lighterColor( baseColor, 95 );
	c3.setAlpha( alpha );
	g.setColorAt( 0.0, c3 );

	QColor c4 = lighterColor( baseColor, 50 );
	c4.setAlpha( alpha );
	g.setColorAt( 1.0, c4 );

	p->setPen( Qt::NoPen );
	p->setBrush( g );

	p->drawRect( 0, yTop + 2, opt.rect.width(), windowMiddleHeight - 2 );

	QColor c5 = lighterColor( baseColor, 35 );
	c5.setAlpha( alpha );
	p->setBrush( c5 );
	p->drawRect( 0, yTop + windowMiddleHeight,
		opt.rect.width(), windowMiddleHeight );
}

void
DateTimePickerPrivate::findMovableSection( const QPointF & pos )
{
	const int x = pos.x();

	int width = 0;

	for( int i = 0; i < sections.size(); ++i )
	{
		if( x >= width && x < width + sections.at( i ).sectionWidth )
		{
			movableSection = i;
			return;
		}

		width += sections.at( i ).sectionWidth;
	}

	movableSection = -1;
}

void
DateTimePickerPrivate::updateOffset( int delta )
{
	if( movableSection != -1 )
		sections[ movableSection ].offset += delta;
}

void
DateTimePickerPrivate::clearOffset()
{
	if( movableSection != -1 )
		sections[ movableSection ].offset = 0;
}

void
DateTimePickerPrivate::updateDaysIfNeeded()
{
	if( movableSection != -1 &&
		( movableSection == monthSection || movableSection == yearSection ) )
	{
		if( daysSection != -1 )
		{
			QDateTime dummy = value;
			QDate date = dummy.date();

			int year = dummy.date().year();
			int month = dummy.date().month();

			if( monthSection != -1 )
				month = sections[ monthSection ].currentIndex + 1;

			if( yearSection != -1 )
				year = minimum.date().year() + sections[ yearSection ].currentIndex;

			date.setDate( year, month, 28 );
			dummy.setDate( date );

			sections[ daysSection ].fillValues( dummy, minimum, maximum,
				false );

			if( sections[ daysSection ].currentIndex >
				sections[ daysSection ].values.size() - 1 )
			{
				sections[ daysSection ].currentIndex =
					sections[ daysSection ].values.size() - 1;
			}
		}
	}
}

void
DateTimePickerPrivate::updateCurrentDateTime()
{
	int year = value.date().year();
	int month = value.date().month();
	int day = -1;
	int hour = value.time().hour();
	int minute = value.time().minute();
	int second = value.time().second();
	int msecs = value.time().msec();
	int amPm = -1;

	for( int i = 0; i < sections.size(); ++i )
	{
		switch( sections.at( i ).type )
		{
			case Section::AmPmSection :
			{
				amPm = sections.at( i ).currentIndex;
			}
			break;

			case Section::SecondSection :
			{
				second = sections.at( i ).currentIndex;
			}
			break;

			case Section::MinuteSection :
			{
				minute = sections.at( i ).currentIndex;
			}
			break;

			case Section::Hour12Section :
			case Section::Hour24Section :
			{
				hour = sections.at( i ).currentIndex;
			}
			break;

			case Section::DaySection :
			case Section::DaySectionShort :
			case Section::DaySectionLong :
			{
				day = sections.at( i ).currentIndex + 1;
			}
			break;

			case Section::MonthSection :
			case Section::MonthSectionShort :
			case Section::MonthSectionLong :
			{
				month = sections.at( i ).currentIndex + 1;
			}
			break;

			case Section::YearSection :
			case Section::YearSection2Digits :
			{
				year = minimum.date().year() + sections.at( i ).currentIndex;
			}
			break;

			default:
				break;
		}
	}

	if( day == -1 )
		day = 1;

	if( amPm == 0 )
	{
		if( hour == 11 )
			hour = 0;
		else
			hour += 1;
	}
	else if( amPm == 1 )
	{
		if( hour == 11 )
			hour = 12;
		else
			hour += 12 + 1;
	}

	setValue( QDateTime( QDate( year, month, day ),
		QTime( hour, minute, second, msecs ), spec ), false );
}

void
DateTimePickerPrivate::initDaysMonthYearSectionIndex()
{
	int day = -1;
	int month = -1;
	int year = -1;

	for( int i = 0; i < sections.size(); ++i )
	{
		if( sections.at( i ).type & Section::DaySectionMask )
			day = i;
		else if( sections.at( i ).type & Section::MonthSectionMask )
			month = i;
		else if( sections.at( i ).type & Section::YearSectionMask )
			year = i;
	}

	daysSection = day;
	monthSection = month;
	yearSection = year;
}

void
DateTimePickerPrivate::fillValues( bool updateIndexes )
{
	for( int i = 0; i < sections.size(); ++i )
	{
		sections[ i ].fillValues( value, minimum, maximum,
			updateIndexes );

		if( sections.at( i ).currentIndex == -1 )
			sections[ i ].currentIndex = 0;
	}
}

void
DateTimePickerPrivate::releaseScrolling()
{
	leftMouseButtonPressed = false;

	clearOffset();
	updateDaysIfNeeded();
	updateCurrentDateTime();

	movableSection = -1;

	q->update();
}


//
// DateTimePicker
//


DateTimePicker::DateTimePicker( QWidget * parent )
	:	QWidget( parent )
	,	d( new DateTimePickerPrivate( this, QMetaType::QDateTime ) )
{
	setSizePolicy( QSizePolicy( QSizePolicy::Fixed,
		QSizePolicy::Fixed ) );

	connect( d->scroller, &Scroller::aboutToStart,
		this, &DateTimePicker::_q_scrollAboutToStart );

	connect( d->scroller, &Scroller::scroll,
		this, &DateTimePicker::_q_scroll );

	connect( d->scroller, &Scroller::finished,
		this, &DateTimePicker::_q_scrollFinished );
}

DateTimePicker::DateTimePicker( const QDateTime & dt, QWidget * parent )
	:	QWidget( parent )
	,	d( new DateTimePickerPrivate( this, QMetaType::QDateTime ) )
{
	setSizePolicy( QSizePolicy( QSizePolicy::Fixed,
		QSizePolicy::Fixed ) );

	setDateTime( dt.isValid() ? dt : DATETIMEPICKER_DATETIME_MIN );

	connect( d->scroller, &Scroller::aboutToStart,
		this, &DateTimePicker::_q_scrollAboutToStart );

	connect( d->scroller, &Scroller::scroll,
		this, &DateTimePicker::_q_scroll );

	connect( d->scroller, &Scroller::finished,
		this, &DateTimePicker::_q_scrollFinished );
}

DateTimePicker::DateTimePicker( const QDate & date, QWidget * parent )
	:	QWidget( parent )
	,	d( new DateTimePickerPrivate( this, QMetaType::QDate ) )
{
	setSizePolicy( QSizePolicy( QSizePolicy::Fixed,
		QSizePolicy::Fixed ) );

	setDate( date.isValid() ? date : DATETIMEPICKER_DATE_MIN );

	connect( d->scroller, &Scroller::aboutToStart,
		this, &DateTimePicker::_q_scrollAboutToStart );

	connect( d->scroller, &Scroller::scroll,
		this, &DateTimePicker::_q_scroll );

	connect( d->scroller, &Scroller::finished,
		this, &DateTimePicker::_q_scrollFinished );
}

DateTimePicker::DateTimePicker( const QTime & time, QWidget * parent )
	:	QWidget( parent )
	,	d( new DateTimePickerPrivate( this, QMetaType::QTime ) )
{
	setSizePolicy( QSizePolicy( QSizePolicy::Fixed,
		QSizePolicy::Fixed ) );

	setTime( time.isValid() ? time : DATETIMEPICKER_TIME_MIN );

	connect( d->scroller, &Scroller::aboutToStart,
		this, &DateTimePicker::_q_scrollAboutToStart );

	connect( d->scroller, &Scroller::scroll,
		this, &DateTimePicker::_q_scroll );

	connect( d->scroller, &Scroller::finished,
		this, &DateTimePicker::_q_scrollFinished );
}

DateTimePicker::DateTimePicker( const QVariant & val, QMetaType::Type parserType,
	QWidget * parent )
	:	QWidget( parent )
	,	d( new DateTimePickerPrivate( this, parserType ) )
{
	setSizePolicy( QSizePolicy( QSizePolicy::Fixed,
		QSizePolicy::Fixed ) );

	switch( val.metaType().id() )
	{
		case QMetaType::QDate :
			setDate( val.toDate() );
			break;

		case QMetaType::QTime :
			setTime( val.toTime() );
			break;

		case QMetaType::QDateTime :
			setDateTime( val.toDateTime() );
			break;

		default :
			setDateTime( QDateTime( DATETIMEPICKER_DATE_INITIAL,
				DATETIMEPICKER_TIME_MIN ) );
	}

	connect( d->scroller, &Scroller::aboutToStart,
		this, &DateTimePicker::_q_scrollAboutToStart );

	connect( d->scroller, &Scroller::scroll,
		this, &DateTimePicker::_q_scroll );

	connect( d->scroller, &Scroller::finished,
		this, &DateTimePicker::_q_scrollFinished );
}

DateTimePicker::~DateTimePicker()
{
}

QDateTime
DateTimePicker::dateTime() const
{
	return d->value;
}

QDate
DateTimePicker::date() const
{
	return d->value.date();
}

QTime
DateTimePicker::time() const
{
	return d->value.time();
}

QDateTime
DateTimePicker::minimumDateTime() const
{
	return d->minimum;
}

void
DateTimePicker::clearMinimumDateTime()
{
	setMinimumDateTime( QDateTime( DATETIMEPICKER_COMPAT_DATE_MIN,
		DATETIMEPICKER_TIME_MIN ) );
}

void
DateTimePicker::setMinimumDateTime( const QDateTime & dt )
{
	if( dt.isValid() && dt.date() >= DATETIMEPICKER_DATE_MIN )
	{
		const QDateTime m = dt.toTimeSpec( d->spec );
		d->setRange( m, ( d->maximum > m ? d->maximum : m ) );
	}
}

QDateTime
DateTimePicker::maximumDateTime() const
{
	return d->maximum;
}

void
DateTimePicker::clearMaximumDateTime()
{
	setMaximumDateTime( DATETIMEPICKER_DATETIME_MAX );
}

void
DateTimePicker::setMaximumDateTime( const QDateTime & dt )
{
	if( dt.isValid() && dt.date() <= DATETIMEPICKER_DATE_MAX )
	{
		const QDateTime m = dt.toTimeSpec( d->spec );
		d->setRange( ( d->minimum < m ? d->minimum : m ), m );
	}
}

void
DateTimePicker::setDateTimeRange( const QDateTime & min, const QDateTime & max )
{
	const QDateTime minimum = min.toTimeSpec( d->spec );
	QDateTime maximum = max.toTimeSpec( d->spec );
	if( min > max )
		maximum = minimum;
	d->setRange( minimum, maximum );
}

QDate
DateTimePicker::minimumDate() const
{
	return d->minimum.date();
}

void
DateTimePicker::setMinimumDate( const QDate & min )
{
	if( min.isValid() && min >= DATETIMEPICKER_DATE_MIN )
		setMinimumDateTime( QDateTime( min, d->minimum.time(), d->spec ) );
}

void
DateTimePicker::clearMinimumDate()
{
	setMinimumDate( DATETIMEPICKER_DATE_MIN );
}

QDate
DateTimePicker::maximumDate() const
{
	return d->maximum.date();
}

void
DateTimePicker::setMaximumDate( const QDate & max )
{
	if( max.isValid() )
		setMaximumDateTime( QDateTime( max, d->maximum.time(), d->spec ) );
}

void
DateTimePicker::clearMaximumDate()
{
	setMaximumDate( DATETIMEPICKER_DATE_MAX );
}

void
DateTimePicker::setDateRange( const QDate & min, const QDate & max )
{
	if( min.isValid() && max.isValid() )
		setDateTimeRange( QDateTime( min, d->minimum.time(), d->spec ),
			QDateTime( max, d->maximum.time(), d->spec ) );
}

QTime
DateTimePicker::minimumTime() const
{
	return d->minimum.time();
}

void
DateTimePicker::setMinimumTime( const QTime & min )
{
	if( min.isValid() )
	{
		const QDateTime m( d->minimum.date(), min, d->spec );
		setMinimumDateTime( m );
	}
}

void
DateTimePicker::clearMinimumTime()
{
	setMinimumTime( DATETIMEPICKER_TIME_MIN );
}

QTime
DateTimePicker::maximumTime() const
{
	return d->maximum.time();
}

void
DateTimePicker::setMaximumTime( const QTime & max )
{
	if( max.isValid() )
	{
		const QDateTime m( d->maximum.date(), max, d->spec );
		setMaximumDateTime( m );
	}
}

void
DateTimePicker::clearMaximumTime()
{
	setMaximumTime( DATETIMEPICKER_TIME_MAX );
}

void
DateTimePicker::setTimeRange( const QTime & min, const QTime & max )
{
	if( min.isValid() && max.isValid() )
		setDateTimeRange( QDateTime( d->minimum.date(), min, d->spec ),
			QDateTime( d->maximum.date(), max, d->spec ) );
}

QString
DateTimePicker::format() const
{
	return d->format;
}

void
DateTimePicker::setFormat( const QString & format )
{
	if( d->parseFormat( format ) )
	{
		d->initDaysMonthYearSectionIndex();
		d->fillValues();
		updateGeometry();
		update();
	}
}

Qt::TimeSpec
DateTimePicker::timeSpec() const
{
	return d->spec;
}

void
DateTimePicker::setTimeSpec( Qt::TimeSpec spec )
{
	if( spec != d->spec )
	{
		d->spec = spec;
		d->updateTimeSpec();
	}
}

Scroller *
DateTimePicker::scroller() const
{
	return d->scroller;
}

QSize
DateTimePicker::sizeHint() const
{
	QStyleOption opt;
	opt.initFrom( this );

	d->itemHeight = opt.fontMetrics.boundingRect( QLatin1String( "AM" ) )
		.height();

	d->itemTopMargin = d->itemHeight / 3;

	d->widgetHeight = d->itemHeight * d->itemsMaxCount +
		( d->itemsMaxCount - 1 ) * d->itemTopMargin;

	d->currentItemY = d->widgetHeight / 2 - d->itemHeight / 2;

	int widgetWidth = 0;

	for( int i = 0; i < d->sections.size(); ++i )
	{
		d->sections[ i ].sectionWidth = d->sections.at( i ).maxWidth( opt );
		d->sections[ i ].sectionWidth += d->itemSideMargin * 2 + 6;
		widgetWidth += d->sections[ i ].sectionWidth;
	}

	return QSize( widgetWidth, d->widgetHeight );
}

void
DateTimePicker::setDateTime( const QDateTime & dateTime )
{
	if( dateTime.isValid() )
		d->setValue( dateTime.toTimeSpec( d->spec ) );
}

void
DateTimePicker::setDate( const QDate & date )
{
	if( date.isValid() )
		d->setValue( QDateTime( date, d->value.time(), d->spec ) );
}

void
DateTimePicker::setTime( const QTime & time )
{
	if( time.isValid() )
		d->setValue( QDateTime( d->value.date(), time, d->spec ) );
}

void
DateTimePicker::wheelEvent( QWheelEvent * event )
{
	QPoint numDegrees = event->angleDelta();

	if( !numDegrees.isNull() )
	{
		d->findMovableSection( event->position() );

		if( numDegrees.y() > 0 )
			d->updateOffset( d->itemHeight + d->itemTopMargin );
		else if( numDegrees.y() < 0 )
			d->updateOffset( -( d->itemHeight + d->itemTopMargin ) );

		if( numDegrees.y() != 0 )
		{
			d->normalizeOffsets();
			d->clearOffset();
			d->updateDaysIfNeeded();
			d->updateCurrentDateTime();

			d->movableSection = -1;

			update();
		}
	}

	event->accept();
}

void
DateTimePicker::mousePressEvent( QMouseEvent * event )
{
	if( event->button() == Qt::LeftButton )
	{
		d->scrolling = false;
		d->mousePos = event->pos();
		d->leftMouseButtonPressed = true;
		d->findMovableSection( event->pos() );

		event->accept();
	}
	else
		event->ignore();
}

void
DateTimePicker::mouseMoveEvent( QMouseEvent * event )
{
	if( d->leftMouseButtonPressed )
	{
		const int delta = event->pos().y() - d->mousePos.y();
		d->updateOffset( delta );
		d->mousePos = event->pos();
		update();

		event->accept();
	}
	else
		event->ignore();
}

void
DateTimePicker::mouseReleaseEvent( QMouseEvent * event )
{
	if( event->button() == Qt::LeftButton )
	{
		if( d->leftMouseButtonPressed && !d->scrolling )
			d->releaseScrolling();

		event->accept();
	}
	else
		event->ignore();
}

void
DateTimePicker::paintEvent( QPaintEvent * )
{
	d->normalizeOffsets();

	QStyleOption opt;
	opt.initFrom( this );

	QPainter p( this );

	int x = 0;

	for( int i = 0; i < d->sections.size(); ++i )
	{
		const QRect r( x, 0, d->sections.at( i ).sectionWidth, d->widgetHeight );

		drawCylinder( &p, r, palette().color( QPalette::Dark ),
			( i == 0 ), ( i == d->sections.size() - 1 ) );

		d->drawSectionItems( i, &p, opt );

		x += d->sections.at( i ).sectionWidth;
	}

	d->drawWindow( &p, opt );
}

void
DateTimePicker::_q_scroll( int dx, int dy )
{
	Q_UNUSED( dx )

	d->updateOffset( dy );

	update();
}

void
DateTimePicker::_q_scrollAboutToStart()
{
	d->scrolling = true;
}

void
DateTimePicker::_q_scrollFinished()
{
	d->scrolling = false;

	d->releaseScrolling();
}

} /* namespace QtMWidgets */
