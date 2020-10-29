
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
#include <QLayout>

// QtMWidgets include.
#include <QtMWidgets/TableView>
#include <QtMWidgets/TextLabel>
#include <QtMWidgets/Slider>
#include <QtMWidgets/Switch>


class TestTable
	:	public QObject
{
	Q_OBJECT

private slots:

	void initTestCase()
	{
		m_v.reset( new QtMWidgets::TableView );

		QtMWidgets::TableView * view = m_v.data();

		view->setHighlightCellOnClick( true );

		m_ringerAndAlerts = new QtMWidgets::TableViewSection( view );
		m_ringerAndAlerts->header()->setText(
			QLatin1String( "RINGER AND ALERTS" ) );
		m_ringerAndAlerts->footer()->setText(
			QLatin1String( "The m_volume of the ringer and alerts "
				"can be adjusted using the m_volume buttons." ) );

		m_volume = new QtMWidgets::TableViewCell( m_ringerAndAlerts );
		m_volume->layout()->setSpacing( 0 );
		QtMWidgets::Slider * m_volumeSlider = new QtMWidgets::Slider(
			Qt::Horizontal, m_volume );
		m_volume->setAccessoryWidget( m_volumeSlider );
		m_ringerAndAlerts->addCell( m_volume );

		QtMWidgets::TableViewCell * changeWithButtons =
			new QtMWidgets::TableViewCell( m_ringerAndAlerts );
		changeWithButtons->textLabel()->setText(
			QLatin1String( "Change with Buttons" ) );

		QtMWidgets::Switch * changeWithButtonsSwitch =
			new QtMWidgets::Switch( changeWithButtons );
		changeWithButtonsSwitch->setState(
			QtMWidgets::Switch::AcceptedCheck );

		changeWithButtons->setAccessoryWidget( changeWithButtonsSwitch );
		m_ringerAndAlerts->addCell( changeWithButtons );

		view->addSection( m_ringerAndAlerts );

		m_sounds = new QtMWidgets::TableViewSection( view );
		m_sounds->header()->setText( QLatin1String( "m_sounds" ) );

		QtMWidgets::TableViewCell * ringtone =
			new QtMWidgets::TableViewCell( m_sounds );
		ringtone->textLabel()->setText( QLatin1String( "Ringtone" ) );
		m_sounds->addCell( ringtone );

		QtMWidgets::TableViewCell * textTone =
			new QtMWidgets::TableViewCell( m_sounds );
		textTone->textLabel()->setText( QLatin1String( "Text Tone" ) );
		m_sounds->addCell( textTone );

		QtMWidgets::TableViewCell * newMail =
			new QtMWidgets::TableViewCell( m_sounds );
		newMail->textLabel()->setText( QLatin1String( "New Mail" ) );
		m_sounds->addCell( newMail );

		QtMWidgets::TableViewCell * sentMail =
			new QtMWidgets::TableViewCell( m_sounds );
		sentMail->textLabel()->setText( QLatin1String( "Sent Mail" ) );
		m_sounds->addCell( sentMail );

		QtMWidgets::TableViewCell * tweet =
			new QtMWidgets::TableViewCell( m_sounds );
		tweet->textLabel()->setText( QLatin1String( "Tweet" ) );
		m_sounds->addCell( tweet );

		m_facebookPost = new QtMWidgets::TableViewCell( m_sounds );
		m_facebookPost->textLabel()->setText( QLatin1String( "Facebook Post" ) );
		m_sounds->addCell( m_facebookPost );

		view->addSection( m_sounds );
	}

	void testTable()
	{
		m_v->resize( 200, 400 );
		m_v->show();

		QVERIFY( QTest::qWaitForWindowActive( m_v.data() ) );

		QVERIFY( m_v->sectionAt( 0 ) == m_ringerAndAlerts );
		QVERIFY( m_v->sectionAt( 1 ) == m_sounds );

		QVERIFY( m_v->highlightCellOnClick() == true );
		QVERIFY( m_ringerAndAlerts->highlightCellOnClick() == true );

		QVERIFY( m_ringerAndAlerts->cellsCount() == 2 );
		QVERIFY( m_ringerAndAlerts->cellAt( 0 ) == m_volume );
		QVERIFY( m_volume->highlightOnClick() == true );

		m_v->ensureWidgetVisible( m_facebookPost );

		QTest::qWait( 550 );

		const auto p = m_sounds->mapTo( m_v->viewport(), m_facebookPost->pos() );

		const QRect r = m_v->viewport()->rect();

		QVERIFY( r.contains( p ) );

		const auto cc = m_sounds->cellsCount();

		m_sounds->removeCell( m_facebookPost );

		m_facebookPost->deleteLater();

		QTest::qWait( 50 );

		QVERIFY( m_sounds->cellsCount() == cc - 1 );

		m_v->removeSection( m_sounds );

		m_sounds->deleteLater();

		QVERIFY( m_v->sectionsCount() == 1 );

		QTest::qWait( 50 );
	}

private:
	QSharedPointer< QtMWidgets::TableView > m_v;
	QtMWidgets::TableViewSection * m_ringerAndAlerts;
	QtMWidgets::TableViewSection * m_sounds;
	QtMWidgets::TableViewCell * m_volume;
	QtMWidgets::TableViewCell * m_facebookPost;
};


QTEST_MAIN( TestTable )

#include "main.moc"
