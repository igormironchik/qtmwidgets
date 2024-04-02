
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
