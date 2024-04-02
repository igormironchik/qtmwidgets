
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
