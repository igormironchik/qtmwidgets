
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>

// QtMWidgets include.
#include <QtMWidgets/Slider>


class Widget
	:	public QWidget
{
public:
	Widget()
	{
		QHBoxLayout * l = new QHBoxLayout( this );

		QtMWidgets::Slider * s1 = new QtMWidgets::Slider( Qt::Horizontal, this );

		l->addWidget( s1 );

		QtMWidgets::Slider * s2 = new QtMWidgets::Slider( Qt::Vertical, this );

		l->addWidget( s2 );

		QtMWidgets::Slider * s3 = new QtMWidgets::Slider( Qt::Horizontal, this );
		s3->setInvertedAppearance( true );

		l->addWidget( s3 );

		QtMWidgets::Slider * s4 = new QtMWidgets::Slider( Qt::Vertical, this );
		s4->setInvertedAppearance( true );

		l->addWidget( s4 );
	}
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}
