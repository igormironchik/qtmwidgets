
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

#ifndef QTMWIDGETS__TOOLBAR_HPP__INCLUDED
#define QTMWIDGETS__TOOLBAR_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>
#include <QAction>


namespace QtMWidgets {

//
// ToolButton
//

class ToolButtonPrivate;

/*!
	Button in the ToolBar.
*/
class ToolButton
	:	public QWidget
{
	Q_OBJECT

	/*!
		\property iconSize

		\brief size of icon.
	*/
	Q_PROPERTY( QSize iconSize READ iconSize WRITE setIconSize
		NOTIFY iconSizeChanged )

signals:
	/*!
		This signal is emitted when the icon size is changed. The \a
		iconSize parameter holds the toolbar's new icon size.

		\sa iconSize
	*/
	void iconSizeChanged( const QSize & iconSize );

public:
	explicit ToolButton( QAction * action, QWidget * parent = 0 );
	virtual ~ToolButton();

	//! \return Icon's size.
	QSize iconSize() const;

	//! \return Action for this button.
	QAction * action() const;

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

public slots:
	//! Set icon's size.
	void setIconSize( const QSize & s );

protected:
	void paintEvent( QPaintEvent * ) override;
	void mousePressEvent( QMouseEvent * e ) override;
	void mouseReleaseEvent( QMouseEvent * e ) override;

private:
	Q_DISABLE_COPY( ToolButton )

	QScopedPointer< ToolButtonPrivate > d;
}; // class ToolButton


//
// ToolBar
//

class ToolBarPrivate;

/*!
	The ToolBar class provides a panel that contains a
	set of ToolButton.
*/
class ToolBar
	:	public QWidget
{
	Q_OBJECT

	/*!
		\property orientation

		\brief orientation of the toolbar

		The default is Qt::Horizontal.
	*/
	Q_PROPERTY( Qt::Orientation orientation READ orientation
		WRITE setOrientation NOTIFY orientationChanged )
	/*!
		\property iconSize

		\brief size of icons in the toolbar.
	*/
	Q_PROPERTY( QSize iconSize READ iconSize WRITE setIconSize
		NOTIFY iconSizeChanged )
	/*!
		\property alignment

		\brief Alignment of the tool bar.

		alignment has the meaning only if width (height) of the
		tool bar is greater then space needed to place all tool
		buttons. Only Qt::AlignLeft, Qt::AlignRight, Qt::AlignHCenter,
		Qt::AlignVCenter, Qt::AlignCenter, Qt::AlignTop, Qt::AlignBottom
		has the meaning, all other values are ignored.

		By default, this property is Qt::AlignLeft.
	*/
	Q_PROPERTY( Qt::Alignment alignment READ alignment
		WRITE setAlignment )

signals:
	/*!
		This signal is emitted when an action in this toolbar is triggered.
		This happens when the action's button is pressed, or when the
		action is triggered in some other way outside the toolbar. The parameter
		holds the triggered \a action.
	*/
	void actionTriggered( QAction * action );
	/*!
		This signal is emitted when the orientation of the toolbar changes.
		The \a orientation parameter holds the toolbar's new orientation.

		\sa orientation
	*/
	void orientationChanged( Qt::Orientation orientation );
	/*!
		This signal is emitted when the icon size is changed. The \a
		iconSize parameter holds the toolbar's new icon size.

		\sa iconSize
	*/
	void iconSizeChanged( const QSize & iconSize );

public:
	/*!
		Constructs a ToolBar with the given \a parent.
	*/
	explicit ToolBar( QWidget * parent = 0 );
	virtual ~ToolBar();

	//! \return Orientation of the ToolBar.
	Qt::Orientation orientation() const;

	/*!
		Removes all actions from the toolbar.

		\sa removeAction()
	*/
	void clear();

	/*!
		Appends the action to the toolbar's list of actions.

		\sa QWidget::addAction()
	*/
	using QWidget::addAction;

	/*!
		Creates a new action with the given \a icon. This
		action is added to the end of the toolbar.
	*/
	QAction * addAction( const QIcon & icon );
	/*!
		Creates a new action with the given \a icon. This
		action is added to the end of the toolbar. The action's
		QAction::triggered() signal is connected to \a
		member in \a receiver.
	*/
	QAction * addAction( const QIcon & icon,
		const QObject * receiver, const char * member );
	/*!
		Creates a new action with the given \a icon. This
		action is added to the end of the toolbar. The action's
		QAction::triggered() signal is connected to \a
		slot in \a receiver.
	*/
	template <typename Func1>
	inline QAction * addAction( const QIcon & icon,
		const typename QtPrivate::FunctionPointer< Func1 >::Object * receiver,
		Func1 slot )
	{
		QAction * action = new QAction( icon, QString(), this );
		QObject::connect( action, &QAction::triggered, receiver, slot );
		addAction( action );
		return action;
	}

	/*!
		Creates a new action with the given \a icon. This
		action is added to the end of the toolbar. The action's
		QAction::triggered() signal is connected to \a
		functor.
	*/
	template <typename Func1>
	inline QAction * addAction( const QIcon & icon, Func1 functor )
	{
		QAction * action = new QAction( icon, QString(), this );
		QObject::connect( action, &QAction::triggered, functor );
		addAction( action );
		return action;
	}

	/*!
		Returns the geometry of the toolbar item associated with the given
		\a action, or an invalid QRect if no matching item is found.
	*/
	QRect actionGeometry( QAction * action ) const;
	/*!
		Returns the action at point \a p. This function returns zero if no
		action was found.

		\sa QWidget::childAt()
	*/
	QAction * actionAt( const QPoint & p ) const;
	/*!
		Returns the action at the point \a x, \a y. This function returns
		zero if no action was found.
	*/
	inline QAction * actionAt( int x, int y ) const
	{
		return actionAt( QPoint( x, y ) );
	}

	//! \return Icon size of the ToolBar.
	QSize iconSize() const;

	//! \return Alignment.
	Qt::Alignment alignment() const;

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

public slots:
	//! Set icon's size of the ToolBar.
	void setIconSize( const QSize & iconSize );
	//! Set alignment.
	void setAlignment( Qt::Alignment align );
	//! Set orientation of the ToolBar.
	void setOrientation( Qt::Orientation orientation );

protected:
	void actionEvent( QActionEvent * event ) override;

private slots:
	void _q_leftArrowClicked();
	void _q_rightArrowClicked();

private:
	Q_DISABLE_COPY( ToolBar )

	QScopedPointer< ToolBarPrivate > d;

}; // class ToolBar

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__TOOLBAR_HPP__INCLUDED
