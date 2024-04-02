
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QObject>
#include <QtTest/QtTest>
#include <QSharedPointer>
#include <QStringListModel>
#include <QtGlobal>

// QtMWidgets include.
#include <QtMWidgets/Picker>

class StringListEvenDisabledModel
	:	public QStringListModel
{
public:
	explicit StringListEvenDisabledModel( QObject * parent = nullptr )
		:	QStringListModel( parent )
	{
	}

protected:
	Qt::ItemFlags flags( const QModelIndex & index ) const override
	{
		if( index.row() % 2 )
			return QStringListModel::flags( index );
		else
			return Qt::NoItemFlags;
	}
};


class TestPicker
	:	public QObject
{
	Q_OBJECT

private slots:

	void initTestCase()
	{
		m_data.append( QStringLiteral( "English" ) );
		m_data.append( QStringLiteral( "Russian" ) );
		m_data.append( QStringLiteral( "German" ) );
		m_data.append( QStringLiteral( "Spanish" ) );
		m_data.append( QStringLiteral( "Portuguese" ) );
		m_data.append( QStringLiteral( "Belorussian" ) );
		m_data.append( QStringLiteral( "Polish" ) );
		m_data.append( QStringLiteral( "Ukrainian" ) );

		m_model.setStringList( m_data );

		m_picker.reset( new QtMWidgets::Picker );

		m_picker->setMouseTracking( true );

		for( int i = 0; i < m_data.size(); ++i )
			m_picker->addItem( m_data.at( i ) );

		m_font = m_picker->font();
		m_font.setBold( true );
		m_font.setPixelSize( 15 );

		m_picker->setFont( m_font );

		m_picker->show();

		QVERIFY( QTest::qWaitForWindowActive( m_picker.data() ) );

		QFontMetrics fm( m_font );
		m_lineHeight = fm.boundingRect( QStringLiteral( "A" ) ).height();

		m_delta = QPoint( 0, - ( m_lineHeight + m_lineHeight / 3 ) );
	}

	void testPicker()
	{
		m_picker->setCurrentIndex( 2 );

		QVERIFY( m_picker->currentIndex() == 2 );
		QVERIFY( m_picker->currentText() == m_data.at( 2 ) );

		QVERIFY( m_picker->count() == 8 );

		QVERIFY( m_picker->findText( m_data.at( 6 ) ) == 6 );
		QVERIFY( m_picker->itemText( 6 ) == m_data.at( 6 ) );

		QPoint c( m_picker->width() / 2, m_picker->height() / 2 );

		QTest::mousePress( m_picker.data(), Qt::LeftButton, {}, c );
		QMouseEvent me( QEvent::MouseMove, c + m_delta, m_picker->mapToGlobal( c + m_delta ),
			Qt::LeftButton, Qt::LeftButton, {} );
		QApplication::sendEvent( m_picker.data(), &me );
		QTest::qWait( 500 );
		QTest::mouseRelease( m_picker.data(), Qt::LeftButton, {}, c + m_delta, 20 );
		QTest::mouseClick( m_picker.data(), Qt::LeftButton, {}, c, 20 );

		QTest::qWait( 500 );

		QVERIFY( m_picker->currentIndex() == 3 );

		int idx = 4;

		for( int i = 0; i < 8; ++i )
		{
			QTest::mousePress( m_picker.data(), Qt::LeftButton, {}, c );
			QMouseEvent me( QEvent::MouseMove, c + m_delta, m_picker->mapToGlobal( c + m_delta ),
				Qt::LeftButton, Qt::LeftButton, {} );
			QApplication::sendEvent( m_picker.data(), &me );
			QTest::qWait( 500 );
			QTest::mouseRelease( m_picker.data(), Qt::LeftButton, {}, c + m_delta, 20 );
			QTest::mouseClick( m_picker.data(), Qt::LeftButton, {}, c, 20 );

			QTest::qWait( 500 );

			QVERIFY( m_picker->currentIndex() == idx );

			++idx;

			if( idx == m_data.size() )
				idx = 0;
		}

		QVERIFY( m_picker->currentIndex() == 3 );
	}

	void testWithModel()
	{
		m_picker->setModel( &m_model );
		m_picker->setCurrentIndex( 0 );

		QVERIFY( m_picker->currentText() == m_data.at( 0 ) );
		QVERIFY( m_picker->currentData( Qt::DisplayRole ).toString() == m_data.at( 0 ) );
		QVERIFY( m_picker->findData( m_data.at( 0 ), Qt::DisplayRole ) == 0 );
		QVERIFY( m_picker->itemData( 0, Qt::DisplayRole ).toString() == m_data.at( 0 ) );

		const auto wrong = QStringLiteral( "Wrong" );

		m_picker->setItemText( 0, wrong );

		QVERIFY( m_picker->currentText() == wrong );

		m_picker->setItemData( 0, m_data.at( 0 ), Qt::DisplayRole );

		QVERIFY( m_picker->currentText() == m_data.at( 0 ) );

		QSignalSpy spy( m_picker.data(), &QtMWidgets::Picker::currentTextChanged );

		m_picker->removeItem( 0 );

		QVERIFY( spy.count() == 1 );

		QVERIFY( m_picker->count() == m_data.size() - 1 );
		QVERIFY( m_picker->currentIndex() == 0 );
		QVERIFY( m_picker->currentText() == m_data.at( 1 ) );

		m_picker->setCurrentText( m_data.at( 6 ) );

		QVERIFY( m_picker->currentIndex() == 5 );
		QVERIFY( m_picker->currentText() == m_data.at( 6 ) );

		QVERIFY( m_picker->rootModelIndex() == QModelIndex() );
		QVERIFY( m_picker->modelColumn() == 0 );
		QVERIFY( m_picker->model() == &m_model );

		m_picker->insertItem( 0, m_data.at( 0 ) );

		QVERIFY( m_picker->count() == m_data.size() );
		QVERIFY( m_picker->itemText( 0 ) == m_data.at( 0 ) );

		m_picker->addItems( m_data );

		QVERIFY( m_picker->count() == m_data.size() * 2 );

		m_picker->clear();

		QVERIFY( m_picker->count() == 0 );
	}

	void testEvenDisabled()
	{
		StringListEvenDisabledModel model;
		model.setStringList( m_data );

		m_picker->setModel( &model );

		m_picker->setCurrentIndex( 3 );

		QPoint c( m_picker->width() / 2, m_picker->height() / 2 );

		QSignalSpy spy( m_picker.data(), QOverload< int >::of(
			&QtMWidgets::Picker::currentIndexChanged ) );

		QTest::mousePress( m_picker.data(), Qt::LeftButton, {}, c );
		QMouseEvent me( QEvent::MouseMove, c + m_delta, m_picker->mapToGlobal( c + m_delta ),
			Qt::LeftButton, Qt::LeftButton, {} );
		QApplication::sendEvent( m_picker.data(), &me );
		QTest::qWait( 500 );
		QTest::mouseRelease( m_picker.data(), Qt::LeftButton, {}, c + m_delta, 20 );
		QTest::mouseClick( m_picker.data(), Qt::LeftButton, {}, c, 20 );

		QVERIFY( spy.count() == 0 );

		m_picker->setModel( &m_model );
	}

	void testThreeItems()
	{
		QStringList data;
		data << QStringLiteral( "One" )
			<< QStringLiteral( "Very very very very very very very long line" )
			<< QStringLiteral( "Three" );
		m_model.setStringList( data );

		m_picker->resize( 150, 100 );
		m_picker->setCurrentIndex( 1 );

		QTest::qWait( 1000 );
	}

private:
	QStringList m_data;
	QSharedPointer< QtMWidgets::Picker > m_picker;
	QStringListModel m_model;
	QFont m_font;
	int m_lineHeight;
	QPoint m_delta;
};


QTEST_MAIN( TestPicker )

#include "main.moc"
