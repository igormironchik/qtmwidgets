
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QFont>

// QtMWidgets include.
#include <QtMWidgets/PageView>
#include <QtMWidgets/TextLabel>


class Widget
	:	public QWidget
{
public:
	Widget()
	{
		QHBoxLayout * l = new QHBoxLayout( this );

		QtMWidgets::PageView * view = new QtMWidgets::PageView( this );

		l->addWidget( view );

		QtMWidgets::TextLabel * page1 = new QtMWidgets::TextLabel( view );

		QTextOption opt = page1->textOption();
		opt.setAlignment( Qt::AlignCenter );

		QFont f = page1->font();
		f.setPointSize( 150 );
		f.setBold( true );

		page1->setTextOption( opt );
		page1->setFont( f );
		page1->setText( QLatin1String( "1" ) );
		view->addWidget( page1 );

		QtMWidgets::TextLabel * page2 = new QtMWidgets::TextLabel( view );

		page2->setTextOption( opt );
		page2->setFont( f );
		page2->setText( QLatin1String( "2" ) );
		view->addWidget( page2 );

		QtMWidgets::TextLabel * page3 = new QtMWidgets::TextLabel( view );

		page3->setTextOption( opt );
		page3->setFont( f );
		page3->setText( QLatin1String( "3" ) );
		view->addWidget( page3 );

		resize( 320, 480 );
	}
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}
