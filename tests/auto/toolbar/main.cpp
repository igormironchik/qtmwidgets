
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
#include <QtMWidgets/ToolBar>


class Object
	:	public QObject
{
	Q_OBJECT

public:
	Object( int & count )
		:	m_count( count )
	{
	}

public slots:
	void slot1()
	{
		++m_count;
	}

	void slot2()
	{
		++m_count;
	}

private:
	int & m_count;
};


class TestToolBar
	:	public QObject
{
	Q_OBJECT

private slots:

	void testBar()
	{
		int count = 0;

		QWidget w;
		auto * l = new QVBoxLayout( &w );
		QtMWidgets::ToolBar b;
		l->addWidget( &b );
		Object o ( count );

		const auto size = 48;

		b.setOrientation( Qt::Horizontal );
		b.setIconSize( { size, size } );
		b.setAlignment( Qt::AlignLeft );

		w.resize( size * 7, size * 3 );
		w.show();

		QVERIFY( QTest::qWaitForWindowActive( &w ) );

		QVERIFY( b.orientation() == Qt::Horizontal );
		QVERIFY( b.iconSize() == QSize( 48, 48 ) );
		QVERIFY( b.alignment() == Qt::AlignLeft );

		auto * a1 = b.addAction( QIcon( QStringLiteral( ":/configure.png" ) ) );
		auto * a2 = b.addAction( QIcon( QStringLiteral( ":/configure.png" ) ),
			&o, SLOT( slot1() ) );
		auto * a3 = b.addAction( QIcon( QStringLiteral( ":/configure.png" ) ),
			&o, &Object::slot2 );
		auto * a4 = b.addAction( QIcon( QStringLiteral( ":/configure.png" ) ),
			[&count] () { ++count; } );

		const auto r1 = b.actionGeometry( a1 );
		const auto r2 = b.actionGeometry( a2 );
		const auto r3 = b.actionGeometry( a3 );
		const auto r4 = b.actionGeometry( a4 );

		QVERIFY( b.actionAt( r1.center() ) == a1 );
		QVERIFY( b.actionAt( r2.center() ) == a2 );
		QVERIFY( b.actionAt( r3.center() ) == a3 );
		QVERIFY( b.actionAt( r4.center() ) == a4 );

		b.setOrientation( Qt::Vertical );
		b.setAlignment( Qt::AlignTop );
		w.resize( size * 3, size * 7 );

		QVERIFY( b.orientation() == Qt::Vertical );
		QVERIFY( b.alignment() == Qt::AlignTop );

		const auto r1v = b.actionGeometry( a1 );
		const auto r2v = b.actionGeometry( a2 );
		const auto r3v = b.actionGeometry( a3 );
		const auto r4v = b.actionGeometry( a4 );

		QVERIFY( r2 != r2v );
		QVERIFY( r3 != r3v );
		QVERIFY( r4 != r4v );

		b.clear();

		QTest::mouseClick( &b, Qt::LeftButton, {}, r1v.center(), 20 );
	}
};


QTEST_MAIN( TestToolBar )

#include "main.moc"
