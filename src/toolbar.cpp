
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

// QtMWidgets include.
#include "toolbar.hpp"
#include "fingergeometry.hpp"
#include "navigationarrow.hpp"

// Qt include.
#include <QActionEvent>
#include <QAction>
#include <QLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QVector>


namespace QtMWidgets {

//
// ToolButtonPrivate
//

class ToolButtonPrivate {
public:
	ToolButtonPrivate( QAction * a, ToolButton * parent )
		:	q( parent )
		,	action( a )
		,	leftButtonPressed( false )
	{
	}

	ToolButton * q;
	QAction * action;
	QSize iconSize;
	bool leftButtonPressed;
}; // class ToolButtonPrivate


//
// ToolButton
//

ToolButton::ToolButton( QAction * action, QWidget * parent )
	:	QWidget( parent )
	,	d( new ToolButtonPrivate( action, this ) )
{
	if( !action->icon().isNull() )
		d->iconSize = action->icon().availableSizes().front();
}

ToolButton::~ToolButton()
{
}

QSize
ToolButton::iconSize() const
{
	return d->iconSize;
}

QSize
ToolButton::minimumSizeHint() const
{
	if( d->iconSize.isValid() )
		return d->iconSize;
	else
		return QSize();
}

QSize
ToolButton::sizeHint() const
{
	return minimumSizeHint();
}

void
ToolButton::setIconSize( const QSize & s )
{
	if( s.isValid() && d->iconSize != s )
	{
		d->iconSize = s;

		update();

		emit iconSizeChanged( d->iconSize );
	}
}

void
ToolButton::paintEvent( QPaintEvent * )
{
	if( !d->action->icon().isNull() )
	{
		const QPixmap pixmap = d->action->icon().pixmap( d->iconSize );
		const QRect r = contentsRect();
		const QSize s = pixmap.size();

		QPainter p( this );

		p.drawPixmap( r.left() + ( r.width() - s.width() ) / 2,
			r.top() + ( r.height() - s.height() ) / 2,
			s.width(), s.height(), pixmap );
	}
}

void
ToolButton::mousePressEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
		d->leftButtonPressed = true;
}

void
ToolButton::mouseReleaseEvent( QMouseEvent * )
{
	if( d->leftButtonPressed )
	{
		d->leftButtonPressed = false;
		d->action->trigger();
	}
}


//
// ToolBarLayout
//

class ToolBarLayout
	:	public QLayout
{
public:
	explicit ToolBarLayout( QWidget * parent );
	virtual ~ToolBarLayout();

	void setLeftArrow( NavigationArrow * a );
	void setRightArrow( NavigationArrow * a );

	void addButton( ToolButton * b );

	void setOrientation( Qt::Orientation o );

	virtual void addItem( QLayoutItem * item );
	virtual int count() const;
	virtual QLayoutItem * itemAt( int index ) const;
	virtual void setGeometry( const QRect & rect );
	virtual QLayoutItem * takeAt( int index );
	virtual bool hasHeightForWidth() const;

	virtual QSize minimumSize() const;
	virtual QSize sizeHint() const;

private:
	NavigationArrow * left;
	NavigationArrow * right;
	QVector< ToolButton* > buttons;
	Qt::Orientation orientation;
}; // class ToolBarLayout

ToolBarLayout::ToolBarLayout( QWidget * parent )
	:	QLayout( parent )
	,	left( 0 )
	,	right( 0 )
	,	orientation( Qt::Horizontal )
{
}

ToolBarLayout::~ToolBarLayout()
{
}

void
ToolBarLayout::setLeftArrow( NavigationArrow * a )
{
	left = a;
}

void
ToolBarLayout::setRightArrow( NavigationArrow * a )
{
	right = a;
}

void
ToolBarLayout::addButton( ToolButton * b )
{
	buttons.push_back( b );

	update();
}

void
ToolBarLayout::setOrientation( Qt::Orientation o )
{
	if( orientation != o )
	{
		orientation = o;

		update();
	}
}

void
ToolBarLayout::addItem( QLayoutItem * item )
{
	Q_UNUSED( item )
}

int
ToolBarLayout::count() const
{
	return 0;
}

QLayoutItem *
ToolBarLayout::itemAt( int index ) const
{
	Q_UNUSED( index )

	return 0;
}

