
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

#ifndef QTMWIDGETS__NAVIGATIONBAR_HPP__INCLUDED
#define QTMWIDGETS__NAVIGATIONBAR_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>


namespace QtMWidgets {

//
// NavigationBar
//

class NavigationBarPrivate;

/*!
	Navigation bars allow you to present your app’s content in an
	organized and intuitive way. A navigation bar is displayed at
	the top of the screen, and contains buttons for navigating
	through a hierarchy of screens. A navigation bar generally has
	a back button, a title, and a right button.
*/
class NavigationBar
	:	public QWidget
{
	Q_OBJECT

signals:
	//! This signal emitted when current screen changes.
	void currentChanged( int index );

public:
	NavigationBar( QWidget * parent = 0 );
	virtual ~NavigationBar();

	/*!
		Set main widget of the hierarchy of screens.

		\return Index of the screen.

		Main widgets can be more than one.
	*/
	int setMainWidget( const QString & title,
		QWidget * widget );

	/*!
		Add subordinate screen to the hierarchy of screens.

		\return Index of the screen.
	*/
	int addWidget( QWidget * parent, const QString & title,
		QWidget * widget );

	//! Remove widget from the hierarchy.
	void removeWidget( QWidget * widget );

	//! \return Index of the current screen.
	int currentIndex() const;

	//! \return Current widget.
	QWidget * currentWidget() const;

	//! \return Index of the given \a widget widget.
	int indexOf( QWidget * widget ) const;

	//! \return Widget with the given \a index index.
	QWidget * widget( int index ) const;

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

public slots:
	//!	Show screen with the given \a index index.
	void showScreen( QWidget * s );
	//! Show previous screen.
	void showPreviousScreen();
	//! Show next screen.
	void showNextScreen();

protected:
	void resizeEvent( QResizeEvent * e ) override;
	void hideEvent( QHideEvent * e ) override;

private:
	Q_DISABLE_COPY( NavigationBar )

	QScopedPointer< NavigationBarPrivate > d;
}; // class NavigationBar

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__NAVIGATIONBAR_HPP__INCLUDED
