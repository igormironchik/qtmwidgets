
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

// QtMWidgets include.
#include <QtMWidgets/Picker>


class TestPicker
	:	public QObject
{
	Q_OBJECT

private slots:

	void testPicker()
	{
		QtMWidgets::Picker picker;
		picker.setMouseTracking( true );

		picker.addItem( QLatin1String( "English" ) );
		picker.addItem( QLatin1String( "Russian" ) );
		picker.addItem( QLatin1String( "German" ) );
		picker.addItem( QLatin1String( "Spanish" ) );
		picker.addItem( QLatin1String( "Portuguese" ) );
		picker.addItem( QLatin1String( "Belorussian" ) );
		picker.addItem( QLatin1String( "Polish" ) );
		picker.addItem( QLatin1String( "Ukrainian" ) );

		QFont font = picker.font();
		font.setBold( true );
		font.setPixelSize( 15 );

		picker.setFont( font );
		picker.setCurrentIndex( 1 );

		picker.show();

		QVERIFY( QTest::qWaitForWindowActive( &picker ) );

		picker.setCurrentIndex( 2 );

		QVERIFY( picker.currentIndex() == 2 );
		QVERIFY( picker.currentText() == QStringLiteral( "German" ) );

		QVERIFY( picker.count() == 8 );

		QVERIFY( picker.findText( QStringLiteral( "Polish" ) ) == 6 );
		QVERIFY( picker.itemText( 6 ) == QStringLiteral( "Polish" ) );

		QPoint c( picker.width() / 2, picker.height() / 2 );
		QFontMetrics fm( font );
		const auto h = fm.boundingRect( QLatin1String( "A" ) ).height();
		QPoint d = QPoint( 0, - ( h + h / 3 ) );

		QTest::mouseMove( &picker, c );
		QTest::mousePress( &picker, Qt::LeftButton, {}, c );
		QTest::mouseMove( &picker, c + d, 20 );
		QTest::mouseRelease( &picker, Qt::LeftButton, {}, c + d, 20 );
		QTest::mouseMove( &picker, c, 20 );
		QTest::mouseClick( &picker, Qt::LeftButton, {}, c, 20 );

		QVERIFY( picker.currentIndex() == 3 );

		for( int i = 0; i < 8; ++i )
		{
			QTest::mouseMove( &picker, c );
			QTest::mousePress( &picker, Qt::LeftButton, {}, c );
			QTest::mouseMove( &picker, c + d, 20 );
			QTest::mouseRelease( &picker, Qt::LeftButton, {}, c + d, 20 );
			QTest::mouseMove( &picker, c, 20 );
			QTest::mouseClick( &picker, Qt::LeftButton, {}, c, 20 );
		}

		QVERIFY( picker.currentIndex() == 3 );
	}
};


QTEST_MAIN( TestPicker )

#include "main.moc"
