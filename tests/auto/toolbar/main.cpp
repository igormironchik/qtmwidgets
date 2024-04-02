
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
