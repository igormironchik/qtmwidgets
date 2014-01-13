
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

// Qt include.
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
#include <QBrush>
#include <QPen>


namespace QtMWidgets {

//
// DateTimePickerPrivate
//

class DateTimePickerPrivate
	:	public DateTimeParser
{
public:
	DateTimePickerPrivate( DateTimePicker * parent,
		QVariant::Type parserType )
		:	DateTimeParser( parserType )
		,	q( parent )
		,	spec( Qt::LocalTime )
	{
	}

	void updateTimeSpec();
	void setRange( const QDateTime & min, const QDateTime & max );
	void setValue( const QDateTime & dt );
	void emitSignals();

	DateTimePicker * q;
	QDateTime minimum;
	QDateTime maximum;
	QDateTime value;
	Qt::TimeSpec spec;
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
		q->update();
	}
}

void
DateTimePickerPrivate::setValue( const QDateTime & dt )
{
	if( value != dt )
	{
		if( dt >= minimum && dt <= maximum )
			value = dt;
		else if( dt < minimum )
			value = minimum;
		else if( dt > maximum )
			value = maximum;

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


//
// DateTimePicker
//


DateTimePicker::DateTimePicker( QWidget * parent )
	:	QWidget( parent )
	,	d( new DateTimePickerPrivate( this, QVariant::DateTime ) )
{
	setMinimumDateTime( QDateTime( DATETIMEPICKER_COMPAT_DATE_MIN,
		DATETIMEPICKER_TIME_MIN ) );
	setMaximumDateTime( DATETIMEPICKER_DATETIME_MAX );
	setDateTime( QDateTime( DATETIMEPICKER_DATE_INITIAL,
		DATETIMEPICKER_TIME_MIN ) );
}

DateTimePicker::DateTimePicker( const QDateTime & dt, QWidget * parent )
	:	QWidget( parent )
	,	d( new DateTimePickerPrivate( this, QVariant::DateTime ) )
{
	setMinimumDateTime( QDateTime( DATETIMEPICKER_COMPAT_DATE_MIN,
		DATETIMEPICKER_TIME_MIN ) );
	setMaximumDateTime( DATETIMEPICKER_DATETIME_MAX );

	setDateTime( dt.isValid() ? dt : DATETIMEPICKER_DATETIME_MIN );
}

DateTimePicker::DateTimePicker( const QDate & date, QWidget * parent )
	:	QWidget( parent )
	,	d( new DateTimePickerPrivate( this, QVariant::Date ) )
{
	setMinimumDateTime( QDateTime( DATETIMEPICKER_COMPAT_DATE_MIN,
		DATETIMEPICKER_TIME_MIN ) );
	setMaximumDateTime( DATETIMEPICKER_DATETIME_MAX );
	setDate( date.isValid() ? date : DATETIMEPICKER_DATE_MIN );
}

DateTimePicker::DateTimePicker( const QTime & time, QWidget * parent )
	:	QWidget( parent )
	,	d( new DateTimePickerPrivate( this, QVariant::Time ) )
{
	setMinimumDateTime( QDateTime( DATETIMEPICKER_COMPAT_DATE_MIN,
		DATETIMEPICKER_TIME_MIN ) );
	setMaximumDateTime( DATETIMEPICKER_DATETIME_MAX );
	setTime( time.isValid() ? time : DATETIMEPICKER_TIME_MIN );
}

DateTimePicker::DateTimePicker( const QVariant & val, QVariant::Type parserType,
	QWidget * parent )
	:	QWidget( parent )
	,	d( new DateTimePickerPrivate( this, parserType ) )
{
	setMinimumDateTime( QDateTime( DATETIMEPICKER_COMPAT_DATE_MIN,
		DATETIMEPICKER_TIME_MIN ) );
	setMaximumDateTime( DATETIMEPICKER_DATETIME_MAX );

	switch( val.type() )
	{
		case QVariant::Date :
			setDate( val.toDate() );
			break;

		case QVariant::Time :
			setTime( val.toTime() );
			break;

		case QVariant::DateTime :
			setDateTime( val.toDateTime() );
			break;

		default :
			setDateTime( QDateTime( DATETIMEPICKER_DATE_INITIAL,
				DATETIMEPICKER_TIME_MIN ) );
	}
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

QSize
DateTimePicker::sizeHint() const
{
	return QSize();
}

void
DateTimePicker::setDateTime( const QDateTime & dateTime )
{
	if( dateTime.isValid() )
		d->setValue( QDateTime( dateTime.date(), dateTime.time(), d->spec ) );
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

}

void
DateTimePicker::mousePressEvent( QMouseEvent * event )
{

}

void
DateTimePicker::mouseMoveEvent( QMouseEvent * event )
{

}

void
DateTimePicker::mouseReleaseEvent( QMouseEvent * event )
{

}

void
DateTimePicker::paintEvent( QPaintEvent * event )
{

}

} /* namespace QtMWidgets */
