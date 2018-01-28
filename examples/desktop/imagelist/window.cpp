
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
		,	filesFutureWatcher( 0 )
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
	//! Future.
	QFuture< QImage > future;
	//! Files future.
	QFuture< QStringList > fileFuture;
	//! Files future watcher.
	QFutureWatcher< QStringList > * filesFutureWatcher;
	//! Image locations.
	QStringList imageLocations;
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

	filesFutureWatcher = new QFutureWatcher< QStringList > ( q );

	QObject::connect( filesFutureWatcher, &QFutureWatcher< QStringList >::finished,
		q, &Window::_q_filesFound );
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
	if( !imageFiles.isEmpty() )
	{
		future = QtConcurrent::run( loadImage,
			imageFiles.at( 0 ),
			imageList->maxImageSize() );
		futureWatcher->setFuture( future );

		imageFiles.removeFirst();
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
	const QStringList & filters, QStringList * locations )
{
	QStringList names;
	QDir dir( startDir );

	foreach( const QString & file, dir.entryList( filters, QDir::Files ) )
		names += startDir + "/" + file;

	foreach( const QString & subdir, dir.entryList(
		QDir::AllDirs | QDir::NoDotAndDotDot ) )
			*locations += ( startDir + "/" + subdir );

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

				d->imageLocations =
					QStandardPaths::standardLocations(
						QStandardPaths::PicturesLocation );

				if( !d->imageLocations.isEmpty() )
				{
					const QString path = d->imageLocations.at( 0 );
					d->imageLocations.removeFirst();

					d->fileFuture = QtConcurrent::run(
						findFiles, path,
						QStringList() << "*.jpg" << "*.jpeg" << "*.png",
						&d->imageLocations );

					d->filesFutureWatcher->setFuture( d->fileFuture );
				}
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

	d->imageList->model()->insertRow( d->imageList->model()->rowCount() );
	d->imageList->model()->setData( d->imageList->model()->rowCount() - 1, image );

	d->loadImages();
}

void
Window::_q_filesFound()
{
	const bool start = d->imageFiles.isEmpty();

	d->imageFiles += d->fileFuture.result();

	if( start )
		d->loadImages();

	if( !d->imageLocations.isEmpty() )
	{
		const QString path = d->imageLocations.at( 0 );
		d->imageLocations.removeFirst();

		d->fileFuture = QtConcurrent::run(
			findFiles, path,
			QStringList() << "*.jpg" << "*.jpeg" << "*.png",
			&d->imageLocations );

		d->filesFutureWatcher->setFuture( d->fileFuture );
	}
}
