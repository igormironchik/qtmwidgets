
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
