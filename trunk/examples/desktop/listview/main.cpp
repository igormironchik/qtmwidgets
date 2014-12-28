
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
#include <QTimer>
#include <QChar>
#include <QDebug>

// QtMWidgets include.
#include <QtMWidgets/ListModel>


class Widget
	:	public QWidget
{
	Q_OBJECT

public:
	Widget()
	{
	}

public slots:
	void start()
	{
		QtMWidgets::ListModel< QChar > model;

		model.insertRow( 0 );
		model.setData( 0, QChar( 'A' ) );

		print( model );

		model.insertRows( 1, 2 );
		model.setData( 1, QChar( 'B' ) );
		model.setData( 2, QChar( 'C' ) );

		print( model );

		model.moveRow( 1, 2 );

		print( model );

		model.sort();

		print( model );
	}

private:
	void print( const QtMWidgets::ListModel< QChar > & m )
	{
		qDebug() << "--------------------";

		for( int i = 0; i < m.rowCount(); ++i )
			qDebug() << m.data( i );

		qDebug() << "--------------------";
	}
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	QTimer::singleShot( 0, &w, &Widget::start );

	return app.exec();
}

#include "main.moc"
