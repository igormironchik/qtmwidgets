
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
#include <QVBoxLayout>
#include <QMap>

// QtMWidgets include.
#include <QtMWidgets/TableView>
#include <QtMWidgets/Switch>
#include <QtMWidgets/TextLabel>
#include <QtMWidgets/Slider>
#include <QtMWidgets/NavigationBar>
#include <QtMWidgets/NavigationButton>


class RingtoneScreen
	:	public QWidget
{
public:
	RingtoneScreen( QWidget * parent )
		:	QWidget( parent )
	{
		QVBoxLayout * l = new QVBoxLayout( this );

		QtMWidgets::TextLabel * label = new QtMWidgets::TextLabel( this );
		label->setText( QLatin1String( "We have to implement list with "
			"ringtones in the future" ) );

		l->addWidget( label );
	}
};


class Widget
	:	public QWidget
{
	Q_OBJECT

	const QString ringtoneString = QLatin1String( "Ringtone" );

public:
	Widget()
		:	bar( 0 )
	{
		QVBoxLayout * l = new QVBoxLayout( this );
		l->setSpacing( 0 );
		l->setContentsMargins( 0, 0, 0, 0 );

		bar = new QtMWidgets::NavigationBar( this );

		QtMWidgets::TableView * mainScreen = createMainScreen( bar );
		mainScreen->setBlurPolicy(
			QtMWidgets::AbstractScrollArea::BlurVerticalOnly );

		bar->setMainWidget(	QLatin1String( "Sounds" ), mainScreen );

		createRingtoneScreen( mainScreen );

		l->addWidget( bar );

		resize( 320, 480 );
	}

private:
	QtMWidgets::TableView * createMainScreen( QWidget * parent )
	{
		QtMWidgets::TableView * view =
			new QtMWidgets::TableView( parent );

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
		ringtone->textLabel()->setText( ringtoneString );
		sounds->addCell( ringtone );
		cells.insert( ringtoneString, ringtone );

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

		return view;
	}

	void createRingtoneScreen( QWidget * parent )
	{
		QtMWidgets::TableViewCell * cell = cells[ ringtoneString ];

		QtMWidgets::NavigationButton * btn = new QtMWidgets::NavigationButton(
			QtMWidgets::NavigationButton::Right, cell );
		btn->setText( QLatin1String( "Xylophone" ) );

		cell->setAccessoryWidget( btn );

		connect( btn, SIGNAL( clicked() ),
			this, SLOT( selectRingtone() ) );

		auto * w = new RingtoneScreen( bar );

		bar->addWidget( parent, ringtoneString, w );

		indexes.insert( ringtoneString, w );
	}

private slots:
	void selectRingtone()
	{
		bar->showScreen( indexes[ ringtoneString ] );
	}

private:
	QMap< QString, QtMWidgets::TableViewCell* > cells;
	QMap< QString, QWidget * > indexes;
	QtMWidgets::NavigationBar * bar;
};


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	Widget w;

	w.show();

	return app.exec();
}

#include "main.moc"
