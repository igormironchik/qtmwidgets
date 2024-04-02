
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QImage>

// QtMWidgets include.
#include <QtMWidgets/ScrollArea>


class Widget
	:	public QWidget
{
public:
	Widget()
	{
		QVBoxLayout * l = new QVBoxLayout( this );
		QtMWidgets::ScrollArea * scrollArea =
			new QtMWidgets::ScrollArea( this );

		QLabel * imageLabel = new QLabel;
		QImage image( ":/flower.jpg" );
		imageLabel->setPixmap( QPixmap::fromImage( image ) );

		scrollArea->setBackgroundRole( QPalette::Dark );
		scrollArea->setWidget( imageLabel );

		l->addWidget( scrollArea );
	}
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.showMaximized();

	return app.exec();
}
