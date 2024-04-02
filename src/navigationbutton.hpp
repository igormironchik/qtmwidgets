
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef QTMWIDGETS__NAVIGATIONBUTTON_HPP__INCLUDED
#define QTMWIDGETS__NAVIGATIONBUTTON_HPP__INCLUDED

// Qt include.
#include <QAbstractButton>
#include <QScopedPointer>


namespace QtMWidgets {

//
// NavigationButton
//

class NavigationButtonPrivate;

/*!
	NavigationButton is a button with text and arrow.
*/
class NavigationButton
	:	public QAbstractButton
{
	Q_OBJECT

	/*!
		\property direction

		Direction of the button.
	*/
	Q_PROPERTY( Direction direction READ direction WRITE setDirection )
	/*!
		\property textColor

		Color of the text.
	*/
	Q_PROPERTY( QColor textColor READ textColor WRITE setTextColor )
	/*!
		\property arrowColor

		Color of the arrow.
	*/
	Q_PROPERTY( QColor arrowColor READ arrowColor WRITE setArrowColor )

public:
	//! Direction of the button.
	enum Direction {
		//! Left.
		Left = 1,
		//! Right.
		Right = 2
	}; // enum Direction
	Q_ENUM( Direction )

public:
	explicit NavigationButton( Direction direction, QWidget * parent = 0 );

	NavigationButton( Direction direction, const QString & text,
		QWidget * parent = 0 );

	virtual ~NavigationButton();

	//! \return Direction of the button.
	Direction direction() const;
	//! Set direction of the button.
	void setDirection( Direction direct );

	//! \return Text color.
	const QColor & textColor() const;
	//! Set text color.
	void setTextColor( const QColor & c );

	//! \return Arrow color.
	const QColor & arrowColor() const;
	//! Set arrow color.
	void setArrowColor( const QColor & c );

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

protected:
	void paintEvent( QPaintEvent * ) override;

private slots:
	void _q_pressed();
	void _q_released();

private:
	Q_DISABLE_COPY( NavigationButton )

	QScopedPointer< NavigationButtonPrivate > d;
}; // class NavigationButton

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__NAVIGATIONBUTTON_HPP__INCLUDED
