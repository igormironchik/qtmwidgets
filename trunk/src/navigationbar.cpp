
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

// Qt include.
#include <QStackedWidget>
#include <QVector>
#include <QMap>
#include <QSharedPointer>


namespace QtMWidgets {

//
// NavigationItem
//

class NavigationItem {
public:

	NavigationItem()
		:	index( -1 )
		,	parent( 0 )
	{
	}

	NavigationItem( const NavigationItem & other )
		:	title( other.title )
		,	index( other.index )
		,	children( other.children )
		,	parent( other.parent )
	{
	}

	NavigationItem & operator = ( const NavigationItem & other )
	{
		if( this != &other )
		{
			title = other.title;
			index = other.index;
			children = other.children;
			parent = other.parent;
		}

		return *this;
	}

	QString title;
	int index;
	QVector< QSharedPointer< NavigationItem > > children;
	QVector< int > childrenIndexes;
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
		,	root( 0 )
	{
	}

	void init();
	void removeWidget( int index );

	NavigationBar * q;
	QStackedWidget * stack;
	QSharedPointer< NavigationItem > root;
	QMap< int, QSharedPointer< NavigationItem > > itemsMap;
}; // class NavigationBarPrivate

void
NavigationBarPrivate::init()
{
	stack = new QStackedWidget( q );
}

void
NavigationBarPrivate::removeWidget( int index )
{
	QSharedPointer< NavigationItem > item = itemsMap[ index ];

	itemsMap.remove( index );
	stack->removeWidget( stack->widget( index ) );

	foreach( int childIndex, item->childrenIndexes )
		removeWidget( childIndex );

	if( item->parent )
	{
		item->parent->children.removeAt(
			item->parent->children.indexOf( item ) );
		item->parent->childrenIndexes.removeAt(
			item->parent->childrenIndexes.indexOf( index ) );
	}
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

	if( d->root )
	{
		foreach( int index, d->itemsMap.keys() )
			d->stack->removeWidget( d->stack->widget( index ) );

		d->itemsMap.clear();
	}

	d->root = QSharedPointer< NavigationItem > ( new NavigationItem );
	d->itemsMap[ index ] = d->root;

	d->root->index = index;
	d->root->title = title;

	d->stack->setCurrentIndex( index );

	return index;
}

int
NavigationBar::addWidget( int parentIndex, const QString & title,
	QWidget * widget )
{
	const int index = d->stack->addWidget( widget );

	if( d->itemsMap.contains( parentIndex ) )
	{
		QSharedPointer< NavigationItem > parent = d->itemsMap[ parentIndex ];

		QSharedPointer< NavigationItem > item( new NavigationItem );
		item->index = index;
		item->title = title;
		item->parent = parent.data();

		parent->children.append( item );
		parent->childrenIndexes.append( index );
		d->itemsMap[ index ] = item;
	}

	return index;
}

void
NavigationBar::removeWidget( QWidget * widget )
{
	const int index = d->stack->indexOf( widget );

	if( index != -1 )
		d->removeWidget( index );
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
	return QSize();
}

QSize
NavigationBar::sizeHint() const
{
	return QSize();
}

void
NavigationBar::showScreen( int index )
{

}

void
NavigationBar::showPreviousScreen()
{

}

void
NavigationBar::showNextScreen()
{

}

} /* namespace QtMWidgets */
