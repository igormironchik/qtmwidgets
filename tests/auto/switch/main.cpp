
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

		QTest::mousePress( m_switch.data(), Qt::LeftButton, {},
			QPoint( w - m_delta.x(), m_delta.y() ), 20 );
		QMouseEvent me( QEvent::MouseMove, QPoint( w - m_delta.x() * 3, m_delta.y() ),
			Qt::LeftButton, Qt::LeftButton, {} );
		QApplication::sendEvent( m_switch.data(), &me );
		QTest::mouseRelease( m_switch.data(), Qt::LeftButton, {},
			QPoint( w - m_delta.x() * 3, m_delta.y() ), 20 );

		QVERIFY( spy.count() == 3 );

		QVERIFY( m_switch->state() == QtMWidgets::Switch::NotAcceptedUncheck );
		QVERIFY( m_switch->isChecked() == false );

		m_switch->setState( QtMWidgets::Switch::AcceptedUncheck );

		QVERIFY( m_switch->state() == QtMWidgets::Switch::AcceptedUncheck );
		QVERIFY( m_switch->isChecked() == false );
	}

private:
	QSharedPointer< QtMWidgets::Switch > m_switch;
	QFont m_font;
	QPoint m_delta;
};


QTEST_MAIN( TestSwitch )

#include "main.moc"
