
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
