
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QIcon>

// QtMWidgets include.
#include <QtMWidgets/ToolBar>


//
// Widget
//

class Widget
	:	public QWidget
{
public:
	Widget()
	{
		QHBoxLayout * layout = new QHBoxLayout( this );

		QtMWidgets::ToolBar * toolbar =
			new QtMWidgets::ToolBar( this );

		toolbar->addAction( QIcon( ":/img/address-book-new.png" ) );
		toolbar->addAction( QIcon( ":/img/application-exit.png" ) );
		toolbar->addAction( QIcon( ":/img/archive-extract.png" ) );
		toolbar->addAction( QIcon( ":/img/bookmark-new.png" ) );
		toolbar->addAction( QIcon( ":/img/call-start.png" ) );

		layout->addWidget( toolbar );

		resize( 150, 150 );
	}
}; // class Widget



int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}
