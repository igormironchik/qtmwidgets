
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>

// QtMWidgets include.
#include <QtMWidgets/Switch>


class Widget
	:	public QWidget
{
public:
	Widget()
	{
		QHBoxLayout * l = new QHBoxLayout( this );

		{
			QtMWidgets::Switch * s = new QtMWidgets::Switch( this );

			QFont f = s->font();
			f.setBold( true );
			s->setFont( f );

			s->setOnText( QLatin1String( "ON" ) );
			s->setOffText( QLatin1String( "OFF" ) );

			l->addWidget( s );

			s->setState( QtMWidgets::Switch::AcceptedUncheck );
		}

		{
			QtMWidgets::Switch * s = new QtMWidgets::Switch( this );

			QFont f = s->font();
			f.setBold( true );
			s->setFont( f );

			s->setOnText( QLatin1String( "ON" ) );
			s->setOffText( QLatin1String( "OFF" ) );

			l->addWidget( s );

			s->setState( QtMWidgets::Switch::NotAcceptedCheck );
		}

		{
			QtMWidgets::Switch * s = new QtMWidgets::Switch( this );

			QFont f = s->font();
			f.setBold( true );
			s->setFont( f );

			s->setOnText( QLatin1String( "ON" ) );
			s->setOffText( QLatin1String( "OFF" ) );

			l->addWidget( s );

			s->setState( QtMWidgets::Switch::AcceptedCheck );
		}
	}
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}
