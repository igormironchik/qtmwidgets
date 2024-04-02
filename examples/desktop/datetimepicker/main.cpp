
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

// QtMWidgets include.
#include <QtMWidgets/DateTimePicker>


class Widget
	:	public QWidget
{
public:
	Widget()
	{
		QVBoxLayout * l = new QVBoxLayout( this );
		QtMWidgets::DateTimePicker * picker =
			new QtMWidgets::DateTimePicker( this );

		QFont font = picker->font();
		font.setPointSize( 13 );

		picker->setFont( font );

		l->addWidget( picker );
	}
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}
