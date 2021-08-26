
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
	{
		d->leftButtonPressed = true;

		e->accept();
	}
	else
		e->ignore();
}

void
ToolButton::mouseReleaseEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
	{
		if( d->leftButtonPressed )
		{
			d->leftButtonPressed = false;
			d->action->trigger();
		}

		e->accept();
	}
	else
		e->ignore();
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

	int spaceNeeded() const;

	using QLayout::indexOf;
	int indexOf( QAction * action ) const;

	void setLeftArrow( NavigationArrow * a );
	void setRightArrow( NavigationArrow * a );
	void addButton( ToolButton * b );

	Qt::Orientation orientation() const;
	void setOrientation( Qt::Orientation o );

	Qt::Alignment alignment() const;
	void setAlignment( Qt::Alignment align );

	void setOffset( int delta );
	void setIconSize( const QSize & s );

	void addItem( QLayoutItem * item ) override;
	int count() const override;
	QLayoutItem * itemAt( int index ) const override;
	void setGeometry( const QRect & rect ) override;
	QLayoutItem * takeAt( int index ) override;
	bool hasHeightForWidth() const override;

	QSize minimumSize() const override;
	QSize sizeHint() const override;

private:
	QWidgetItem * left;
	QWidgetItem * right;
	QList< QLayoutItem* > buttons;
	Qt::Orientation orient;
	Qt::Alignment align;
	int offset;
}; // class ToolBarLayout

ToolBarLayout::ToolBarLayout( QWidget * parent )
	:	QLayout( parent )
	,	left( 0 )
	,	right( 0 )
	,	orient( Qt::Horizontal )
	,	align( Qt::AlignLeft )
	,	offset( 0 )
{
}

ToolBarLayout::~ToolBarLayout()
{
}

int
ToolBarLayout::spaceNeeded() const
{
	QSize size( 0, 0 );

	foreach( QLayoutItem * item, buttons )
	{
		size += item->sizeHint();
		size += QSize( spacing(), spacing() );
	}

	if( orient == Qt::Horizontal )
		return size.width();
	else
		return size.height();
}

