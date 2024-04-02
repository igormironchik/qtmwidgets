
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QObject>
#include <QtTest/QtTest>
#include <QSharedPointer>

// QtMWidgets include.
#include <QtMWidgets/NavigationBar>


class TestNavBar
	:	public QObject
{
	Q_OBJECT

private slots:

	void testBar()
	{
		QtMWidgets::NavigationBar b;

		b.show();

		QVERIFY( QTest::qWaitForWindowActive( &b ) );

		QWidget w1;

		const auto w1i = b.setMainWidget( QStringLiteral( "W1" ), &w1 );

		QWidget w2;

		const auto w2i = b.addWidget( &w1, QStringLiteral( "W2" ), &w2 );

		QVERIFY( b.currentIndex() == w1i );
		QVERIFY( b.currentWidget() == &w1 );
		QVERIFY( b.indexOf( &w1 ) == w1i );
		QVERIFY( b.widget( w1i ) == &w1 );

		QWidget w3;

		const auto w3i = b.addWidget( &w2, QStringLiteral( "W3" ), &w3 );

		b.showScreen( &w2 );

		QVERIFY( b.currentIndex() == w2i );

		b.showScreen( &w3 );

		QVERIFY( b.currentIndex() == w3i );

		b.showPreviousScreen();

		QVERIFY( b.currentIndex() == w2i );

		b.showPreviousScreen();

		QVERIFY( b.currentIndex() == w1i );

		b.showNextScreen();

		QVERIFY( b.currentIndex() == w2i );

		b.showNextScreen();

		QVERIFY( b.currentIndex() == w3i );

		b.showScreen( &w1 );

		QVERIFY( b.currentIndex() == w1i );

		b.showPreviousScreen();

		QVERIFY( b.currentIndex() == w3i );

		b.removeWidget( &w2 );

		QVERIFY( b.currentIndex() == w1i );
	}
};


QTEST_MAIN( TestNavBar )

#include "main.moc"
