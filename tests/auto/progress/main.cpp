
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QObject>
#include <QtTest/QtTest>
#include <QSharedPointer>
#include <QVBoxLayout>

// QtMWidgets include.
#include <QtMWidgets/ProgressBar>


class TestProgress
	:	public QObject
{
	Q_OBJECT

private slots:

	void testHorBar()
	{
		QWidget w;
		QVBoxLayout l;
		w.setLayout( &l );
		QtMWidgets::ProgressBar p;
		l.addWidget( &p );

		w.show();

		QVERIFY( QTest::qWaitForWindowActive( &w ) );

		QSignalSpy spy( &p, &QtMWidgets::ProgressBar::valueChanged );

		p.setMinimum( 0 );
		p.setMaximum( 100 );
		p.setValue( 1 );
		p.setOrientation( Qt::Horizontal );

		w.resize( 150, 50 );

		QVERIFY( p.minimum() == 0 );
		QVERIFY( p.maximum() == 100 );
		QVERIFY( p.value() == 1 );
		QVERIFY( p.orientation() == Qt::Horizontal );
		QVERIFY( p.invertedAppearance() == false );
		QVERIFY( spy.count() == 1 );

		for( int i = p.minimum(); i <= p.maximum(); i += 10 )
		{
			p.setValue( i );
			QVERIFY( p.value() == i );
			QTest::qWait( 50 );
		}

		p.reset();
		QTest::qWait( 550 );

		p.setInvertedAppearance( true );

		QVERIFY( p.invertedAppearance() == true );

		for( int i = p.minimum(); i <= p.maximum(); i += 10 )
		{
			p.setValue( i );
			QVERIFY( p.value() == i );
			QTest::qWait( 50 );
		}
	}

	void testVertBar()
	{
		QWidget w;
		QVBoxLayout l;
		w.setLayout( &l );
		QtMWidgets::ProgressBar p;
		l.addWidget( &p );

		w.show();

		QVERIFY( QTest::qWaitForWindowActive( &w ) );

		QSignalSpy spy( &p, &QtMWidgets::ProgressBar::valueChanged );

		p.setMinimum( 0 );
		p.setMaximum( 100 );
		p.setValue( 1 );
		p.setOrientation( Qt::Vertical );

		w.resize( 50, 150 );

		QVERIFY( p.minimum() == 0 );
		QVERIFY( p.maximum() == 100 );
		QVERIFY( p.value() == 1 );
		QVERIFY( p.orientation() == Qt::Vertical );
		QVERIFY( p.invertedAppearance() == false );
		QVERIFY( spy.count() == 1 );

		for( int i = p.minimum(); i <= p.maximum(); i += 10 )
		{
			p.setValue( i );
			QVERIFY( p.value() == i );
			QTest::qWait( 50 );
		}

		p.reset();
		QTest::qWait( 550 );

		p.setInvertedAppearance( true );

		QVERIFY( p.invertedAppearance() == true );

		for( int i = p.minimum(); i <= p.maximum(); i += 10 )
		{
			p.setValue( i );
			QVERIFY( p.value() == i );
			QTest::qWait( 50 );
		}
	}

	void testStuff()
	{
		QtMWidgets::ProgressBar p;

		p.setGrooveColor( Qt::red );
		p.setGrooveHeight( 10 );
		p.setHighlightColor( Qt::green );

		QVERIFY( p.grooveColor() == Qt::red );
		QVERIFY( p.grooveHeight() == 10 );
		QVERIFY( p.highlightColor() == Qt::green );
	}
};


QTEST_MAIN( TestProgress )

#include "main.moc"
