
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
