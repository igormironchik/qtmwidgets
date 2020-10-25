
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

// QtMWidgets include.
#include <QtMWidgets/BusyIndicator>


class TestBusy
	:	public QObject
{
	Q_OBJECT

private slots:

	void testBusy()
	{
		QtMWidgets::BusyIndicator i;
		i.setRadius( 35 );
		i.setRunning( false );

		i.show();

		QVERIFY( QTest::qWaitForWindowActive( &i ) );

		QVERIFY( i.radius() == 35 );

		i.setRunning( true );

		QVERIFY( i.isRunning() == true );

		QTest::qWait( 500 );

		i.setRunning( false );

		QVERIFY( i.isRunning() == false );

		i.setColor( Qt::red );

		QVERIFY( i.color() == Qt::red );
	}
};


QTEST_MAIN( TestBusy )

#include "main.moc"
