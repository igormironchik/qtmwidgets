
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
#include <QList>


namespace QtMWidgets {

//
// ToolButtonPrivate
//

class ToolButtonPrivate {
public:
	ToolButtonPrivate( QAction * a, ToolButton * parent )
		:	q( parent )
		,	action( a )
		,	iconSize( QSize( 32, 32 ) )
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
}

ToolButton::~ToolButton()
{
}

QSize
ToolButton::iconSize() const
{
	return d->iconSize;
}

QAction *
ToolButton::action() const
{
	return d->action;
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

	int indexOf( QAction * action ) const;

	void setLeftArrow( NavigationArrow * a );
	void setRightArrow( NavigationArrow * a );
	void addButton( ToolButton * b );

	void setOrientation( Qt::Orientation o );
	void setOffset( int o );
	void setIconSize( const QSize & s );

	virtual void addItem( QLayoutItem * item );
	virtual int count() const;
	virtual QLayoutItem * itemAt( int index ) const;
	virtual void setGeometry( const QRect & rect );
	virtual QLayoutItem * takeAt( int index );
	virtual bool hasHeightForWidth() const;

	virtual QSize minimumSize() const;
	virtual QSize sizeHint() const;

private:
	QWidgetItem * left;
	QWidgetItem * right;
	QList< QLayoutItem* > buttons;
	Qt::Orientation orientation;
	int offset;
}; // class ToolBarLayout

ToolBarLayout::ToolBarLayout( QWidget * parent )
	:	QLayout( parent )
	,	left( 0 )
	,	right( 0 )
	,	orientation( Qt::Horizontal )
	,	offset( 0 )
{
}

ToolBarLayout::~ToolBarLayout()
{
}

int
ToolBarLayout::indexOf( QAction * action ) const
{
	for( int i = 0; i < buttons.count(); ++i )
	{
		ToolButton * b = qobject_cast< ToolButton* >
			( buttons.at( i )->widget() );

		if( b && b->action() == action )
			return i;
	}

	return -1;
}

void
ToolBarLayout::setLeftArrow( NavigationArrow * a )
{
	if( left )
		delete left;

	left = new QWidgetItem( a );
}

void
ToolBarLayout::setRightArrow( NavigationArrow * a )
{
	if( right )
		delete right;

	right = new QWidgetItem( a );
}

void
ToolBarLayout::addButton( ToolButton * b )
{
	buttons.append( new QWidgetItem( b ) );

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
ToolBarLayout::setOffset( int o )
{
	if( offset != o )
	{
		offset = o;

		update();
	}
}

void
ToolBarLayout::setIconSize( const QSize & s )
{
	foreach( QLayoutItem * item, buttons )
	{
		ToolButton * b = qobject_cast< ToolButton* >
			( item->widget() );

		if( b )
			b->setIconSize( s );
	}
}

void
ToolBarLayout::addItem( QLayoutItem * item )
{
	buttons.append( item );
}

int
ToolBarLayout::count() const
{
	return buttons.size() + 2;
}

QLayoutItem *
ToolBarLayout::itemAt( int index ) const
{
	if( index >= 0 && index < buttons.size() + 2 )
	{
		if( index == 0 )
			return left;
		else if( index == buttons.size() + 1 )
			return right;
		else
			return buttons.at( index - 1 );
	}
	else
		return 0;
}

void
ToolBarLayout::setGeometry( const QRect & rect )
{
	QLayout::setGeometry( rect );

	const QMargins m = contentsMargins();
	const QRect r = rect.adjusted( m.left(), m.top(), -m.right(), -m.bottom() );

	int x = r.x();
	int y = r.y();

	if( !buttons.isEmpty() && left && right )
	{
		if( offset < 0 )
			offset = 0;

		const QSize leftSize = left->sizeHint();
		const QSize rightSize = right->sizeHint();

		if( orientation == Qt::Horizontal )
			y = r.y() + ( r.height() - leftSize.height() ) / 2;
		else
			x = r.x() + ( r.width() - leftSize.width() ) / 2;

		// Set left arrow geometry.
		if( offset > 0 )
		{
			left->setGeometry( QRect( x, y, leftSize.width(), leftSize.height() ) );

			if( orientation == Qt::Horizontal )
				x += leftSize.width();
			else
				y += leftSize.height();
		}
		else
			left->setGeometry( QRect( 0, 0, 0, 0 ) );

		const QSize buttonSize = buttons.at( 0 )->sizeHint();
		const int dim = ( orientation == Qt::Horizontal ?
			buttonSize.width() : buttonSize.height() ) + spacing();
		int i = 0;
		int tmpOffset = offset;

		// Hide buttons that left on left arrow.
		while( tmpOffset > dim )
		{
			if( orientation == Qt::Horizontal )
			{
				if( ( buttons.size() - i ) * dim <
					r.width() - leftSize.width() - rightSize.width() )
						break;
			}
			else
			{
				if( ( buttons.size() - i ) * dim <
					r.height() - leftSize.height() - rightSize.height() )
						break;
			}

			const QSize s = buttons.at( i )->sizeHint();

			buttons.at( i )->setGeometry( QRect( 0, 0, 0, 0 ) );

			++i;
			tmpOffset -= dim;
		}

		int stop = 0;

		// Show visible buttons.
		if( orientation == Qt::Horizontal )
		{
			x = offset % dim + m.left();
			tmpOffset = x;
			stop = ( r.width() -
				( left->widget()->isVisible() ? leftSize.width() : 0 ) ) / dim * dim;
			y = r.y() + ( r.height() - buttonSize.height() ) / 2;
		}
		else
		{
			y = offset % dim + m.top();
			tmpOffset = y;
			stop = ( r.height() -
				( left->widget()->isVisible() ? leftSize.height() : 0 ) ) / dim * dim;
			x = r.x() + ( r.width() - buttonSize.width() ) / 2;
		}

		while( tmpOffset < stop && i < buttons.size() )
		{
			buttons.at( i )->setGeometry( QRect( x, y,
				buttonSize.width(), buttonSize.height() ) );

			if( orientation == Qt::Horizontal )
				x += dim;
			else
				y += dim;

			tmpOffset += dim;
			++i;
		}

		// Set right arrow geometry.
		if( i < buttons.size() )
		{
			if( orientation == Qt::Horizontal )
			{
				x = r.width() - rightSize.width() + r.x();
				y = r.y() + ( r.height() - rightSize.height() ) / 2;
			}
			else
			{
				x = r.x() + ( r.width() - rightSize.width() ) / 2;
				y = r.height() - rightSize.height() + r.y();
			}

			right->setGeometry( QRect( x, y,
				rightSize.width(), rightSize.height() ) );

			right->widget()->raise();
		}
		else
			right->setGeometry( QRect( 0, 0, 0, 0 ) );

		// Hide buttons that right on right arrow.
		for( ; i < buttons.size(); ++i )
		{
			const QSize s = buttons.at( i )->sizeHint();
			buttons.at( i )->setGeometry( QRect( 0, 0, 0, 0 ) );
		}

		if( left->widget()->isVisible() )
			left->widget()->raise();
	}
}

QLayoutItem *
ToolBarLayout::takeAt( int index )
{
	if( index >= 0 && index < buttons.size() + 2 )
	{
		if( index == 0 )
		{
			QLayoutItem * item = left;
			left = 0;
			return item;
		}
		else if( index == buttons.size() + 1 )
		{
			QLayoutItem * item = right;
			right = 0;
			return item;
		}
		else
		{
			QLayoutItem * item = buttons.takeAt( index - 1 );
			return item;
		}
	}
	else
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
	int width = 0;
	int height = 0;

	const QSize arrowSize = left->sizeHint();

	QSize buttonSize;

	if( !buttons.isEmpty() )
		buttonSize = buttons.at( 0 )->sizeHint();
	else
		buttonSize = QSize( FingerGeometry::width(),
			FingerGeometry::height() );

	const QMargins m = contentsMargins();

	if( orientation == Qt::Horizontal )
	{
		width = arrowSize.width() * 2 + buttonSize.width() * 2 + m.left() +
			m.right();
		height = qMax( arrowSize.height(), buttonSize.height() ) + m.top() +
			m.bottom();
	}
	else
	{
		height = arrowSize.height() * 2 + buttonSize.height() * 2 + m.top() +
			m.bottom();
		width = qMax( arrowSize.width(), buttonSize.width() ) + m.left() +
			m.right();
	}

	return QSize( width, height );
}

QSize
ToolBarLayout::sizeHint() const
{
	return minimumSize();
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

	q->setBackgroundRole( QPalette::Base );
	q->setAutoFillBackground( true );

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
	return d->layout->sizeHint();
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

			d->layout->setIconSize( d->iconSize );

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
			ToolButton * b = new ToolButton( action, this );
			b->setIconSize( d->iconSize );
			d->layout->addButton( b );
		}
			break;

		case QEvent::ActionChanged :
			d->layout->invalidate();
			break;

		case QEvent::ActionRemoved :
		{
			int index = d->layout->indexOf( action );
			if( index != -1 )
				delete d->layout->takeAt( index );
		}
			break;

		default :
			break;
	}
}

} /* namespace QtMWidgets */
