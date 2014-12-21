
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

#ifndef QTMWIDGETS__LINEEDIT_HPP__INCLUDED
#define QTMWIDGETS__LINEEDIT_HPP__INCLUDED

// Qt include.
#include <QLineEdit>
#include <QScopedPointer>


namespace QtMWidgets {

//
// LineEdit
//

class LineEditPrivate;

/*!
	LineEdit is a QLineEdit but with pop-up CursorShifter.
*/
class LineEdit
	:	public QLineEdit
{
	Q_OBJECT

public:
	LineEdit( QWidget * parent = 0 );
	explicit LineEdit( const QString & text, QWidget * parent = 0 );

	virtual ~LineEdit();

protected:
	virtual void keyPressEvent( QKeyEvent * e );
	virtual void mousePressEvent( QMouseEvent * e );
	virtual void mouseMoveEvent( QMouseEvent * e );
	virtual void focusOutEvent( QFocusEvent * e );

private slots:
	void _q_cursorShifterPosChanged( const QPoint & pos );

private:
	friend class LineEditPrivate;

	Q_DISABLE_COPY( LineEdit )

	QScopedPointer< LineEditPrivate > d;
}; // class LineEdit

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__LINEEDIT_HPP__INCLUDED