void
ToolBarLayout::setGeometry( const QRect & rect )
{
	QLayout::setGeometry( rect );

	const QMargins m = contentsMargins();
	const QRect r = rect.adjusted( m.left(), m.top(), -m.right(), -m.bottom() );


}

QLayoutItem *
ToolBarLayout::takeAt( int index )
{
	Q_UNUSED( index )

	return 0;
}

bool
ToolBarLayout::hasHeightForWidth() const
{
	return false;
}

QSize
ToolBarLayout::minimumSize() const
{
	return QSize();
}

QSize
ToolBarLayout::sizeHint() const
{
	return QSize();
}


//
// ToolBarPrivate
//

class ToolBarPrivate {
public:
	explicit ToolBarPrivate( ToolBar * parent )
		:	q( parent )
		,	orientation( Qt::Horizontal )
		,	layout( 0 )
		,	left( 0 )
		,	right( 0 )
	{
	}

	void init();

	ToolBar * q;
	Qt::Orientation orientation;
	QSize iconSize;
	ToolBarLayout * layout;
	NavigationArrow * left;
	NavigationArrow * right;
}; // class ToolBarPrivate

void
ToolBarPrivate::init()
{
	q->setSizePolicy( QSizePolicy(
		QSizePolicy::Preferred, QSizePolicy::Fixed ) );

	layout = new ToolBarLayout( q );
	left = new NavigationArrow( NavigationArrow::Left, q );
	right = new NavigationArrow( NavigationArrow::Right, q );
	layout->setLeftArrow( left );
	layout->setRightArrow( right );
}


//
// ToolBar
//

ToolBar::ToolBar( QWidget * parent )
	:	QWidget( parent )
	,	d( new ToolBarPrivate( this ) )
{
	d->init();
}

ToolBar::~ToolBar()
{
}

void
ToolBar::setOrientation( Qt::Orientation orientation )
{
	if( d->orientation != orientation )
	{
		d->orientation = orientation;

		if( d->orientation == Qt::Vertical )
			setSizePolicy( QSizePolicy(
				QSizePolicy::Fixed, QSizePolicy::Preferred ) );
		else
			setSizePolicy( QSizePolicy(
				QSizePolicy::Preferred, QSizePolicy::Fixed ) );

		d->layout->setOrientation( d->orientation );

		emit orientationChanged( d->orientation );
	}
}

Qt::Orientation
ToolBar::orientation() const
{
	return d->orientation;
}

void
ToolBar::clear()
{
	QList< QAction* > actions = this->actions();

	for( int i = 0; i < actions.size(); ++i )
		removeAction( actions.at( i ) );
}

QAction *
ToolBar::addAction( const QIcon & icon )
{
	QAction * action = new QAction( icon, QString(), this );
	addAction( action );
	return action;
}

QAction *
ToolBar::addAction( const QIcon & icon,
	const QObject * receiver, const char * member )
{
	QAction * action = new QAction( icon, QString(), this );
	QObject::connect( action, SIGNAL( triggered( bool ) ), receiver, member );
	addAction( action );
	return action;
}

QRect
ToolBar::actionGeometry( QAction * action ) const
{
	return QRect();
}

QAction *
ToolBar::actionAt( const QPoint & p ) const
{
	return 0;
}

QSize
ToolBar::iconSize() const
{
	return d->iconSize;
}

QSize
ToolBar::minimumSizeHint() const
{
	const int width = FingerGeometry::width() * 2;
	const int height = FingerGeometry::height();

	return ( d->orientation == Qt::Horizontal ?
		QSize( width, height ) : QSize( height, width ) );
}

QSize
ToolBar::sizeHint() const
{
	return minimumSizeHint();
}

void
ToolBar::setIconSize( const QSize & iconSize )
{
	if( d->iconSize != iconSize )
	{
		if( iconSize.isValid() )
		{
			d->iconSize = iconSize;

			emit iconSizeChanged( d->iconSize );
		}
	}
}

void
ToolBar::actionEvent( QActionEvent * event )
{
	QAction * action = event->action();

	switch( event->type() )
	{
		case QEvent::ActionAdded :
		{
		}
			break;

		case QEvent::ActionChanged :
		{
		}
			break;

		case QEvent::ActionRemoved :
		{
		}
			break;

		default :
			break;
	}
}

} /* namespace QtMWidgets */
