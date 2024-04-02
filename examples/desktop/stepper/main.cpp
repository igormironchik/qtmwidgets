
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>

// QtMWidgets include.
#include <QtMWidgets/Stepper>


class Widget
	:	public QWidget
{
	Q_OBJECT

public:
	Widget()
		:	edit( 0 )
	{
		QHBoxLayout * l = new QHBoxLayout( this );

		edit = new QLineEdit( this );
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
	QLineEdit * edit;
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}

#include "main.moc"
