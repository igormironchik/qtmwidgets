
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
#include <QTimer>
#include <QAbstractButton>

// QtMWidgets include.
#include <QtMWidgets/MessageBox>


class TestMessageBox
	:	public QObject
{
	Q_OBJECT

private slots:

	void testMessageBox()
	{
		m_box.reset( new QtMWidgets::MessageBox( QStringLiteral( "MessageBox" ),
			QStringLiteral( "Text." ) ) );

		m_box->setTextFormat( Qt::PlainText );

		QVERIFY( m_box->textFormat() == Qt::PlainText );

		auto * ok = m_box->addButton( QStringLiteral( "OK 2" ), QtMWidgets::MessageBox::AcceptRole );

		const auto accept = m_box->buttons( QtMWidgets::MessageBox::AcceptRole );

		QVERIFY( accept.size() == 2 );

		QVERIFY( m_box->buttonRole( ok ) == QtMWidgets::MessageBox::AcceptRole );

		auto * cancel = m_box->addButton( QStringLiteral( "Cancel" ),
			QtMWidgets::MessageBox::RejectRole );

		auto reject = m_box->buttons( QtMWidgets::MessageBox::RejectRole );

		QVERIFY( reject.size() == 1 );
		QVERIFY( reject.constFirst() == cancel );

		QVERIFY( m_box->buttonRole( cancel ) == QtMWidgets::MessageBox::RejectRole );

		QVERIFY( m_box->buttons().size() == 3 );

		m_box->removeButton( cancel );

		reject = m_box->buttons( QtMWidgets::MessageBox::RejectRole );

		QVERIFY( reject.isEmpty() );

		QVERIFY( m_box->text() == QStringLiteral( "Text." ) );

		const auto text = QStringLiteral( "Text in the message box." );

		m_box->setText( text );

		QVERIFY( m_box->text() == text );

		QSignalSpy spy( m_box.data(), &QtMWidgets::MessageBox::buttonClicked );

		connect( m_box.data(), &QtMWidgets::MessageBox::buttonClicked, this,
			[&] ( QAbstractButton * b )
			{
				QVERIFY( b == ok );
			} );

		QTimer::singleShot( 1000, this,
			[&] ()
			{
				QTest::mouseClick( ok, Qt::LeftButton, {}, ok->rect().center() );
			} );

		const auto ret = m_box->exec();

		QVERIFY( ret == QDialog::Accepted );

		QVERIFY( spy.count() == 1 );

		QVERIFY( m_box->clickedButton() == ok );
	}

private:
	QSharedPointer< QtMWidgets::MessageBox > m_box;
};


QTEST_MAIN( TestMessageBox )

#include "main.moc"
