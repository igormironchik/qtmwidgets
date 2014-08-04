
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2014 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
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
