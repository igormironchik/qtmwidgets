
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

// QtMWidgets include.
#include <QtMWidgets/TableView>
#include <QtMWidgets/Switch>
#include <QtMWidgets/TextLabel>
#include <QtMWidgets/Slider>


class Widget
	:	public QWidget
{
public:
	Widget()
	{
		QVBoxLayout * l = new QVBoxLayout( this );
		QtMWidgets::TableView * view =
			new QtMWidgets::TableView( this );
		view->setHighlightCellOnClick( true );

		QtMWidgets::TableViewSection * ringerAndAlerts =
			new QtMWidgets::TableViewSection( view );
		ringerAndAlerts->header()->setText(
			QLatin1String( "RINGER AND ALERTS" ) );
		ringerAndAlerts->footer()->setText(
			QLatin1String( "The volume of the ringer and alerts "
				"can be adjusted using the volume buttons." ) );

		QtMWidgets::TableViewCell * volume =
			new QtMWidgets::TableViewCell( ringerAndAlerts );
		volume->layout()->setSpacing( 0 );
		QtMWidgets::Slider * volumeSlider = new QtMWidgets::Slider(
			Qt::Horizontal, volume );
		volume->setAccessoryWidget( volumeSlider );
		ringerAndAlerts->addCell( volume );

		QtMWidgets::TableViewCell * changeWithButtons =
			new QtMWidgets::TableViewCell( ringerAndAlerts );
		changeWithButtons->textLabel()->setText(
			QLatin1String( "Change with Buttons" ) );

		QtMWidgets::Switch * changeWithButtonsSwitch =
			new QtMWidgets::Switch( changeWithButtons );
		changeWithButtonsSwitch->setState(
			QtMWidgets::Switch::AcceptedCheck );

		changeWithButtons->setAccessoryWidget( changeWithButtonsSwitch );
		ringerAndAlerts->addCell( changeWithButtons );

		view->addSection( ringerAndAlerts );

		QtMWidgets::TableViewSection * sounds =
			new QtMWidgets::TableViewSection( view );
		sounds->header()->setText( QLatin1String( "SOUNDS" ) );

		QtMWidgets::TableViewCell * ringtone =
			new QtMWidgets::TableViewCell( sounds );
		ringtone->textLabel()->setText( QLatin1String( "Ringtone" ) );
		sounds->addCell( ringtone );

		QtMWidgets::TableViewCell * textTone =
			new QtMWidgets::TableViewCell( sounds );
		textTone->textLabel()->setText( QLatin1String( "Text Tone" ) );
		sounds->addCell( textTone );

		QtMWidgets::TableViewCell * newMail =
			new QtMWidgets::TableViewCell( sounds );
		newMail->textLabel()->setText( QLatin1String( "New Mail" ) );
		sounds->addCell( newMail );

		QtMWidgets::TableViewCell * sentMail =
			new QtMWidgets::TableViewCell( sounds );
		sentMail->textLabel()->setText( QLatin1String( "Sent Mail" ) );
		sounds->addCell( sentMail );

		QtMWidgets::TableViewCell * tweet =
			new QtMWidgets::TableViewCell( sounds );
		tweet->textLabel()->setText( QLatin1String( "Tweet" ) );
		sounds->addCell( tweet );

		QtMWidgets::TableViewCell * facebookPost =
			new QtMWidgets::TableViewCell( sounds );
		facebookPost->textLabel()->setText( QLatin1String( "Facebook Post" ) );
		sounds->addCell( facebookPost );

		view->addSection( sounds );

		l->addWidget( view );

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
