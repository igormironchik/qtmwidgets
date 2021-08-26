
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

#ifndef QTMWIDGETS__PAGECONTROL_HPP__INCLUDED
#define QTMWIDGETS__PAGECONTROL_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>


namespace QtMWidgets {

//
// PageControl
//

class PageControlPrivate;

/*!
	PageControl is a set of controls that represents pages.
	Controls in this mean is circles of two possible colors.
	At one time control-circle can have highlighted color that
	shows that this page is current and have non-highlighted color
	for all other circles shows non-current pages.
*/
class PageControl
	:	public QWidget
{
	Q_OBJECT

	/*!
		\property currentIndex

		\brief the index position of the widget that is visible

		The current index is -1 if there is no current widget.

		By default, this property contains a value of -1 because the stack
		is initially empty.
	*/
	Q_PROPERTY( int currentIndex READ currentIndex WRITE setCurrentIndex
		NOTIFY currentChanged )
	/*!
		\property count

		\brief the number of widgets contained by this page view

		By default, this property contains a value of 0.

		\sa currentIndex()
	*/
	Q_PROPERTY( int count READ count WRITE setCount )
	/*!
		\property pageIndicatorColor

		\brief Color for non-active page indicator

		By default, this property is QPalette::Highlight darker on 75.
	*/
	Q_PROPERTY( QColor pageIndicatorColor READ pageIndicatorColor
		WRITE setPageIndicatorColor )
	/*!
		\property currentPageIndicatorColor

		\brief Color for current page indicator

		By default, this property is QPalette::Highlight.
	*/
	Q_PROPERTY( QColor currentPageIndicatorColor READ currentPageIndicatorColor
		WRITE setCurrentPageIndicatorColor )

signals:
	/*!
		This signal is emitted whenever the current page changes.

		The parameter holds the \a index of the new current page, or -1
		if there isn't a new one (for example, if there are no pages).
	*/
	void currentChanged( int index, int prev );

public:
	PageControl( QWidget * parent = 0 );
	virtual ~PageControl();

	//! \return The number of pages.
	int count() const;
	//! \return Current index.
	int currentIndex() const;

	//! \return Page indicator color.
	const QColor & pageIndicatorColor() const;
	//! Set page indicator color.
	void setPageIndicatorColor( const QColor & c );

	//! \return Current page indicator color.
	const QColor & currentPageIndicatorColor() const;
	//! Set current page indicator color.
	void setCurrentPageIndicatorColor( const QColor & c );

	bool hasHeightForWidth() const override;
	int heightForWidth( int width ) const override;

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

public slots:
	//! Set current index.
	void setCurrentIndex( int index );
	//! Set count of pages.
	void setCount( int c );

protected:
	void paintEvent( QPaintEvent * ) override;
	void resizeEvent( QResizeEvent * e ) override;
	void mousePressEvent( QMouseEvent * e ) override;
	void mouseReleaseEvent( QMouseEvent * e ) override;

private:
	Q_DISABLE_COPY( PageControl )

	QScopedPointer< PageControlPrivate > d;
}; // class PageControl

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PAGECONTROL_HPP__INCLUDED
