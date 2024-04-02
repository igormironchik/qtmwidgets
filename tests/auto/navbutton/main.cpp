
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QObject>
#include <QtTest/QtTest>
#include <QSharedPointer>

// QtMWidgets include.
#include <QtMWidgets/NavigationButton>


class TestNavButton
	:	public QObject
{
	Q_OBJECT

private slots:

	void testButton()
	{
		const auto text = QStringLiteral( "Button very very very very long" );

		QtMWidgets::NavigationButton b( QtMWidgets::NavigationButton::Right, text );
		b.setArrowColor( Qt::red );
		b.setTextColor( Qt::green );
		b.resize( 75, 25 );

		b.show();

		QVERIFY( QTest::qWaitForWindowActive( &b ) );

		QVERIFY( b.arrowColor() == Qt::red );
		QVERIFY( b.textColor() == Qt::green );
		QVERIFY( b.direction() == QtMWidgets::NavigationButton::Right );
		QVERIFY( b.text() == text );

		QSignalSpy spy( &b, &QtMWidgets::NavigationButton::clicked );

		QTest::mouseClick( &b, Qt::LeftButton, {}, b.rect().center(), 20 );

		QVERIFY( spy.count() == 1 );

		b.setDirection( QtMWidgets::NavigationButton::Left );

		QVERIFY( b.direction() == QtMWidgets::NavigationButton::Left );

		QTest::qWait( 500 );

		QtMWidgets::NavigationButton tmp( QtMWidgets::NavigationButton::Left );

		QVERIFY( tmp.direction() == QtMWidgets::NavigationButton::Left );
	}
};


QTEST_MAIN( TestNavButton )

#include "main.moc"
