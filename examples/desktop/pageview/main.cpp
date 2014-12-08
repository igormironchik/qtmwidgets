
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

		view->setFrameStyle( QFrame::StyledPanel | QFrame::Raised );

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

		page1->setFrameStyle( QFrame::StyledPanel | QFrame::Raised );

		QtMWidgets::TextLabel * page2 = new QtMWidgets::TextLabel( view );

		page2->setTextOption( opt );
		page2->setFont( f );
		page2->setText( QLatin1String( "2" ) );
		view->addWidget( page2 );

		page2->setFrameStyle( QFrame::StyledPanel | QFrame::Raised );

		QtMWidgets::TextLabel * page3 = new QtMWidgets::TextLabel( view );

		page3->setTextOption( opt );
		page3->setFont( f );
		page3->setText( QLatin1String( "3" ) );
		view->addWidget( page3 );

		page3->setFrameStyle( QFrame::StyledPanel | QFrame::Raised );

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
