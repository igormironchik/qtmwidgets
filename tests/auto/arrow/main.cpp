
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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

		QTest::qWait( 1000 );

		a.setDirection( QtMWidgets::NavigationArrow::Bottom );

		QVERIFY( a.direction() == QtMWidgets::NavigationArrow::Bottom );

		a.animate();

		QTest::qWait( 1000 );

		a.setDirection( QtMWidgets::NavigationArrow::Top );

		QVERIFY( a.direction() == QtMWidgets::NavigationArrow::Top );

		a.animate();

		QTest::qWait( 1000 );

		a.setColor( Qt::red );

		QVERIFY( a.color() == Qt::red );

		a.animate();

		QTest::qWait( 1000 );

		QTest::mouseClick( &a, Qt::RightButton, {}, a.rect().center(), 20 );

		QVERIFY( spy.count() == 1 );
	}
};


QTEST_MAIN( TestArrow )

#include "main.moc"
