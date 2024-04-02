
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QObject>
#include <QtTest/QtTest>
#include <QSharedPointer>

// QtMWidgets include.
#include <QtMWidgets/Switch>


class TestSwitch
	:	public QObject
{
	Q_OBJECT

private slots:

	void initTestCase()
	{
		m_switch.reset( new QtMWidgets::Switch );

		m_font = m_switch->font();
		m_font.setBold( true );
		m_switch->setFont( m_font );

		m_switch->setOnText( QStringLiteral( "ON" ) );
		m_switch->setOffText( QStringLiteral( "OFF" ) );

		m_switch->show();

		QVERIFY( QTest::qWaitForWindowActive( m_switch.data() ) );

		m_delta = QPoint( m_switch->height() / 2, m_switch->height() / 2 );
	}

	void testSwitch()
	{
		QVERIFY( m_switch->state() == QtMWidgets::Switch::AcceptedUncheck );
		QVERIFY( m_switch->isChecked() == false );
		QVERIFY( m_switch->onText() == QStringLiteral( "ON" ) );
		QVERIFY( m_switch->offText() == QStringLiteral( "OFF" ) );

		QSignalSpy spy( m_switch.data(), &QtMWidgets::Switch::stateChanged );

		QTest::mouseClick( m_switch.data(), Qt::LeftButton, {}, m_delta, 20 );

		QVERIFY( spy.count() == 1 );

		QVERIFY( m_switch->state() == QtMWidgets::Switch::NotAcceptedCheck );
		QVERIFY( m_switch->isChecked() == true );

		m_switch->setState( QtMWidgets::Switch::AcceptedCheck );

		QVERIFY( m_switch->state() == QtMWidgets::Switch::AcceptedCheck );
		QVERIFY( m_switch->isChecked() == true );

		const auto w = m_switch->width();
		const auto m1 = qRound( (double) ( w - m_delta.x() * 2 + 2 ) / 2.5 );
		QTest::mousePress( m_switch.data(), Qt::LeftButton, {},
			QPoint( w - m_delta.x(), m_delta.y() ), 20 );
		QMouseEvent me1( QEvent::MouseMove, QPoint( m1, m_delta.y() ),
			m_switch->mapToGlobal( QPoint( m1, m_delta.y() ) ),
			Qt::LeftButton, Qt::LeftButton, {} );
		QApplication::sendEvent( m_switch.data(), &me1 );
		QTest::mouseRelease( m_switch.data(), Qt::LeftButton, {},
			QPoint( m1, m_delta.y() ), 20 );

		QVERIFY( spy.count() == 3 );

		QVERIFY( m_switch->state() == QtMWidgets::Switch::NotAcceptedUncheck );
		QVERIFY( m_switch->isChecked() == false );

		m_switch->setState( QtMWidgets::Switch::AcceptedUncheck );

		QVERIFY( m_switch->state() == QtMWidgets::Switch::AcceptedUncheck );
		QVERIFY( m_switch->isChecked() == false );

		const auto m2 = w - m_delta.x() * 2;
		QTest::mousePress( m_switch.data(), Qt::LeftButton, {},
			QPoint( m_delta.x(), m_delta.y() ), 20 );
		QMouseEvent me2( QEvent::MouseMove, QPoint( m2, m_delta.y() ),
			m_switch->mapToGlobal( QPoint( m2, m_delta.y() ) ),
			Qt::LeftButton, Qt::LeftButton, {} );
		QApplication::sendEvent( m_switch.data(), &me2 );
		QTest::mouseRelease( m_switch.data(), Qt::LeftButton, {},
			QPoint( m2, m_delta.y() ), 20 );

		QVERIFY( spy.count() == 5 );

		QVERIFY( m_switch->state() == QtMWidgets::Switch::NotAcceptedCheck );
		QVERIFY( m_switch->isChecked() == true );

		m_switch->setState( QtMWidgets::Switch::AcceptedCheck );

		QVERIFY( m_switch->state() == QtMWidgets::Switch::AcceptedCheck );
		QVERIFY( m_switch->isChecked() == true );

		QTest::mouseClick( m_switch.data(), Qt::LeftButton, {}, m_delta, 20 );

		QVERIFY( spy.count() == 7 );

		QVERIFY( m_switch->state() == QtMWidgets::Switch::NotAcceptedUncheck );
		QVERIFY( m_switch->isChecked() == false );
	}

private:
	QSharedPointer< QtMWidgets::Switch > m_switch;
	QFont m_font;
	QPoint m_delta;
};


QTEST_MAIN( TestSwitch )

#include "main.moc"
