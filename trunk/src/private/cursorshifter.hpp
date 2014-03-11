
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

#ifndef QTMWIDGETS__PRIVATE__CURSORSHIFTER_HPP__INCLUDED
#define QTMWIDGETS__PRIVATE__CURSORSHIFTER_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>


namespace QtMWidgets {

//
// CursorShifter
//

class CursorShifterPrivate;

/*!
	CursorShifter is a small widget that used to move
	cursor in such widgets as LineEdit and TextEdit. This widget
	usualy pops up when user clickes on the text. CursorShifter
	automatically hides when user doesn't change it's position.
*/
class CursorShifter
	:	public QWidget
{
	Q_OBJECT

signals:
	/*!
		Emits when position changes.

		\a pos is the global postion of the mouse cursor.

		\note \a pos in this signal is possible position of the
		widget. I.e. this position is not accepted yet, so widget
		draws on his old position. When you use CursorShifter you
		have to accept new postion so widget will be moved to the
		new one. While new position will not be accepted with
		setCursorPos() cursorPos() will return old position.

		\sa setCursorPos()
	*/
	void posChanged( const QPoint & pos );

public:
	CursorShifter( QWidget * parent = 0 );
	virtual ~CursorShifter();

	/*!
		\return Cursor position.

		Cursor position is a point under the cursor in text.

		\note Cursor position is the global position.
	*/
	QPoint cursorPos() const;

	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;

public slots:
	/*!
		Accept new \a pos cursor position.

		\note \a pos must be the global position.
	*/
	void setCursorPos( const QPoint & pos );
	//! Pop up.
	void popup();

protected:
	virtual void paintEvent( QPaintEvent * );
	virtual void mousePressEvent( QMouseEvent * e );
	virtual void mouseMoveEvent( QMouseEvent * e );
	virtual void mouseReleaseEvent( QMouseEvent * );

private slots:
	void _q_hideTimer();

private:
	Q_DISABLE_COPY( CursorShifter )

	QScopedPointer< CursorShifterPrivate > d;
}; // class CursorShifter

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PRIVATE__CURSORSHIFTER_HPP__INCLUDED
