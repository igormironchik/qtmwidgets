
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

#include "image_list.hpp"

// Qt include.
#include <QPainter>
#include <QLinearGradient>

// QtMWidgets include.
#include <QtMWidgets/Color>


//
// ImageListPrivate
//

class ImageListPrivate {
public:
	ImageListPrivate( const QSize & maxSize, ImageList * parent )
		:	q( parent )
		,	borderWidth( 5 )
		,	maxImageSize( QSize( maxSize.width() - 2 * borderWidth,
				maxSize.height() - 2 * borderWidth ) )
	{
	}

	//! Init.
	void init();

	//! Parent.
	ImageList * q;
	//! Border width.
	int borderWidth;
	//! Maximum image size.
	QSize maxImageSize;
}; // class ImageListPrivate

void
ImageListPrivate::init()
{
	q->setSpacing( borderWidth );
}


//
// ImageList
//


ImageList::ImageList( const QSize maxImageSize, QWidget * parent )
	:	QtMWidgets::AbstractListView< QImage > ( parent )
	,	d( new ImageListPrivate( maxImageSize, this ) )
{
	d->init();
}

ImageList::~ImageList()
{
}

QSize
ImageList::maxImageSize() const
{
	return d->maxImageSize;
}

void
ImageList::drawRow( QPainter * painter,
	const QRect & rect, int row )
{
	QColor background = palette().color( QPalette::Button );

	QLinearGradient g( QPointF( 0.0, 0.0 ), QPointF( 1.0, 0.0 ) );
	g.setCoordinateMode( QGradient::ObjectBoundingMode );
	g.setColorAt( 0.0, background );
	g.setColorAt( 1.0, QtMWidgets::lighterColor( background, 75 ) );

	QColor border = palette().color( QPalette::Dark );

	painter->setPen( border );
	painter->setBrush( g );

	const QImage & image = model()->data( row );

	const int x = rect.x() + ( rect.width() - image.width() ) / 2;
	const int y = rect.y();

	const QRect r( x, y,
		image.width() + 2 * d->borderWidth,
		image.height() + 2 * d->borderWidth );

	painter->drawRect( r );
	painter->drawRect( r.adjusted( d->borderWidth - 1, d->borderWidth - 1,
		- d->borderWidth, - d->borderWidth ) );

	if( !image.isNull() )
		painter->drawImage( QPoint( x + d->borderWidth, y + d->borderWidth ),
			image );
}

int
ImageList::rowHeightForWidth( int row, int width ) const
{
	Q_UNUSED( width )

	return model()->data( row ).height() + 2 * d->borderWidth;
}