int
ToolBarLayout::indexOf( QAction * action ) const
{
	for( int i = 0; i < buttons.count(); ++i )
	{
		ToolButton * b = qobject_cast< ToolButton* >
			( buttons.at( i )->widget() );

		if( b && b->action() == action )
			return i + 1;
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

Qt::Orientation
ToolBarLayout::orientation() const
{
	return orient;
}

void
ToolBarLayout::setOrientation( Qt::Orientation o )
{
	if( orient != o )
	{
		orient = o;

		update();
	}
}

Qt::Alignment
ToolBarLayout::alignment() const
{
	return align;
}

void
ToolBarLayout::setAlignment( Qt::Alignment a )
{
	if( a & ( Qt::AlignLeft | Qt::AlignRight | Qt::AlignCenter
				| Qt::AlignTop | Qt::AlignBottom ) )
	{
		if( align != a )
		{
			align = a & ( Qt::AlignLeft | Qt::AlignRight | Qt::AlignCenter
				| Qt::AlignTop | Qt::AlignBottom );

			update();
		}
	}
}

void
ToolBarLayout::setOffset( int delta )
{
	offset += delta;

	update();
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

	update();
}

void
ToolBarLayout::addItem( QLayoutItem * item )
{
	buttons.append( item );

	update();
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

		QSize leftSize = left->sizeHint();
		const QSize rightSize = right->sizeHint();

		if( orient == Qt::Horizontal )
			y = r.y() + ( r.height() - leftSize.height() ) / 2;
		else
			x = r.x() + ( r.width() - leftSize.width() ) / 2;

		// Set left arrow geometry.
		bool leftArrowShown = false;

		if( offset > 0 )
		{
			leftArrowShown = true;

			left->setGeometry( QRect( x, y, leftSize.width(), leftSize.height() ) );

			if( orient == Qt::Horizontal )
				x += leftSize.width() + spacing();
			else
				y += leftSize.height() + spacing();
		}
		else
		{
			left->setGeometry( QRect( 0, 0, 0, 0 ) );
			leftSize = QSize( 0, 0 );

			x = r.x();
			y = r.y();
		}

		const QSize buttonSize = buttons.at( 0 )->sizeHint();
		const int dim = ( orient == Qt::Horizontal ?
			buttonSize.width() : buttonSize.height() ) + spacing();
		int i = 0;
		int tmpOffset = offset;

		// Hide buttons that left on left arrow.
		while( tmpOffset >= dim )
		{
			buttons.at( i )->setGeometry( QRect( 0, 0, 0, 0 ) );

			++i;
			tmpOffset -= dim;
		}

		int stop = 0;

		// Show visible buttons.
		if( orient == Qt::Horizontal )
		{
			x += offset % dim;
			tmpOffset = x;
			stop = ( r.width() - leftSize.width() ) / dim * dim;
			y = r.y() + ( r.height() - buttonSize.height() ) / 2;
		}
		else
		{
			y += offset % dim;
			tmpOffset = y;
			stop = ( r.height() - leftSize.height() ) / dim * dim;
			x = r.x() + ( r.width() - buttonSize.width() ) / 2;
		}

		int space = spaceNeeded();

		if( space < ( orient == Qt::Horizontal ? r.width() : r.height() ) )
		{
			if( ( ( align & Qt::AlignRight ) && orient == Qt::Horizontal )
				| ( ( align & Qt::AlignBottom ) && orient == Qt::Vertical  ) )
			{
				if( orient == Qt::Horizontal )
					x += r.width() - space;
				else
					y += r.height() - space;
			}
			else if( align & Qt::AlignCenter )
			{
				if( orient == Qt::Horizontal )
					x += ( r.width() - space ) / 2;
				else
					y += ( r.height() - space ) / 2;
			}
		}

		space = 0;

		while( tmpOffset < stop && i < buttons.size() )
		{
			buttons.at( i )->setGeometry( QRect( x, y,
				buttonSize.width(), buttonSize.height() ) );

			if( orient == Qt::Horizontal )
			{
				x += dim;
				space = x;
			}
			else
			{
				y += dim;
				space = y;
			}

			tmpOffset += dim;
			++i;
		}

		// Set right arrow geometry.
		bool rightArrowShown = false;

		if( i < buttons.size() )
		{
			if( orient == Qt::Horizontal )
			{
				x = r.width() - rightSize.width() + r.x();
				y = r.y() + ( r.height() - rightSize.height() ) / 2;

				if( tmpOffset > r.width() )
					buttons.at( i - 1 )->setGeometry( QRect( 0, 0, 0, 0 ) );
			}
			else
			{
				x = r.x() + ( r.width() - rightSize.width() ) / 2;
				y = r.height() - rightSize.height() + r.y();

				if( tmpOffset > r.height() )
					buttons.at( i - 1 )->setGeometry( QRect( 0, 0, 0, 0 ) );
			}

			right->setGeometry( QRect( x, y,
				rightSize.width(), rightSize.height() ) );

			right->widget()->raise();

			rightArrowShown = true;
		}
		else
		{
			right->setGeometry( QRect( 0, 0, 0, 0 ) );
		}

		// Hide buttons that right on right arrow.
		if( i < buttons.size() )
		{
			for( ; i < buttons.size(); ++i )
				buttons.at( i )->setGeometry( QRect( 0, 0, 0, 0 ) );
		}
		else if( offset > 0 && leftArrowShown &&
			!rightArrowShown &&
			space + dim / 2 <= ( orient == Qt::Horizontal ? r.width() : r.height() ) )
		{
			offset = 0;
			setGeometry( rect );
		}

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

	if( orient == Qt::Horizontal )
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
		,	layout( 0 )
		,	left( 0 )
		,	right( 0 )
	{
	}

	void init();

	ToolBar * q;
	QSize iconSize;
	ToolBarLayout * layout;
	NavigationArrow * left;
	NavigationArrow * right;
}; // class ToolBarPrivate

void
ToolBarPrivate::init()
{
	layout = new ToolBarLayout( q );

	if( layout->orientation() == Qt::Horizontal )
		left = new NavigationArrow( NavigationArrow::Left, q );
	else
		left = new NavigationArrow( NavigationArrow::Top, q );

	if( layout->orientation() == Qt::Horizontal )
		right = new NavigationArrow( NavigationArrow::Right, q );
	else
		right = new NavigationArrow( NavigationArrow::Bottom, q );

	layout->setLeftArrow( left );
	layout->setRightArrow( right );

	q->setSizePolicy( QSizePolicy(
		QSizePolicy::Preferred, QSizePolicy::Fixed ) );

	q->setBackgroundRole( QPalette::Base );
	q->setAutoFillBackground( true );
	q->setIconSize( QSize( 32, 32 ) );
}


//
// ToolBar
//

ToolBar::ToolBar( QWidget * parent )
	:	QWidget( parent )
	,	d( new ToolBarPrivate( this ) )
{
	d->init();

	connect( d->left, &NavigationArrow::clicked,
		this, &ToolBar::_q_leftArrowClicked );

	connect( d->right, &NavigationArrow::clicked,
		this, &ToolBar::_q_rightArrowClicked );
}

ToolBar::~ToolBar()
{
}

void
ToolBar::setOrientation( Qt::Orientation orientation )
{
	if( d->layout->orientation() != orientation )
	{
		d->left->deleteLater();
		d->right->deleteLater();

		if( orientation == Qt::Vertical )
		{
			d->left = new NavigationArrow( NavigationArrow::Top, this );
			d->right = new NavigationArrow( NavigationArrow::Bottom, this );

			setSizePolicy( QSizePolicy(
				QSizePolicy::Fixed, QSizePolicy::Preferred ) );
		}
		else
		{
			d->left = new NavigationArrow( NavigationArrow::Left, this );
			d->right = new NavigationArrow( NavigationArrow::Right, this );

			setSizePolicy( QSizePolicy(
				QSizePolicy::Preferred, QSizePolicy::Fixed ) );
		}

		d->layout->setLeftArrow( d->left );
		d->layout->setRightArrow( d->right );

		connect( d->left, &NavigationArrow::clicked,
			this, &ToolBar::_q_leftArrowClicked );

		connect( d->right, &NavigationArrow::clicked,
			this, &ToolBar::_q_rightArrowClicked );

		d->layout->setOrientation( orientation );

		updateGeometry();

		emit orientationChanged( orientation );
	}
}

Qt::Orientation
ToolBar::orientation() const
{
	return d->layout->orientation();
}

Qt::Alignment
ToolBar::alignment() const
{
	return d->layout->alignment();
}

void
ToolBar::setAlignment( Qt::Alignment align )
{
	d->layout->setAlignment( align );
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
	QObject::connect( action, SIGNAL( triggered(bool) ), receiver, member );
	addAction( action );
	return action;
}

QRect
ToolBar::actionGeometry( QAction * action ) const
{
	int index = d->layout->indexOf( action );

	if( index == -1 )
		return QRect();

	return d->layout->itemAt( index )->widget()->geometry();
}

QAction *
ToolBar::actionAt( const QPoint & p ) const
{
	QWidget * widget = childAt( p );

	int index = d->layout->indexOf( widget );

	if( index == -1 || index == 0 || index == d->layout->count() + 1 )
		return 0;

	QLayoutItem * item = d->layout->itemAt( index );

	ToolButton * b = 0;

	if( item )
		b = static_cast< ToolButton* > ( item->widget() );

	if( b )
		return b->action();
	else
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
			b->show();
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

void
ToolBar::_q_leftArrowClicked()
{
	int delta = 0;

	if( d->layout->orientation() == Qt::Horizontal )
		delta = - d->iconSize.width() - d->layout->spacing();
	else
		delta = - d->iconSize.height() - d->layout->spacing();

	d->layout->setOffset( delta );

	update();
}

void
ToolBar::_q_rightArrowClicked()
{
	int delta = 0;

	if( d->layout->orientation() == Qt::Horizontal )
		delta = d->iconSize.width() + d->layout->spacing();
	else
		delta = d->iconSize.height() + d->layout->spacing();

	d->layout->setOffset( delta );

	update();
}

} /* namespace QtMWidgets */
