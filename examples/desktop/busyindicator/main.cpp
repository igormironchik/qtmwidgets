
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

// QtMWidgets include.
#include <QtMWidgets/BusyIndicator>


class Widget
	:	public QWidget
{
public:
	Widget()
	{
		QVBoxLayout * l = new QVBoxLayout( this );

		QtMWidgets::BusyIndicator * busy =
			new QtMWidgets::BusyIndicator( this );

		l->addWidget( busy );
	}
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}
