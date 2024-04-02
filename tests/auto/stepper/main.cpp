
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QObject>
#include <QtTest/QtTest>
#include <QSharedPointer>

// QtMWidgets include.
#include <QtMWidgets/Stepper>


class TestStepper
	:	public QObject
{
	Q_OBJECT

private slots:

	void initTestCase()
	{
		m_stepper.reset( new QtMWidgets::Stepper );

		m_stepper->show();

		QVERIFY( QTest::qWaitForWindowActive( m_stepper.data() ) );

		p1 = QPoint( m_stepper->width() / 4, m_stepper->height() / 2 );
		p2 = QPoint( m_stepper->width() - p1.x(), p1.y() );
	}

	void testStepper()
	{
		m_stepper->setMinimum( 0 );
		m_stepper->setMaximum( 5 );
		m_stepper->setSingleStep( 1 );
		m_stepper->setWrapping( false );
		m_stepper->setAutorepeat( false );

		QVERIFY( m_stepper->value() == 0 );
		QVERIFY( m_stepper->minimum() == 0 );
		QVERIFY( m_stepper->maximum() == 5 );
		QVERIFY( m_stepper->wrapping() == false );
		QVERIFY( m_stepper->autorepeat() == false );
		QVERIFY( m_stepper->singleStep() == 1 );

		QSignalSpy spy( m_stepper.data(), &QtMWidgets::Stepper::valueChanged );

		m_stepper->stepUp();

		QVERIFY( m_stepper->value() == 1 );
		QVERIFY( spy.count() == 1 );

		for( int i = 1; i < 5; ++i )
			m_stepper->stepUp();

		QVERIFY( m_stepper->value() == 5 );
		QVERIFY( spy.count() == 5 );

		m_stepper->stepUp();

		QVERIFY( m_stepper->value() == 5 );
		QVERIFY( spy.count() == 5 );

		m_stepper->stepDown();

		QVERIFY( m_stepper->value() == 4 );
		QVERIFY( spy.count() == 6 );

		m_stepper->setWrapping( true );

		for( int i = 0; i < 5; ++i )
			m_stepper->stepDown();

		QVERIFY( m_stepper->value() == 5 );
		QVERIFY( spy.count() == 11 );

		QTest::mouseClick( m_stepper.data(), Qt::LeftButton, {}, p2, 20 );

		QVERIFY( m_stepper->value() == 0 );
		QVERIFY( spy.count() == 12 );

		m_stepper->setWrapping( false );

		QTest::mouseClick( m_stepper.data(), Qt::LeftButton, {}, p1, 20 );

		QVERIFY( m_stepper->value() == 0 );
		QVERIFY( spy.count() == 12 );

		m_stepper->setAutorepeat( true );

		QVERIFY( m_stepper->autorepeat() == true );

		QTest::mousePress( m_stepper.data(), Qt::LeftButton, {}, p2, 20 );
		QTest::qWait( 500 * 5 );
		QTest::mouseRelease( m_stepper.data(), Qt::LeftButton, {}, p2, 20 );

		QVERIFY( m_stepper->value() == 5 );
		QVERIFY( spy.count() == 17 );

		m_stepper->setValue( 2 );

		QVERIFY( m_stepper->value() == 2 );
		QVERIFY( spy.count() == 18 );
	}

private:
	QSharedPointer< QtMWidgets::Stepper > m_stepper;
	QPoint p1;
	QPoint p2;
};


QTEST_MAIN( TestStepper )

#include "main.moc"
