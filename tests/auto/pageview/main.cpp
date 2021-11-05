
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
#include <QLabel>

// QtMWidgets include.
#include <QtMWidgets/PageView>


class TestPageView
	:	public QObject
{
	Q_OBJECT

private slots:

	void testView()
	{
		QtMWidgets::PageView p;
		p.show();

		QVERIFY( QTest::qWaitForWindowActive( &p ) );

		QVERIFY( p.currentIndex() == -1 );
		QVERIFY( p.count() == 0 );
		QVERIFY( p.showPageControl() == true );

		p.setShowPageControl( false );

		QVERIFY( p.showPageControl() == false );

		p.resize( 300, 300 );

		QLabel w1( QStringLiteral( "1" ) );

		auto f = w1.font();
		f.setPixelSize( 100 );
		f.setBold( true );

		w1.setFont( f );
		w1.setAlignment( Qt::AlignCenter );

		p.addWidget( &w1 );

		QVERIFY( p.count() == 1 );
		QVERIFY( p.currentIndex() == 0 );
		QVERIFY( p.currentWidget() == &w1 );
		QVERIFY( p.indexOf( &w1 ) == 0 );
		QVERIFY( p.widget( 0 ) == &w1 );

		QLabel w2( QStringLiteral( "2" ) );
		w2.setFont( f );
		w2.setAlignment( Qt::AlignCenter );

		p.addWidget( &w2 );

		p.setCurrentWidget( &w2 );

		QVERIFY( p.currentIndex() == 1 );

		QLabel w3( QStringLiteral( "3" ) );
		w3.setFont( f );
		w3.setAlignment( Qt::AlignCenter );

		QTest::qWait( 350 );

		p.addWidget( &w3 );

		{
			const auto c = p.rect().center() - QPoint( p.rect().width() / 3, 0 );
			QTest::mousePress( &p, Qt::LeftButton, {}, c, 20 );
			const auto pos = c -
				QPoint( p.rect().width() - p.rect().width() / 4, 0 );
			QMouseEvent me( QEvent::MouseMove, pos, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( &p, &me );
			QTest::mouseRelease( &p, Qt::LeftButton, {}, pos, 20 );
		}

		QTest::qWait( 1000 );

		QVERIFY( p.currentWidget() == &w3 );

		{
			const auto c = p.rect().center() - QPoint( p.rect().width() / 3, 0 );
			QTest::mousePress( &p, Qt::LeftButton, {}, c, 20 );
			const auto pos = c +
				QPoint( p.rect().width() - p.rect().width() / 4, 0 );
			QMouseEvent me( QEvent::MouseMove, pos, Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( &p, &me );
			QTest::mouseRelease( &p, Qt::LeftButton, {}, pos, 20 );
		}

		QTest::qWait( 1000 );

		QVERIFY( p.currentIndex() == 1 );

		p.removeWidget( &w3 );

		QVERIFY( p.count() == 2 );

		QVERIFY( p.widget( 1 ) == &w2 );
	}
};


QTEST_MAIN( TestPageView )

#include "main.moc"
