
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
#include "navigationbar.hpp"
#include "navigationbutton.hpp"
#include "textlabel.hpp"
#include "fingergeometry.hpp"

// Qt include.
#include <QStackedWidget>
#include <QVector>
#include <QMap>
#include <QSharedPointer>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QStack>
#include <QHideEvent>


namespace QtMWidgets {

//
// NavigationItem
//

class NavigationItem {
public:

	NavigationItem()
		:	self( nullptr )
		,	parent( 0 )
	{
	}

	NavigationItem( const NavigationItem & other )
		:	title( other.title )
		,	self( other.self )
		,	children( other.children )
		,	parent( other.parent )
	{
	}

	NavigationItem & operator = ( const NavigationItem & other )
	{
		if( this != &other )
		{
			title = other.title;
			self = other.self;
			children = other.children;
			parent = other.parent;
		}

		return *this;
	}

	QString title;
	QWidget * self;
	QVector< QSharedPointer< NavigationItem > > children;
	NavigationItem * parent;
}; // class NavigationItem


//
// NavigationBarPrivate
//

class NavigationBarPrivate {
public:
	NavigationBarPrivate( NavigationBar * parent )
		:	q( parent )
		,	stack( 0 )
		,	left( 0 )
		,	right( 0 )
		,	title( 0 )
		,	grid( 0 )
	{
	}

	void init();
	void removeWidget( QWidget * w );

