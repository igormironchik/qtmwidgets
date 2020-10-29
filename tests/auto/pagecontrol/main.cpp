
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
#include <QtMWidgets/PageControl>
#include <QtMWidgets/FingerGeometry>


class TestPageControl
	:	public QObject
{
	Q_OBJECT

private slots:

	void initTestCase()
	{
		m_btnSize = QtMWidgets::FingerGeometry::width() * 0.3 / 2 * 3;
	}

	void testBusy()
	{
		QtMWidgets::PageControl p;

		p.resize( m_btnSize * 3, m_btnSize * 4 );
		p.show();

		QVERIFY( QTest::qWaitForWindowActive( &p ) );

		int count = p.width() / m_btnSize;

		p.setPageIndicatorColor( Qt::red );
		p.setCurrentPageIndicatorColor( Qt::red );

		QVERIFY( p.pageIndicatorColor() == Qt::red );
		QVERIFY( p.currentPageIndicatorColor() == Qt::red );

		p.setCount( 10 );

		p.setCurrentIndex( 4 );

		QVERIFY( p.count() == 10 );
		QVERIFY( p.currentIndex() == 4 );

		QTest::mouseClick( &p, Qt::LeftButton, {},
			QRect( ( p.width() - m_btnSize * count ) / 2, 0, m_btnSize, m_btnSize ).center(),
			20 );

		QTest::qWait( 50 );

		QVERIFY( p.currentIndex() == 0 );

		p.resize( m_btnSize * 10, m_btnSize * 2 );

		QTest::qWait( 50 );

		count = p.width() / m_btnSize;

		QSignalSpy spy( &p, &QtMWidgets::PageControl::currentChanged );

		QTest::mouseClick( &p, Qt::LeftButton, {},
			QRect( ( p.width() - m_btnSize * count ) / 2 + m_btnSize * 4, 0,
				m_btnSize, m_btnSize ).center(), 20 );

		QTest::qWait( 50 );

		QVERIFY( p.currentIndex() == 4 );
		QVERIFY( spy.count() == 1 );
	}

private:
	int m_btnSize;
};


QTEST_MAIN( TestPageControl )

#include "main.moc"
