
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// Qt include.
#include <QApplication>
#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QHBoxLayout>
#include <QMessageBox>

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
		setModel( new QtMWidgets::ListModel< QColor > () );
	}

protected:
	void drawRow( QPainter * painter,
		const QRect & rect, int row )
	{
		const QColor & c = model()->data( row );

		painter->setPen( Qt::black );
		painter->setBrush( c );

		painter->drawRect( rect );

		QTextOption opt;
		opt.setAlignment( Qt::AlignCenter );

		painter->drawText( rect, QString::number( row ), opt );
	}
};


class Widget
	:	public QWidget
{
	Q_OBJECT

public:
	Widget()
	{
		QHBoxLayout * l = new QHBoxLayout( this );

		ListView * list = new ListView( this );
		list->setSpacing( 5 );

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

		connect( list, &ListView::rowLongTouched,
			this, &Widget::longTouched );

		connect( list, &ListView::rowDoubleTouched,
			this, &Widget::doubleTouched );
	}

private slots:
	void longTouched( int row )
	{
		QMessageBox::information( this, tr( "Long Touch..." ),
			tr( "Row %1 was long touched." ).arg( QString::number( row ) ) );
	}

	void doubleTouched( int row )
	{
		QMessageBox::information( this, tr( "Double Touch..." ),
			tr( "Row %1 was double touched." ).arg( QString::number( row ) ) );
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
