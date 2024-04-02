
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
