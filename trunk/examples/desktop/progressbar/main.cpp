
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
#include <QSpinBox>

// QtMWidgets include.
#include <QtMWidgets/ProgressBar>


class Widget
	:	public QWidget
{
	Q_OBJECT

public:
	Widget()
	{
		QHBoxLayout * l = new QHBoxLayout( this );

		p1 = new QtMWidgets::ProgressBar( this );
		p1->setRange( 0, 99 );
		p1->setValue( 0 );

		l->addWidget( p1 );

		p2 = new QtMWidgets::ProgressBar( this );
		p2->setOrientation( Qt::Vertical );
		p2->setRange( 0, 99 );
		p2->setValue( 0 );

		l->addWidget( p2 );

		p3 = new QtMWidgets::ProgressBar( this );
		p3->setInvertedAppearance( true );
		p3->setRange( 0, 99 );
		p3->setValue( 0 );

		l->addWidget( p3 );

		p4 = new QtMWidgets::ProgressBar( this );
		p4->setOrientation( Qt::Vertical );
		p4->setInvertedAppearance( true );
		p4->setRange( 0, 99 );
		p4->setValue( 0 );

		l->addWidget( p4 );

		QSpinBox * spin = new QSpinBox( this );
		spin->setRange( -1, 99 );
		spin->setValue( 0 );

		l->addWidget( spin );

		void ( QSpinBox::*signal )( int ) = &QSpinBox::valueChanged;

		connect( spin, signal, this, &Widget::valueChanged );
	}

private slots:
	void valueChanged( int value )
	{
		p1->setValue( value );
		p2->setValue( value );
		p3->setValue( value );
		p4->setValue( value );
	}

private:
	QtMWidgets::ProgressBar * p1;
	QtMWidgets::ProgressBar * p2;
	QtMWidgets::ProgressBar * p3;
	QtMWidgets::ProgressBar * p4;
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}

#include "main.moc"
