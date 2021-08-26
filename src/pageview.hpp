
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

#ifndef QTMWIDGETS__PAGEVIEW_HPP__INCLUDED
#define QTMWIDGETS__PAGEVIEW_HPP__INCLUDED

// Qt include.
#include <QFrame>
#include <QScopedPointer>


namespace QtMWidgets {

//
// PageView
//

class PageViewPrivate;

/*!
	A page view lets the user navigate between pages of content.
	Navigation can be controlled programmatically by your app or directly
	by the user using gestures.

	API of PageView is very similar to QStackedWidget so it's very
	simple to switch to the PageView from QStackedWidget.

	PageView can display PageControl, and by default page control is displayed.
	Page control show how much pages managed by PageView and indicates
	current page. \sa PageControl
*/
class PageView
	:	public QFrame
{
	Q_OBJECT

	/*!
		\property currentIndex

		\brief the index position of the widget that is visible

		The current index is -1 if there is no current widget.

		By default, this property contains a value of -1 because the stack
		is initially empty.

		\sa currentWidget(), indexOf()
	*/
	Q_PROPERTY( int currentIndex READ currentIndex WRITE setCurrentIndex
		NOTIFY currentChanged )
	/*!
		\property count

		\brief the number of widgets contained by this page view

		By default, this property contains a value of 0.

		\sa currentIndex(), widget()
	*/
	Q_PROPERTY( int count READ count )
	/*!
		\property showPageControl

		\brief Should PageView display PageControl or not.

		By default, this property is true.
	*/
	Q_PROPERTY( bool showPageControl READ showPageControl
		WRITE setShowPageControl )

signals:
	/*!
		This signal is emitted whenever the current widget changes.

		The parameter holds the \a index of the new current widget, or -1
		if there isn't a new one (for example, if there are no widgets in
		the PageView).

		\sa currentWidget(), setCurrentWidget()
	*/
	void currentChanged( int index );
	/*!
		This signal is emitted whenever a widget is removed. The widget's
		\a index is passed as parameter.

		\sa removeWidget()
	*/
	void widgetRemoved( int index );

public:
	PageView( QWidget * parent = 0 );
	virtual ~PageView();

	/*!
		Appends the given \a widget to the PageView and returns the
		index position. Ownership of \a widget is passed on to the
		PageView.

		If the PageView is empty before this function is called,
		\a widget becomes the current widget.

		\sa insertWidget(), removeWidget(), setCurrentWidget()
	*/
	int addWidget( QWidget * widget );
	//! \return The number of widgets contained by this page view.
	int count() const;
	//! \return Current index.
	int currentIndex() const;
	/*!
		Returns the current widget, or 0 if there are no child widgets.

		\sa currentIndex(), setCurrentWidget()
	*/
	QWidget * currentWidget() const;
	/*!
		Returns the index of the given \a widget, or -1 if the given \a
		widget is not a child of the PageView.

		\sa currentIndex(), widget()
	*/
	int indexOf( QWidget * widget ) const;
	/*!
		Inserts the given \a widget at the given \a index in the
		PageView. Ownership of \a widget is passed on to the
		PageView. If \a index is out of range, the \a widget is
		appended (in which case it is the actual index of the \a widget
		that is returned).

		If the PageView was empty before this function is called,
		the given \a widget becomes the current widget.

		Inserting a new widget at an index less than or equal to the current index
		will increment the current index, but keep the current widget.

		\sa addWidget(), removeWidget(), setCurrentWidget()
	*/
	int insertWidget( int index, QWidget * widget );
	/*!
		Removes \a widget from the PageView. i.e., \a widget is \e
		not deleted but simply removed from the stacked layout, causing it
		to be hidden.

		\b{Note:} Ownership of \a widget reverts to the application.

		\sa addWidget(), insertWidget(), currentWidget()
	*/
	void removeWidget( QWidget * widget );
	/*!
		Returns the widget at the given \a index, or 0 if there is no such
		widget.

		\sa currentWidget(), indexOf()
	*/
	QWidget * widget( int index ) const;
	//! \return Is PageControl shown?
	bool showPageControl() const;
	//! Show/hide PageControl.
	void setShowPageControl( bool show );

public slots:
	//! Set current index.
	void setCurrentIndex( int index );
	/*!
		Sets the current widget to be the specified \a widget. The new
		current widget must already be contained in this page view.

		\sa currentWidget(), setCurrentIndex()
	 */
	void setCurrentWidget( QWidget * widget );

protected:
	void resizeEvent( QResizeEvent * e ) override;
	void mousePressEvent( QMouseEvent * e ) override;
	void mouseMoveEvent( QMouseEvent * e ) override;
	void mouseReleaseEvent( QMouseEvent * e ) override;

private slots:
	void _q_currentIndexChanged( int index, int prev );
	void _q_normalizePageAnimation( const QVariant & v );
	void _q_normalizeAnimationFinished();

private:
	Q_DISABLE_COPY( PageView )

	QScopedPointer< PageViewPrivate > d;
}; // class PageView

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PAGEVIEW_HPP__INCLUDED
