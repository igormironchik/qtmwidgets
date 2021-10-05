
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

#ifndef QTMWIDGETS__DATETIMEPICKER_HPP__INCLUDED
#define QTMWIDGETS__DATETIMEPICKER_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>
#include <QVariant>
#include <QDateTime>


namespace QtMWidgets {

class Scroller;


//
// DateTimePicker
//

class DateTimePickerPrivate;

/*!
	The DateTimePicker class provides widget for selecting
	date and time. DateTimePicker shows cylinders with separate
	components of date and time, i.e. year, month, day, hour, minute,
	etc. By default format of the DateTimePicker is "dddd MMMM yyyy hh mm",
	that mean that widgets will show 5 cylinders with year, long month name,
	long day name with day number, hour and minute. \sa setFormat().

	DateTimePicker allows to select date and time from the given
	range. By default minimum date and time is September 14, 1752 00:00:00
	and maximum is December 31, 7999 23:59:59.
*/
class DateTimePicker
	:	public QWidget
{
	Q_OBJECT

	/*!
		\property dateTime

		\brief the QDateTime that is set in the DateTimePicker

		When setting this property the timespec of the QDateTimePicker remains the same
		and the timespec of the new QDateTime is ignored.

		By default, this property contains a date that refers to January 1,
		2000 and a time of 00:00:00 and 0 milliseconds.

		\sa date, time
	*/
	Q_PROPERTY( QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged USER true )
	/*!
		\property date

		\brief the QDate that is set in the widget

		By default, this property contains a date that refers to January 1, 2000.

		\sa time, dateTime
	*/
	Q_PROPERTY( QDate date READ date WRITE setDate NOTIFY dateChanged )
	/*!
		\property time

		\brief the QTime that is set in the widget

		By default, this property contains a time of 00:00:00 and 0 milliseconds.

		\sa date, dateTime
	*/
	Q_PROPERTY( QTime time READ time WRITE setTime NOTIFY timeChanged )
	/*!
		\property maximumDateTime

		\brief the maximum datetime of the date time picker

		When setting this property the \link minimumDateTime minimumDateTime \endlink
		is adjusted if
		necessary to ensure that the range remains valid. If the datetime is
		not a valid QDateTime object, this function does nothing.

		The default maximumDateTime can be restored with
		clearMaximumDateTime().

		By default, this property contains a date that refers to 31 December,
		7999 and a time of 23:59:59 and 999 milliseconds.

		\sa minimumDateTime(), minimumTime(), maximumTime(), minimumDate(),
		maximumDate(), setDateTimeRange(), setDateRange(), setTimeRange(),
		clearMinimumDateTime(), clearMinimumDate(),
		clearMaximumDate(), clearMinimumTime(), clearMaximumTime()
	*/
	Q_PROPERTY( QDateTime maximumDateTime READ maximumDateTime WRITE setMaximumDateTime RESET clearMaximumDateTime )
	/*!
		\property minimumDateTime

		\brief the minimum datetime of the date time picker

		When setting this property the \link maximumDateTime maximumDateTime \endlink
		is adjusted if
		necessary to ensure that the range remains valid. If the datetime is
		not a valid QDateTime object, this function does nothing.

		The default minimumDateTime can be restored with
		clearMinimumDateTime()

		By default, this property contains a date that refers to January 1,
		1900 and a time of 00:00:00 and 0 milliseconds.

		\sa maximumDateTime(), minimumTime(), maximumTime(), minimumDate(),
		maximumDate(), setDateTimeRange(), setDateRange(), setTimeRange(),
		clearMaximumDateTime(), clearMinimumDate(),
		clearMaximumDate(), clearMinimumTime(), clearMaximumTime()
	*/
	Q_PROPERTY( QDateTime minimumDateTime READ minimumDateTime WRITE setMinimumDateTime RESET clearMinimumDateTime )
	/*!
		\property maximumDate

		\brief the maximum date of the date time picker

		When setting this property the \link minimumDate minimumDate \endlink
		is adjusted if
		necessary to ensure that the range remains valid. If the date is
		not a valid QDate object, this function does nothing.

		By default, this property contains a date that refers to December 31, 7999.

		\sa minimumDate, minimumTime, maximumTime, setDateRange()
	*/
	Q_PROPERTY( QDate maximumDate READ maximumDate WRITE setMaximumDate RESET clearMaximumDate )
	/*!
		\property minimumDate

		\brief the minimum date of the date time picker

		When setting this property the \link maximumDate maximumDate \endlink
		is adjusted if
		necessary, to ensure that the range remains valid. If the date is
		not a valid QDate object, this function does nothing.

		By default, this property contains a date that refers to January 1, 1900.
		The minimum date must be at least the first day in year 100, otherwise
		setMinimumDate() has no effect.

		\sa minimumTime, maximumTime, setDateRange()
	*/
	Q_PROPERTY( QDate minimumDate READ minimumDate WRITE setMinimumDate RESET clearMinimumDate )
	/*!
		\property maximumTime

		\brief the maximum time of the date time edit

		When setting this property, the \link minimumTime minimumTime \endlink
		is adjusted if
		necessary to ensure that the range remains valid. If the time is
		not a valid QTime object, this function does nothing.

		By default, this property contains a time of 23:59:59 and 999 milliseconds.

		\sa minimumTime, minimumDate, maximumDate, setTimeRange()
	*/
	Q_PROPERTY( QTime maximumTime READ maximumTime WRITE setMaximumTime RESET clearMaximumTime )
	/*!
		\property minimumTime

		\brief the minimum time of the date time picker

		When setting this property the \link maximumTime maximumTime \endlink
		is adjusted if
		necessary, to ensure that the range remains valid. If the time is
		not a valid QTime object, this function does nothing.

		By default, this property contains a time of 00:00:00 and 0 milliseconds.

		\sa maximumTime, minimumDate, maximumDate, setTimeRange()
	*/
	Q_PROPERTY( QTime minimumTime READ minimumTime WRITE setMinimumTime RESET clearMinimumTime )
	/*!
		\property timeSpec

		\brief the current timespec used by the date time picker
	*/
	Q_PROPERTY( Qt::TimeSpec timeSpec READ timeSpec WRITE setTimeSpec )

signals:
	/*!
		This signal is emitted whenever the date or time is changed. The
		new date and time is passed in \a dateTime.
	*/
	void dateTimeChanged( const QDateTime & dateTime );
	/*!
		This signal is emitted whenever the time is changed. The new time
		is passed in \a time.
	*/
	void timeChanged( const QTime & time );
	/*!
		This signal is emitted whenever the date is changed. The new date
		is passed in \a date.
	*/
	void dateChanged( const QDate & date );

public:
	explicit DateTimePicker( QWidget * parent = 0 );
	explicit DateTimePicker( const QDateTime & dt, QWidget * parent = 0 );
	explicit DateTimePicker( const QDate & date, QWidget * parent = 0 );
	explicit DateTimePicker( const QTime & time, QWidget * parent = 0 );

	virtual ~DateTimePicker();

	/*!
		\return Date and time.

		\sa dateTime.
	*/
	QDateTime dateTime() const;
	/*!
		\return Date.

		\sa date.
	*/
	QDate date() const;
	/*!
		\return Time.

		\sa time.
	*/
	QTime time() const;

	/*!
		\return Minimum date and time.

		\sa minimumDateTime.
	*/
	QDateTime minimumDateTime() const;
	/*!
		Clear minimum date and time to the default value.

		\sa minimumDateTime.
	*/
	void clearMinimumDateTime();
	/*!
		Set minimum date and time.

		\sa minimumDateTime.
	*/
	void setMinimumDateTime( const QDateTime & dt );

	/*!
		\return Maximum date and time.

		\sa maximumDateTime.
	*/
	QDateTime maximumDateTime() const;
	/*!
		Clear maximum date and time to the default value.

		\sa maximumDateTime.
	*/
	void clearMaximumDateTime();
	/*!
		Set maximum date and time.

		\sa maximumDateTime.
	*/
	void setMaximumDateTime( const QDateTime & dt );

	/*!
		Convenience function to set minimum and maximum date time with one
		function call.

		If either \a min or \a max are not valid, this function does
		nothing.

		\sa setMinimumDate(), maximumDate(), setMaximumDate(),
		clearMinimumDate(), setMinimumTime(), maximumTime(),
		setMaximumTime(), clearMinimumTime(), QDateTime::isValid()
	*/
	void setDateTimeRange( const QDateTime & min, const QDateTime & max );

	/*!
		\return Minimum date.

		\sa minimumDate.
	*/
	QDate minimumDate() const;
	/*!
		Set minimum date.

		\sa minimumDate.
	*/
	void setMinimumDate( const QDate & min );
	/*!
		Clear minimum date to the default value.

		\sa minimumDate.
	*/
	void clearMinimumDate();

	/*!
		\return Maximum date.

		\sa maximumDate.
	*/
	QDate maximumDate() const;
	/*!
		Set maximum date.

		\sa maximumDate.
	*/
	void setMaximumDate( const QDate & max );
	/*!
		Clear maximum date to the default value.

		\sa maximumDate.
	*/
	void clearMaximumDate();

	/*!
		Convenience function to set minimum and maximum date with one
		function call.

		If either \a min or \a max are not valid, this function does
		nothing.

		\sa setMinimumDate(), maximumDate(), setMaximumDate(),
		clearMinimumDate(), setMinimumTime(), maximumTime(),
		setMaximumTime(), clearMinimumTime(), QDate::isValid()
	*/
	void setDateRange( const QDate & min, const QDate & max );

	/*!
		\return Minimum time.

		\sa minimumTime.
	*/
	QTime minimumTime() const;
	/*!
		Set minimum time.

		\sa minimumTime.
	*/
	void setMinimumTime( const QTime & min );
	/*!
		Clear minimum time to the default value.

		\sa minimumTime.
	*/
	void clearMinimumTime();

	/*!
		\return Maximum time.

		\sa maximumTime.
	*/
	QTime maximumTime() const;
	/*!
		Set maximum time.

		\sa maximumTime.
	*/
	void setMaximumTime( const QTime & max );
	/*!
		Clear maximum time to the default value.

		\sa maximumTime.
	*/
	void clearMaximumTime();

	/*!
		Convenience function to set minimum and maximum time with one
		function call.

		If either \a min or \a max are not valid, this function does
		nothing.

		\sa setMinimumDate(), maximumDate(), setMaximumDate(),
		clearMinimumDate(), setMinimumTime(), maximumTime(),
		setMaximumTime(), clearMinimumTime(), QTime::isValid()
	*/
	void setTimeRange( const QTime & min, const QTime & max );

	/*!
		\return current format of the date & time used by widget.

		\sa setFormat()
	*/
	QString format() const;
	/*!
		\note Only one specifier for the date/time value can be used. I.e
		if you defined "mm" in the format string then there can't be "m",
		otherwise format will not be parsed.

		In format string possible to use space character, it will be simple
		ignored. In which order sections will be defined in that they will
		appear in the widget. I.e. format string "yyyy MM dd" mean that widget
		will have thre sections/cylinders, where first will display year in four
		digit format, than mont with two digit format and day with two digit
		format.

		These expressions may be used for the date part of the format string:

		Expression | Output
		---------- | ------------------------------------------------------------------------------------------
		d          | the day as number without a leading zero (1 to 31)
		dd         | the day as number with a leading zero (01 to 31)
		ddd        | the abbreviated localized day name (e.g. 'Mon' to 'Sun'). Uses QDate::shortDayName().
		dddd       | the long localized day name (e.g. 'Monday' to 'Sunday'). Uses QDate::longDayName().
		M          | the month as number without a leading zero (1-12)
		MM         | the month as number with a leading zero (01-12)
		MMM        | the abbreviated localized month name (e.g. 'Jan' to 'Dec'). Uses QDate::shortMonthName().
		MMMM       | the long localized month name (e.g. 'January' to 'December'). Uses QDate::longMonthName().
		yy         | the year as two digit number (00-99)
		yyyy       | the year as four digit number

		These expressions may be used for the time part of the format string:

		Expression | Output
		---------- | ---------------------------------------------------------------------
		h          | the hour without a leading zero (0 to 23 or 1 to 12 if AM/PM display)
		hh         | the hour with a leading zero (00 to 23 or 01 to 12 if AM/PM display)
		m          | the minute without a leading zero (0 to 59)
		mm         | the minute with a leading zero (00 to 59)
		s          | the second without a leading zero (0 to 59)
		ss         | the second with a leading zero (00 to 59)
		a          | Interpret as an AM/PM time. ap must be either "AM" or "PM".
	*/
	void setFormat( const QString & format );

	/*!
		\return Time spec used by widget.

		\sa timeSpec.
	*/
	Qt::TimeSpec timeSpec() const;
	/*!
		Set time spec.

		\sa timeSpec.
	*/
	void setTimeSpec( Qt::TimeSpec spec );

	//! \return Scroller interface.
	Scroller * scroller() const;

	QSize sizeHint() const override;

public slots:
	/*!
		Set date and time.

		\sa dateTime.
	*/
	void setDateTime( const QDateTime & dateTime );
	/*!
		Set date.

		\sa date.
	*/
	void setDate( const QDate & date );
	/*!
		Set time.

		\sa time.
	*/
	void setTime( const QTime & time );

protected:
	void wheelEvent( QWheelEvent * event ) override;
	void mousePressEvent( QMouseEvent * event ) override;
	void mouseMoveEvent( QMouseEvent * event ) override;
	void mouseReleaseEvent( QMouseEvent * event ) override;
	void paintEvent( QPaintEvent * event ) override;

	DateTimePicker( const QVariant & val, QMetaType::Type parserType,
		QWidget * parent = 0 );

private slots:
	void _q_scroll( int dx, int dy );
	void _q_scrollAboutToStart();
	void _q_scrollFinished();

private:
	friend class DateTimePickerPrivate;

	Q_DISABLE_COPY( DateTimePicker )

	QScopedPointer< DateTimePickerPrivate > d;
}; // class DateTimePicker

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__DATETIMEPICKER_HPP__INCLUDED
