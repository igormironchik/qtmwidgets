
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

// QtMWidgets include.
#include <QtMWidgets/Picker>


class Widget
	:	public QWidget
{
public:
	Widget()
	{
		QVBoxLayout * l = new QVBoxLayout( this );
		QtMWidgets::Picker * picker = new QtMWidgets::Picker( this );

		picker->addItem( QLatin1String( "English" ) );
		picker->addItem( QLatin1String( "Russian" ) );
		picker->addItem( QLatin1String( "German" ) );
		picker->addItem( QLatin1String( "Spanish" ) );
		picker->addItem( QLatin1String( "Portuguese" ) );
		picker->addItem( QLatin1String( "Belorussian" ) );
		picker->addItem( QLatin1String( "Polish" ) );
		picker->addItem( QLatin1String( "Ukrainian" ) );

		QFont font = picker->font();
		font.setBold( true );
		font.setPointSize( 25 );

		picker->setFont( font );

		l->addWidget( picker );
	}
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.showMaximized();

	return app.exec();
}
