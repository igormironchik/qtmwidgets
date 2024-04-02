
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>

// QtMWidgets include.
#include <QtMWidgets/NavigationButton>


class Widget
	:	public QWidget
{
public:
	Widget()
	{
		QHBoxLayout * l = new QHBoxLayout( this );

		QtMWidgets::NavigationButton * left =
			new QtMWidgets::NavigationButton( QtMWidgets::NavigationButton::Left,
				this );
		left->setText( tr( "Left Button" ) );

		l->addWidget( left );

		QtMWidgets::NavigationButton * right =
			new QtMWidgets::NavigationButton( QtMWidgets::NavigationButton::Right,
				this );
		right->setText( tr( "Right Button" ) );

		l->addWidget( right );

		resize( 300, 150 );
	}
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}
