
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
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
