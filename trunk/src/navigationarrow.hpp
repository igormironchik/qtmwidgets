
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

#ifndef QTMWIDGETS__NAVIGATIONARROW_HPP__INCLUDED
#define QTMWIDGETS__NAVIGATIONARROW_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>


namespace QtMWidgets {

//
// NavigationArrow
//

class NavigationArrowPrivate;

/*!
	NavigationArrow is simple widget that just displays
	left or right arrow. This widget can animate his state,
	for example, when NavigationButton clicked it uses this
	animation for inform of click.
*/
class NavigationArrow
	:	public QWidget
{
	Q_OBJECT

	Q_ENUMS( Type )

public:
	//! Type of the arrow.
	enum Type {
		//! Left arrow.
		Left = 1,
		//! Right arrow.
		Right = 2
	}; // enum Type

public:
	explicit NavigationArrow( Type type, QWidget * parent = 0 );
	virtual ~NavigationArrow();

	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;

public slots:
	//! Animate.
	void animate();

protected:
	virtual void paintEvent( QPaintEvent * );

private:
	Q_DISABLE_COPY( NavigationArrow )

	QScopedPointer< NavigationArrowPrivate > d;
}; // class NavigationArrow

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__NAVIGATIONARROW_HPP__INCLUDED
