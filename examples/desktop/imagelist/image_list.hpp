
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
