
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
#include <QtMWidgets/NavigationArrow>


class TestArrow
	:	public QObject
{
	Q_OBJECT

private slots:

	void testArrow()
	{
		QtMWidgets::NavigationArrow a( QtMWidgets::NavigationArrow::Left );

		a.show();

		QVERIFY( QTest::qWaitForWindowActive( &a ) );

		QVERIFY( a.direction() == QtMWidgets::NavigationArrow::Left );

		QSignalSpy spy( &a, &QtMWidgets::NavigationArrow::clicked );

		QTest::mouseClick( &a, Qt::LeftButton, {}, a.rect().center(), 20 );

		QVERIFY( spy.count() == 1 );

		a.setDirection( QtMWidgets::NavigationArrow::Right );

		QVERIFY( a.direction() == QtMWidgets::NavigationArrow::Right );

		a.animate();

		QTest::qWait( 520 );

		a.setDirection( QtMWidgets::NavigationArrow::Bottom );

		QVERIFY( a.direction() == QtMWidgets::NavigationArrow::Bottom );

		a.animate();

		QTest::qWait( 520 );

		a.setDirection( QtMWidgets::NavigationArrow::Top );

		QVERIFY( a.direction() == QtMWidgets::NavigationArrow::Top );

		a.animate();

		QTest::qWait( 520 );

		a.setColor( Qt::red );

		QVERIFY( a.color() == Qt::red );

		a.animate();

		QTest::qWait( 520 );

		QTest::mouseClick( &a, Qt::RightButton, {}, a.rect().center(), 20 );

		QVERIFY( spy.count() == 1 );
	}
};


QTEST_MAIN( TestArrow )

#include "main.moc"
