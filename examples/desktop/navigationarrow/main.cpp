
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>

// QtMWidgets include.
#include <QtMWidgets/NavigationArrow>


class Widget
	:	public QWidget
{
	Q_OBJECT

public:
	Widget()
	{
		layout = new QVBoxLayout( this );

		QHBoxLayout * h = new QHBoxLayout;

		arrow = new QtMWidgets::NavigationArrow(
			QtMWidgets::NavigationArrow::Right, this );

		arrow2 = new QtMWidgets::NavigationArrow(
			QtMWidgets::NavigationArrow::Bottom, this );

		h->addWidget( arrow );
		h->addWidget( arrow2 );

		layout->addLayout( h );

		QHBoxLayout * hbox = new QHBoxLayout;

		QPushButton * change = new QPushButton( tr( "Change Direction" ), this );
		hbox->addWidget( change );
		connect( change, SIGNAL( clicked() ), this, SLOT( changeDirection() ) );

		QPushButton * ani = new QPushButton( tr( "Animate" ), this );
		hbox->addWidget( ani );
		connect( ani, SIGNAL( clicked() ), this, SLOT( animate() ) );

		layout->addLayout( hbox );

		resize( 300, 150 );
	}

private slots:
	void changeDirection()
	{
		if( arrow->direction() == QtMWidgets::NavigationArrow::Left )
			arrow->setDirection( QtMWidgets::NavigationArrow::Right );
		else
			arrow->setDirection( QtMWidgets::NavigationArrow::Left );

		if( arrow2->direction() == QtMWidgets::NavigationArrow::Top )
			arrow2->setDirection( QtMWidgets::NavigationArrow::Bottom );
		else
			arrow2->setDirection( QtMWidgets::NavigationArrow::Top );
	}

	void animate()
	{
		arrow->animate();
		arrow2->animate();
	}

private:
	QtMWidgets::NavigationArrow * arrow;
	QtMWidgets::NavigationArrow * arrow2;
	QVBoxLayout * layout;
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}

#include "main.moc"
