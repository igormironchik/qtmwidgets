
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
#include <QColor>
#include <QPainter>
#include <QVBoxLayout>

// QtMWidgets include.
#include <QtMWidgets/AbstractListView>
#include <QtMWidgets/AbstractListModel>


class ListView
	:	public QtMWidgets::AbstractListView< QColor >
{
public:
	ListView( QWidget * parent )
		:	QtMWidgets::AbstractListView< QColor > ( parent )
	{
	}

protected:
	void drawRow( QPainter * painter,
		const QRect & rect, int row )
	{
		const QColor & c = model()->data( row );

		painter->setPen( c );
		painter->setBrush( c );

		painter->drawRect( rect );
	}
};


class Widget
	:	public QWidget
{
	Q_OBJECT

public:
	Widget()
	{
		QVBoxLayout * l = new QVBoxLayout( this );

		ListView * list = new ListView( this );

		l->addWidget( list );

		list->model()->appendRow( Qt::blue );
		list->model()->appendRow( Qt::red );
		list->model()->appendRow( Qt::green );
		list->model()->appendRow( Qt::cyan );
		list->model()->appendRow( Qt::magenta );
		list->model()->appendRow( Qt::gray );
		list->model()->appendRow( Qt::yellow );
		list->model()->appendRow( Qt::white );

		list->model()->appendRow( Qt::blue );
		list->model()->appendRow( Qt::red );
		list->model()->appendRow( Qt::green );
		list->model()->appendRow( Qt::cyan );
		list->model()->appendRow( Qt::magenta );
		list->model()->appendRow( Qt::gray );
		list->model()->appendRow( Qt::yellow );
		list->model()->appendRow( Qt::white );
	}
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}

#include "main.moc"
