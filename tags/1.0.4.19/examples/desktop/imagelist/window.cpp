
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2015 Igor Mironchik

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

// SelfiePrint include.
#include "window.hpp"
#include "image_list.hpp"

// Qt include.
#include <QApplication>
#include <QStackedWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QDir>
#include <QStandardPaths>
#include <QFutureWatcher>
#include <QImage>
#include <QtConcurrent>
#include <QFuture>

// QtMWidgets include.
#include <QtMWidgets/ListModel>


//
// WindowPrivate
//

class WindowPrivate {
public:
	WindowPrivate( Window * parent )
		:	q( parent )
		,	stack( 0 )
		,	imageList( 0 )
		,	futureWatcher( 0 )
		,	currentLoadImageIndex( 0 )
	{
	}

	//! Init.
	void init();
	//!Load images.
	void loadImages();

	//! Parent.
	Window * q;
	//! Stacked widget.
	QStackedWidget * stack;
	//! List of images.
	ImageList * imageList;
	//! Image files.
	QStringList imageFiles;
	//! Load images future watcher.
	QFutureWatcher< QImage > * futureWatcher;
	//! Current index for loading image.
	int currentLoadImageIndex;
	//! Future.
	QFuture< QImage > future;
}; // class WindowPrivate

void
WindowPrivate::init()
{
	QVBoxLayout * l = new QVBoxLayout( q );
	l->setSpacing( 0 );
	l->setContentsMargins( 0, 0, 0, 0 );

	stack = new QStackedWidget( q );
	l->addWidget( stack );

	futureWatcher = new QFutureWatcher< QImage > ( q );

	QObject::connect( futureWatcher, &QFutureWatcher< QImage >::finished,
		q, &Window::_q_imageLoaded );
}

QImage loadImage( const QString & fileName, const QSize & maxSize )
{
	QImage image( fileName );

	if( !image.isNull() )
	{
		image = image.scaled( maxSize, Qt::KeepAspectRatio,
			Qt::SmoothTransformation );
	}

	return image;
}

void
WindowPrivate::loadImages()
{
	currentLoadImageIndex = 0;

	if( !imageFiles.isEmpty() )
	{
		future = QtConcurrent::run( loadImage,
			imageFiles.at( currentLoadImageIndex ),
			imageList->maxImageSize() );
		futureWatcher->setFuture( future );
	}
}


//
// Window
//

Window::Window()
	:	d( new WindowPrivate( this ) )
{
	d->init();
}

Window::~Window()
{
}

QStringList findFiles( const QString & startDir,
	const QStringList & filters )
{
	QStringList names;
	QDir dir( startDir );

	foreach( const QString & file, dir.entryList( filters, QDir::Files ) )
		names += startDir + "/" + file;

	foreach( const QString & subdir, dir.entryList(
		QDir::AllDirs | QDir::NoDotAndDotDot ) )
			names += findFiles( startDir + "/" + subdir, filters );

	return names;
}

void
Window::startAndFinish( Qt::ApplicationState state )
{
	switch( state )
	{
		case Qt::ApplicationActive :
		{
			if( !d->imageList )
			{
				const int s = size().width() / 2;

				d->imageList = new ImageList( QSize( s, s ), this );
				QtMWidgets::ListModel< QImage > * imageModel =
					new QtMWidgets::ListModel< QImage > ( this );
				d->imageList->setModel( imageModel );
				d->stack->addWidget( d->imageList );

				QStringList imageLocations =
					QStandardPaths::standardLocations(
						QStandardPaths::PicturesLocation );

				foreach( const QString & path, imageLocations )
					d->imageFiles += findFiles( path, QStringList() << "*.jpg"
						<< "*.jpeg" << "*.png" );

				d->imageList->model()->insertRows( 0, d->imageFiles.count() );

				d->loadImages();
			}
		}
			break;

		case Qt::ApplicationHidden :
		{
			QApplication::quit();
		}
			break;

		default :
			break;
	}
}

void
Window::_q_imageLoaded()
{
	QImage image = d->future.result();

	d->imageList->model()->setData( d->currentLoadImageIndex, image );

	++d->currentLoadImageIndex;

	if( d->currentLoadImageIndex < d->imageList->model()->rowCount() )
	{
		d->future = QtConcurrent::run( loadImage,
			d->imageFiles.at( d->currentLoadImageIndex ),
			d->imageList->maxImageSize() );

		d->futureWatcher->setFuture( d->future );
	}
}
