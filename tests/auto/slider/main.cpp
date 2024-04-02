
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QObject>
#include <QtTest/QtTest>
#include <QSharedPointer>

// QtMWidgets include.
#include <QtMWidgets/Slider>


class TestSlider
	:	public QObject
{
	Q_OBJECT

private slots:

	void testVerticalSlider()
	{
		QtMWidgets::Slider s;

		s.resize( s.handleRadius() * 3, s.handleRadius() * 20 );
		s.show();

		QVERIFY( QTest::qWaitForWindowActive( &s ) );

		QVERIFY( s.orientation() == Qt::Vertical );

		int v = 0;

		s.setRange( 0, 100 );
		s.setValue( v );

		QRect r( s.width() / 2, s.handleRadius() * 2, s.handleRadius() * 2, s.handleRadius() * 2 );

		QTest::mouseClick( &s, Qt::LeftButton, {}, r.center(), 20 );

		QVERIFY( s.value() > v );

		v = s.value();

		QTest::mousePress( &s, Qt::LeftButton, {}, r.center(), 20 );
		QMouseEvent me( QEvent::MouseMove, r.center() + QPoint( 0, s.handleRadius() * 2 ),
			s.mapToGlobal( r.center() + QPoint( 0, s.handleRadius() * 2 ) ),
			Qt::LeftButton, Qt::LeftButton, {} );
		QApplication::sendEvent( &s, &me );
		QTest::mouseRelease( &s, Qt::LeftButton, {},
			r.center() + QPoint( 0, s.handleRadius() * 2 ), 20 );

		QVERIFY( s.value() > v );

		v = s.value();

		QTest::mouseClick( &s, Qt::LeftButton, {}, r.center(), 20 );

		QVERIFY( s.value() < v );

		v = s.value();

		s.setInvertedAppearance( true );

		QTest::mouseClick( &s, Qt::LeftButton, {}, r.center(), 20 );

		QVERIFY( s.value() > v );
	}

	void testCtors()
	{
		QtMWidgets::Slider s( Qt::Horizontal );

		QVERIFY( s.orientation() == Qt::Horizontal );

		s.setHandleRadius( 20 );

		QVERIFY( s.handleRadius() == 20 );

		s.setGrooveHeight( 5 );

		QVERIFY( s.grooveHeight() == 5 );

		s.setHighlightColor( Qt::red );

		QVERIFY( s.highlightColor() == Qt::red );
	}

	void testHorizontalSlider()
	{
		QtMWidgets::Slider s( Qt::Horizontal );

		s.resize( s.handleRadius() * 20, s.handleRadius() * 3 );
		s.show();

		QVERIFY( QTest::qWaitForWindowActive( &s ) );

		QVERIFY( s.orientation() == Qt::Horizontal );

		int v = 0;

		s.setRange( 0, 100 );
		s.setValue( v );

		QRect r( s.handleRadius() * 2, s.height() / 2, s.handleRadius() * 2, s.handleRadius() * 2 );

		QTest::mouseClick( &s, Qt::LeftButton, {}, r.center(), 20 );

		QVERIFY( s.value() > v );

		v = s.value();

		QTest::mousePress( &s, Qt::LeftButton, {}, r.center(), 20 );
		QMouseEvent me( QEvent::MouseMove, r.center() + QPoint( s.handleRadius() * 2, 0 ),
			s.mapToGlobal( r.center() + QPoint( s.handleRadius() * 2, 0 ) ),
			Qt::LeftButton, Qt::LeftButton, {} );
		QApplication::sendEvent( &s, &me );
		QTest::mouseRelease( &s, Qt::LeftButton, {},
			r.center() + QPoint( s.handleRadius() * 2, 0 ), 20 );

		QVERIFY( s.value() > v );

		v = s.value();

		QTest::mouseClick( &s, Qt::LeftButton, {}, r.center(), 20 );

		QVERIFY( s.value() < v );

		v = s.value();

		s.setInvertedAppearance( true );

		QTest::mouseClick( &s, Qt::LeftButton, {}, r.center(), 20 );

		QVERIFY( s.value() > v );
	}
};


QTEST_MAIN( TestSlider )

#include "main.moc"