	NavigationBar * q;
	QStackedWidget * stack;
	QList< QSharedPointer< NavigationItem > > rootItems;
	QMap< QWidget *, QSharedPointer< NavigationItem > > itemsMap;
	NavigationButton * left;
	NavigationButton * right;
	TextLabel * title;
	QGridLayout * grid;
	QStack< int > backStack;
	QStack< int > forwardStack;
}; // class NavigationBarPrivate

void
NavigationBarPrivate::init()
{
	q->setBackgroundRole( QPalette::Base );
	q->setAutoFillBackground( true );

	stack = new QStackedWidget( q );
	stack->setFrameStyle( QFrame::NoFrame | QFrame::Plain );

	left = new NavigationButton( NavigationButton::Left, q );
	right = new NavigationButton( NavigationButton::Right, q );
	title = new TextLabel( q );
	title->setColor( q->palette().color( QPalette::Highlight ) );

	QObject::connect( left, &NavigationButton::clicked,
		q, &NavigationBar::showPreviousScreen );
	QObject::connect( right, &NavigationButton::clicked,
		q, &NavigationBar::showNextScreen );

	QTextOption opt = title->textOption();
	opt.setAlignment( Qt::AlignCenter );
	opt.setWrapMode( QTextOption::NoWrap );

	title->setTextOption( opt );

	QFont f = title->font();
	f.setBold( true );
	f.setPointSize( f.pointSize() + 1 );

	title->setFont( f );

	QVBoxLayout * layout = new QVBoxLayout( q );
	layout->setSpacing( 0 );
	layout->setContentsMargins( 3, 3, 3, 3 );

	grid = new QGridLayout;
	grid->addWidget( left, 0, 0 );
	grid->addWidget( title, 0, 1 );
	grid->addWidget( right, 0, 2 );
	grid->setRowMinimumHeight( 0, FingerGeometry::height() );

	left->hide();
	right->hide();

	layout->addLayout( grid );
	layout->addWidget( stack );
}

void
NavigationBarPrivate::removeWidget( QWidget * w )
{
	QSharedPointer< NavigationItem > item = itemsMap[ w ];

	itemsMap.remove( w );
	stack->removeWidget( w );

	const auto tmp = item->children;

	for( const auto & c : tmp )
		removeWidget( c->self );

	if( item->parent )
		item->parent->children.removeAt(
			item->parent->children.indexOf( item ) );
}


//
// NavigationBar
//

NavigationBar::NavigationBar( QWidget * parent )
	:	QWidget( parent )
	,	d( new NavigationBarPrivate( this ) )
{
	d->init();
}

NavigationBar::~NavigationBar()
{
}

int
NavigationBar::setMainWidget( const QString & title,
	QWidget * widget )
{
	const int index = d->stack->addWidget( widget );

	d->rootItems.append(
		QSharedPointer< NavigationItem > ( new NavigationItem ) );
	d->itemsMap[ widget ] = d->rootItems.back();

	d->rootItems.back()->title = title;
	d->rootItems.back()->self = widget;

	d->stack->setCurrentIndex( index );
	d->title->setText( title );

	return index;
}

int
NavigationBar::addWidget( QWidget * parent, const QString & title,
	QWidget * widget )
{
	int index = -1;

	if( d->itemsMap.contains( parent ) )
	{
		index = d->stack->addWidget( widget );

		QSharedPointer< NavigationItem > pp = d->itemsMap[ parent ];

		QSharedPointer< NavigationItem > item( new NavigationItem );
		item->title = title;
		item->parent = pp.data();
		item->self = widget;

		pp->children.append( item );
		d->itemsMap[ widget ] = item;
	}

	return index;
}

void
NavigationBar::removeWidget( QWidget * widget )
{
	const int index = d->stack->indexOf( widget );

	if( index != -1 )
	{
		d->removeWidget( widget );

		d->backStack.clear();
		d->forwardStack.clear();

		d->left->hide();
		d->right->hide();
	}
}

int
NavigationBar::currentIndex() const
{
	return d->stack->currentIndex();
}

QWidget *
NavigationBar::currentWidget() const
{
	return d->stack->currentWidget();
}

int
NavigationBar::indexOf( QWidget * widget ) const
{
	return d->stack->indexOf( widget );
}

QWidget *
NavigationBar::widget( int index ) const
{
	return d->stack->widget( index );
}

QSize
NavigationBar::minimumSizeHint() const
{
	return QSize( FingerGeometry::width(), FingerGeometry::height() );
}

QSize
NavigationBar::sizeHint() const
{
	return minimumSizeHint();
}

void
NavigationBar::showScreen( QWidget * s )
{
	if( d->itemsMap.contains( s ) && currentIndex() != -1 )
	{
		QSharedPointer< NavigationItem > nextItem = d->itemsMap[ s ];
		QSharedPointer< NavigationItem > currentItem =
			d->itemsMap[ d->stack->widget( currentIndex() ) ];

		if( d->backStack.isEmpty() || d->backStack.top() != currentIndex() )
		{
			d->backStack.push( currentIndex() );

			d->left->setText( currentItem->title );
			d->left->show();
		}

		d->forwardStack.clear();
		d->right->hide();

		d->title->setText( nextItem->title );

		d->stack->setCurrentWidget( nextItem->self );
	}
}

void
NavigationBar::showPreviousScreen()
{
	if( !d->backStack.isEmpty() )
	{
		const int prevIndex = d->backStack.pop();
		QSharedPointer< NavigationItem > prevItem =
			d->itemsMap[ d->stack->widget( prevIndex ) ];
		QSharedPointer< NavigationItem > currentItem =
			d->itemsMap[ d->stack->widget( currentIndex() ) ];

		if( d->forwardStack.isEmpty() || d->forwardStack.top() != currentIndex() )
		{
			d->forwardStack.push( currentIndex() );

			d->right->setText( currentItem->title );
			d->right->show();
		}

		d->title->setText( prevItem->title );

		d->stack->setCurrentWidget( prevItem->self );

		if( !d->backStack.isEmpty() )
		{
			QSharedPointer< NavigationItem > backItem =
				d->itemsMap[ d->stack->widget( d->backStack.top() ) ];

			d->left->setText( backItem->title );
		}
		else
			d->left->hide();
	}
}

void
NavigationBar::showNextScreen()
{
	if( !d->forwardStack.isEmpty() )
	{
		const int nextIndex = d->forwardStack.pop();
		QSharedPointer< NavigationItem > nextItem =
			d->itemsMap[ d->stack->widget( nextIndex ) ];
		QSharedPointer< NavigationItem > currentItem =
			d->itemsMap[ d->stack->widget( currentIndex() ) ];

		d->backStack.push( currentIndex() );

		d->title->setText( nextItem->title );

		d->left->setText( currentItem->title );
		d->left->show();

		d->stack->setCurrentWidget( nextItem->self );

		if( !d->forwardStack.isEmpty() )
		{
			QSharedPointer< NavigationItem > item =
				d->itemsMap[ d->stack->widget( d->forwardStack.top() ) ];

			d->right->setText( item->title );
		}
		else
			d->right->hide();
	}
}

void
NavigationBar::resizeEvent( QResizeEvent * e )
{
	const int width = e->size().width() / 3;

	d->grid->setColumnMinimumWidth( 0, width );
	d->grid->setColumnMinimumWidth( 1, width );
	d->grid->setColumnMinimumWidth( 2, width );

	e->accept();
}

void
NavigationBar::hideEvent( QHideEvent * e )
{
	if( !isVisible() )
	{
		d->backStack.clear();
		d->forwardStack.clear();

		d->left->hide();
		d->right->hide();
	}

	e->accept();
}

} /* namespace QtMWidgets */
