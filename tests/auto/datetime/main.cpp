
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

// Qt include.
#include <QObject>
#include <QtTest/QtTest>
#include <QSharedPointer>
#include <QStyleOption>

// QtMWidgets include.
#include <QtMWidgets/DateTimePicker>
#include <QtMWidgets/DatePicker>
#include <QtMWidgets/TimePicker>

#include <QtMWidgets/private/datetimeparser.hpp>


class TestDateTime
	:	public QObject
{
	Q_OBJECT

private slots:

	void initTestCase()
	{
		m_dt.reset( new QtMWidgets::DateTimePicker );
		m_dt->setFormat( QStringLiteral( "ddd MMMM yyyy hh mm a" ) );

		m_dt->setMouseTracking( true );

		m_font = m_dt->font();
		m_font.setBold( true );
		m_dt->setFont( m_font );

		QFontMetrics fm( m_font );
		m_height = fm.boundingRect( QLatin1String( "AM" ) ).height();
		m_height += m_height / 3;

		m_delta = QPoint( 0, -m_height );

		m_d.reset( new QtMWidgets::DatePicker );
		m_d->setFormat( QStringLiteral( "dddd MMM yyyy" ) );
		m_d->setFont( m_font );

		m_t.reset( new QtMWidgets::TimePicker );
		m_t->setFormat( QStringLiteral( "hh mm" ) );
		m_t->setFont( m_font );

		{
			QStyleOption opt;
			opt.initFrom( m_dt.data() );

			QtMWidgets::Section s1( QtMWidgets::Section::DaySectionShort );

			int sw = s1.maxWidth( opt ) + 5 * 2 + 6;
			int w = sw;

			m_dtSections.append( sw / 2 );

			QtMWidgets::Section s2( QtMWidgets::Section::MonthSectionLong );

			sw = s2.maxWidth( opt ) + 5 * 2 + 6;

			m_dtSections.append( w + sw / 2 );

			w += sw;

			QtMWidgets::Section s3( QtMWidgets::Section::YearSection );

			sw = s3.maxWidth( opt ) + 5 * 2 + 6;

			m_dtSections.append( w + sw / 2 );

			w += sw;

			QtMWidgets::Section s4( QtMWidgets::Section::Hour12Section );

			sw = s4.maxWidth( opt ) + 5 * 2 + 6;

			m_dtSections.append( w + sw / 2 );

			w += sw;

			QtMWidgets::Section s5( QtMWidgets::Section::MinuteSection );

			sw = s5.maxWidth( opt ) + 5 * 2 + 6;

			m_dtSections.append( w + sw / 2 );

			w += sw;

			QtMWidgets::Section s6( QtMWidgets::Section::AmPmSection );

			sw = s6.maxWidth( opt ) + 5 * 2 + 6;

			m_dtSections.append( w + sw / 2 );
		}

		{
			QStyleOption opt;
			opt.initFrom( m_d.data() );

			QtMWidgets::Section s1( QtMWidgets::Section::DaySectionLong );

			int sw = s1.maxWidth( opt ) + 5 * 2 + 6;
			int w = sw;

			m_dSections.append( sw / 2 );

			QtMWidgets::Section s2( QtMWidgets::Section::MonthSectionShort );

			sw = s2.maxWidth( opt ) + 5 * 2 + 6;

			m_dSections.append( w + sw / 2 );

			w += sw;

			QtMWidgets::Section s3( QtMWidgets::Section::YearSection );

			sw = s3.maxWidth( opt ) + 5 * 2 + 6;

			m_dSections.append( w + sw / 2 );
		}

		{
			QStyleOption opt;
			opt.initFrom( m_t.data() );

			QtMWidgets::Section s4( QtMWidgets::Section::Hour24Section );

			int sw = s4.maxWidth( opt ) + 5 * 2 + 6;
			int w = sw;

			m_tSections.append( sw / 2 );

			QtMWidgets::Section s5( QtMWidgets::Section::MinuteSection );

			sw = s5.maxWidth( opt ) + 5 * 2 + 6;

			m_tSections.append( w + sw / 2 );
		}
	}

	void testDateTime()
	{
		m_dt->show();

		QVERIFY( QTest::qWaitForWindowActive( m_dt.data() ) );

		QVERIFY( m_dt->dateTime() == QDateTime( DATETIMEPICKER_DATE_INITIAL,
			DATETIMEPICKER_TIME_MIN ) );

		const QDateTime dt = { { 2020, 10, 24 }, { 13, 12 } };
		m_dt->setDateTime( dt );

		QVERIFY( m_dt->minimumDate() == DATETIMEPICKER_COMPAT_DATE_MIN );
		QVERIFY( m_dt->maximumDate() == DATETIMEPICKER_DATE_MAX );
		QVERIFY( m_dt->minimumTime() == DATETIMEPICKER_TIME_MIN );
		QVERIFY( m_dt->maximumTime() == DATETIMEPICKER_TIME_MAX );
		QVERIFY( m_dt->minimumDateTime() == DATETIMEPICKER_COMPAT_DATETIME_MIN );
		QVERIFY( m_dt->maximumDateTime() == DATETIMEPICKER_DATETIME_MAX );

		QVERIFY( m_dt->dateTime() == dt );

		m_dt->update();

		QTest::qWait( 500 );

		{
			QPoint p( m_dtSections.at( 0 ), m_dt->height() / 2 );
			QTest::mousePress( m_dt.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_dt.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_dt.data(), Qt::LeftButton, {}, p + m_delta, 20 );
			QTest::qWait( 500 );

			QVERIFY( m_dt->dateTime() == QDateTime( { 2020, 10, 25 }, { 13, 12 } ) );
		}

		{
			QPoint p( m_dtSections.at( 1 ), m_dt->height() / 2 );
			QTest::mousePress( m_dt.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_dt.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_dt.data(), Qt::LeftButton, {}, p + m_delta, 20 );
			QTest::qWait( 500 );

			QVERIFY( m_dt->dateTime() == QDateTime( { 2020, 11, 25 }, { 13, 12 } ) );
		}

		{
			QPoint p( m_dtSections.at( 2 ), m_dt->height() / 2 );
			QTest::mousePress( m_dt.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_dt.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_dt.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_dt->dateTime() == QDateTime( { 2021, 11, 25 }, { 13, 12 } ) );
		}

		{
			QPoint p( m_dtSections.at( 3 ), m_dt->height() / 2 );
			QTest::mousePress( m_dt.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_dt.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_dt.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_dt->dateTime() == QDateTime( { 2021, 11, 25 }, { 14, 12 } ) );
		}

		{
			QPoint p( m_dtSections.at( 4 ), m_dt->height() / 2 );
			QTest::mousePress( m_dt.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_dt.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_dt.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_dt->dateTime() == QDateTime( { 2021, 11, 25 }, { 14, 13 } ) );
		}

		{
			QPoint p( m_dtSections.at( 5 ), m_dt->height() / 2 );
			QTest::mousePress( m_dt.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p - m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_dt.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_dt.data(), Qt::LeftButton, {}, p - m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_dt->dateTime() == QDateTime( { 2021, 11, 25 }, { 2, 13 } ) );
		}

		m_dt->hide();
	}

	void testDate()
	{
		m_d->show();

		QVERIFY( QTest::qWaitForWindowActive( m_d.data() ) );

		const QDate d = { 2020, 10, 24 };
		m_d->setDate( d );

		QVERIFY( m_d->date() == d );

		{
			QPoint p( m_dSections.at( 0 ), m_d->height() / 2 );
			QTest::mousePress( m_d.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_d.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_d.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_d->date() == QDate( 2020, 10, 25 ) );
		}

		{
			QPoint p( m_dSections.at( 1 ), m_d->height() / 2 );
			QTest::mousePress( m_d.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p - m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_d.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_d.data(), Qt::LeftButton, {}, p - m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_d->date() == QDate( 2020, 9, 25 ) );
		}

		{
			QPoint p( m_dSections.at( 2 ), m_d->height() / 2 );
			QTest::mousePress( m_d.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_d.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_d.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QTest::mousePress( m_d.data(), Qt::LeftButton, {}, p, 20 );
			QApplication::sendEvent( m_d.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_d.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_d->date() == QDate( 2022, 9, 25 ) );
		}

		m_d->hide();
	}

	void testTime()
	{
		m_t->show();

		QVERIFY( QTest::qWaitForWindowActive( m_t.data() ) );

		const QTime t = { 13, 12 };
		m_t->setTime( t );

		QVERIFY( m_t->time() == t );

		{
			QPoint p( m_tSections.at( 0 ), m_t->height() / 2 );
			QTest::mousePress( m_t.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_t.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_t.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_t->time() == QTime( 14, 12 ) );
		}

		{
			QPoint p( m_tSections.at( 1 ), m_t->height() / 2 );
			QTest::mousePress( m_t.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_t.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_t.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_t->time() == QTime( 14, 13 ) );
		}

		for( int i = 14; i < 24; ++i )
		{
			QPoint p( m_tSections.at( 0 ), m_t->height() / 2 );
			QTest::mousePress( m_t.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_t.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_t.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_t->time() == QTime( ( i + 1 < 24 ? i + 1 : 0 ), 13 ) );
		}

		{
			QPoint p( m_tSections.at( 0 ), m_t->height() / 2 );
			QTest::mousePress( m_t.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_t.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_t.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_t->time() == QTime( 1, 13 ) );
		}

		m_t->hide();
	}

	void testDateTime2()
	{
		m_dt->setFormat( QStringLiteral( "d M yy h m s" ) );
		m_dt->setTimeSpec( Qt::UTC );
		m_dt->setDateTime( { { 2020, 10, 12 }, { 15, 12, 00 }, Qt::UTC } );
		m_dt->setMinimumDate( { 2019, 1, 1 } );
		m_dt->setMaximumDate( { 2021, 12, 31 } );

		m_dt->show();

		QVERIFY( m_dt->timeSpec() == Qt::UTC );
		QVERIFY( m_dt->minimumDate() == QDate( 2019, 1, 1 ) );
		QVERIFY( m_dt->maximumDate() == QDate( 2021, 12, 31 ) );
		QVERIFY( m_dt->dateTime() == QDateTime( { 2020, 10, 12 }, { 15, 12, 00 }, Qt::UTC ) );

		m_dt->setDateTimeRange( { { 2019, 1, 1 }, { 0, 0 }, Qt::UTC },
			{ { 2021, 12, 31 }, { 23, 59, 59, 999 }, Qt::UTC } );

		QVERIFY( m_dt->minimumDate() == QDate( 2019, 1, 1 ) );
		QVERIFY( m_dt->maximumDate() == QDate( 2021, 12, 31 ) );
		QVERIFY( m_dt->dateTime() == QDateTime( { 2020, 10, 12 }, { 15, 12, 00 }, Qt::UTC ) );

		{
			m_dtSections.clear();

			QStyleOption opt;
			opt.initFrom( m_dt.data() );

			QtMWidgets::Section s1( QtMWidgets::Section::DaySection );

			int sw = s1.maxWidth( opt ) + 5 * 2 + 6;
			int w = sw;

			m_dtSections.append( sw / 2 );

			QtMWidgets::Section s2( QtMWidgets::Section::MonthSection );

			sw = s2.maxWidth( opt ) + 5 * 2 + 6;

			m_dtSections.append( w + sw / 2 );

			w += sw;

			QtMWidgets::Section s3( QtMWidgets::Section::YearSection2Digits );

			sw = s3.maxWidth( opt ) + 5 * 2 + 6;

			m_dtSections.append( w + sw / 2 );

			w += sw;

			QtMWidgets::Section s4( QtMWidgets::Section::Hour24Section );

			sw = s4.maxWidth( opt ) + 5 * 2 + 6;

			m_dtSections.append( w + sw / 2 );

			w += sw;

			QtMWidgets::Section s5( QtMWidgets::Section::MinuteSection );

			sw = s5.maxWidth( opt ) + 5 * 2 + 6;

			m_dtSections.append( w + sw / 2 );

			w += sw;

			QtMWidgets::Section s6( QtMWidgets::Section::SecondSection );

			sw = s6.maxWidth( opt ) + 5 * 2 + 6;

			m_dtSections.append( w + sw / 2 );
		}

		{
			QPoint p( m_dtSections.at( 2 ), m_dt->height() / 2 );
			QTest::mousePress( m_dt.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_dt.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_dt.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_dt->dateTime() == QDateTime( { 2021, 10, 12 }, { 15, 12 }, Qt::UTC ) );
		}

		{
			QPoint p( m_dtSections.at( 2 ), m_dt->height() / 2 );
			QTest::mousePress( m_dt.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p - m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_dt.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_dt.data(), Qt::LeftButton, {}, p - m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_dt->dateTime() == QDateTime( { 2020, 10, 12 }, { 15, 12 }, Qt::UTC ) );
		}

		{
			QPoint p( m_dtSections.at( 2 ), m_dt->height() / 2 );
			QTest::mousePress( m_dt.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p - m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_dt.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_dt.data(), Qt::LeftButton, {}, p - m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_dt->dateTime() == QDateTime( { 2019, 10, 12 }, { 15, 12 }, Qt::UTC ) );
		}

		{
			QPoint p( m_dtSections.at( 2 ), m_dt->height() / 2 );
			QTest::mousePress( m_dt.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p - m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_dt.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_dt.data(), Qt::LeftButton, {}, p - m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_dt->dateTime() == QDateTime( { 2019, 10, 12 }, { 15, 12 }, Qt::UTC ) );
		}

		m_dt->setDateTime( { { 2021, 12, 31 }, { 23, 59, 59, 999 }, Qt::UTC } );

		m_dt->setTimeRange( { 0, 0 }, { 22, 0 } );

		QVERIFY( m_dt->dateTime() == QDateTime( { 2021, 12, 31 }, { 22, 0 }, Qt::UTC ) );

		m_dt->clearMaximumTime();
		m_dt->clearMinimumTime();
		m_dt->clearMaximumDate();
		m_dt->clearMinimumDate();

		{
			QPoint p( m_dtSections.at( 4 ), m_dt->height() / 2 );
			QTest::mousePress( m_dt.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_dt.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_dt.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QVERIFY( m_dt->dateTime() == QDateTime( { 2021, 12, 31 }, { 22, 1 }, Qt::UTC ) );
		}

		m_dt->setMaximumDateTime( { { 2021, 12, 31 }, { 22, 0 }, Qt::UTC });

		QVERIFY( m_dt->dateTime() == QDateTime( { 2021, 12, 31 }, { 22, 0 }, Qt::UTC ) );

		m_dt->setMaximumDateTime( { { 2021, 12, 31 }, { 22, 2 }, Qt::UTC } );
		m_dt->setMinimumDateTime( { { 2021, 12, 31 }, { 22, 1 }, Qt::UTC } );

		QVERIFY( m_dt->dateTime() == QDateTime( { 2021, 12, 31 }, { 22, 1 }, Qt::UTC ) );

		m_dt->clearMaximumDateTime();
		m_dt->clearMinimumDateTime();

		{
			QPoint p( m_dtSections.at( 3 ), m_dt->height() / 2 );
			QTest::mousePress( m_dt.data(), Qt::LeftButton, {}, p, 20 );
			QMouseEvent me( QEvent::MouseMove, p + m_delta, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_dt.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_dt.data(), Qt::LeftButton, {}, p + m_delta, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_dt->dateTime() == QDateTime( { 2021, 12, 31 }, { 23, 1 }, Qt::UTC ) );
		}

		m_dt->setTimeSpec( Qt::LocalTime );
		m_dt->setDateRange( { 2020, 12, 31 }, { 2019, 12, 31 } );

		QVERIFY( m_dt->date() == QDate( 2020, 12, 31 ) );

		m_dt->hide();
	}

	void testCtors()
	{
		{
			QtMWidgets::DateTimePicker dt( QDateTime( { 2019, 12, 31 }, { 23, 59 } ) );
			QVERIFY( dt.dateTime() == QDateTime( { 2019, 12, 31 }, { 23, 59 } ) );
		}

		{
			QtMWidgets::DateTimePicker dt( QDate( 2019, 12, 31 ) );
			QVERIFY( dt.dateTime() == QDateTime( { 2019, 12, 31 }, { 0, 0 } ) );
		}

		{
			QtMWidgets::DateTimePicker dt( QTime( 23, 59 ) );
			QVERIFY( dt.dateTime() == QDateTime( { 2000, 1, 1 }, { 23, 59 } ) );
		}

		{
			QtMWidgets::DatePicker d( QDate( 2011, 10, 9 ) );
			QVERIFY( d.date() == QDate( 2011, 10, 9 ) );
		}

		{
			QtMWidgets::TimePicker t( QTime( 23, 59 ) );
			QVERIFY( t.time() == QTime( 23, 59 ) );
		}
	}

private:
	QSharedPointer< QtMWidgets::DateTimePicker > m_dt;
	QSharedPointer< QtMWidgets::DatePicker > m_d;
	QSharedPointer< QtMWidgets::TimePicker > m_t;
	QVector< int > m_dtSections;
	QVector< int > m_dSections;
	QVector< int > m_tSections;
	QFont m_font;
	QPoint m_delta;
	int m_height;
};


QTEST_MAIN( TestDateTime )

#include "main.moc"
