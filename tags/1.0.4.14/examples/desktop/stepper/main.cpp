
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
#include <QtMWidgets/Stepper>
#include <QtMWidgets/LineEdit>


class Widget
	:	public QWidget
{
	Q_OBJECT

public:
	Widget()
		:	edit( 0 )
	{
		QHBoxLayout * l = new QHBoxLayout( this );

		edit = new QtMWidgets::LineEdit( this );
		edit->setReadOnly( true );

		l->addWidget( edit );

		QtMWidgets::Stepper * s = new QtMWidgets::Stepper( this );

		s->setWrapping( true );

		edit->setText( QString::number( s->value() ) );

		l->addWidget( s );

		connect( s, &QtMWidgets::Stepper::valueChanged,
			this, &Widget::valueChanged );
	}

private slots:
	void valueChanged( int value )
	{
		edit->setText( QString::number( value ) );
	}

private:
	QtMWidgets::LineEdit * edit;
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}

#include "main.moc"
