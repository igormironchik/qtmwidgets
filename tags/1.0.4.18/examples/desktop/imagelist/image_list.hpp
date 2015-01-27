
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

#ifndef IMAGE_LIST_HPP__INCLUDED
#define IMAGE_LIST_HPP__INCLUDED

// Qt include.
#include <QScopedPointer>
#include <QImage>

// QtMWidgets include.
#include <QtMWidgets/AbstractListView>


//
// ImageList
//

class ImageListPrivate;

//! List of images.
class ImageList
	:	public QtMWidgets::AbstractListView< QImage >
{
public:
	explicit ImageList( const QSize maxImageSize, QWidget * parent = 0 );
	virtual ~ImageList();

	//! \return Maximum image size.
	QSize maxImageSize() const;

protected:
	//! Draw row in the list view.
	virtual void drawRow( QPainter * painter,
		const QRect & rect, int row );

	//! \return Height of the given \a row row for the given \a width width.
	virtual int rowHeightForWidth( int row, int width ) const;

private:
	Q_DISABLE_COPY( ImageList )

	QScopedPointer< ImageListPrivate > d;
}; // class ImageList

#endif // IMAGE_LIST_HPP__INCLUDED
