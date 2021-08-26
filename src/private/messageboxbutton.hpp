
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

#ifndef QTMWIDGETS__PRIVATE__MESSAGEBOXBUTTON_HPP__INCLUDED
#define QTMWIDGETS__PRIVATE__MESSAGEBOXBUTTON_HPP__INCLUDED

// Qt include.
#include <QAbstractButton>
#include <QScopedPointer>


namespace QtMWidgets {

//
// MsgBoxButton
//

class MsgBoxButtonPrivate;

class MsgBoxButton
	:	public QAbstractButton
{
	Q_OBJECT

public:
	explicit MsgBoxButton( const QString & text, QWidget * parent = 0 );
	virtual ~MsgBoxButton();

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

protected:
	void paintEvent( QPaintEvent * ) override;

private slots:
	void _q_pressed();
	void _q_released();

private:
	Q_DISABLE_COPY( MsgBoxButton )

	QScopedPointer< MsgBoxButtonPrivate > d;
}; // class MsgBoxButton

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PRIVATE__MESSAGEBOXBUTTON_HPP__INCLUDED
