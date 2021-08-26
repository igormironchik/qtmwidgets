
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

	/*!
		\property direction

		Direction of the arrow.
	*/
	Q_PROPERTY( Direction direction READ direction WRITE setDirection )
	/*!
		\property color

		Color of the arrow.

		By default, this property is QPalette::Highlight.
	*/
	Q_PROPERTY( QColor color READ color WRITE setColor )

signals:
	/*!
		This signal emitted when user clicked on the arrow.
	*/
	void clicked();

public:
	//! Direction of the arrow.
	enum Direction {
		//! Left arrow.
		Left = 1,
		//! Right arrow.
		Right = 2,
		//! Top arrow.
		Top = 3,
		//! Bottom arrow.
		Bottom = 4
	}; // enum Direction

	Q_ENUM( Direction )

public:
	explicit NavigationArrow( Direction direction, QWidget * parent = 0 );
	virtual ~NavigationArrow();

	//! \return Direction of the arrow.
	Direction direction() const;
	//! Set direction of the arrow.
	void setDirection( Direction direct );

	//! \return Color.
	const QColor & color() const;
	//! Set color.
	void setColor( const QColor & color );

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

public slots:
	//! Animate.
	void animate();

protected:
	void paintEvent( QPaintEvent * ) override;
	void mousePressEvent( QMouseEvent * e ) override;
	void mouseReleaseEvent( QMouseEvent * e ) override;

private slots:
	void _q_timer();

private:
	friend class NavigationArrowPrivate;

	Q_DISABLE_COPY( NavigationArrow )

	QScopedPointer< NavigationArrowPrivate > d;
}; // class NavigationArrow

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__NAVIGATIONARROW_HPP__INCLUDED
