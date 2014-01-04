
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
#include "picker.hpp"

// Qt include.
#include <QStandardItemModel>
#include <QEvent>

#ifndef QT_NO_ACCESSIBILITY
#include <QAccessible>
#endif


namespace QtMWidgets {

//
// PickerPrivate
//

class PickerPrivate {
public:
	PickerPrivate( Picker * parent )
		:	q( parent )
		,	model( 0 )
		,	insertPolicy( Picker::InsertAtBottom )
		,	modelColumn( 0 )
		,	indexBeforeChange( -1 )
		,	inserting( false )
		,	maxCount( INT_MAX )
	{}

	void init();
	void setCurrentIndex( const QModelIndex & mi );
	QString itemText( const QModelIndex & index ) const;

	Picker * q;
	QAbstractItemModel * model;
	Picker::InsertPolicy insertPolicy;
	int modelColumn;
	QPersistentModelIndex currentIndex;
	QPersistentModelIndex root;
	int indexBeforeChange;
	bool inserting;
	int maxCount;
}; // class PickerPrivate

void
PickerPrivate::init()
{
	q->setSizePolicy( QSizePolicy( QSizePolicy::Preferred,
		QSizePolicy::Fixed ) );

	q->setModel( new QStandardItemModel( 0, 1, q ) );
}

void
PickerPrivate::setCurrentIndex( const QModelIndex & mi )
{
	QModelIndex normalized;

	if( mi.column() != modelColumn )
		normalized = model->index( mi.row(), modelColumn, mi.parent() );

	if( !normalized.isValid() )
		normalized = mi;    // Fallback to passed index.

	bool indexChanged = ( normalized != currentIndex );

	if( indexChanged )
	{
		currentIndex = QPersistentModelIndex( normalized );
		q->update();
		q->_q_emitCurrentIndexChanged( currentIndex );
	}
}

QString
PickerPrivate::itemText( const QModelIndex & index ) const
{
	return index.isValid() ? model->data( index, Qt::DisplayRole ).toString() :
		QString();
}


//
// Picker
//

Picker::Picker( QWidget * parent, Qt::WindowFlags f )
	:	QWidget( parent, f )
	,	d( new PickerPrivate( this ) )
{
	d->init();
}

Picker::~Picker()
{
	try {
		disconnect( d->model, SIGNAL( destroyed() ),
			this, SLOT( _q_modelDestroyed() ) );
	}
	catch( ... )
	{
		; // objects can't throw in destructor
	}
}

int
Picker::count() const
{
	return d->model->rowCount( d->root );
}

void
Picker::setMaxCount( int max )
{
	if( max < 0 )
	{
		qWarning( "QtMWidgets::Picker::setMaxCount: Invalid count (%d) must be >= 0",
			max );
		return;
	}

	if( max < count() )
		d->model->removeRows( max, count() - max, d->root );

	d->maxCount = max;
}

int
Picker::maxCount() const
{
	return d->maxCount;
}

int
Picker::findData( const QVariant & data, int role, Qt::MatchFlags flags ) const
{
	QModelIndexList result;

	QModelIndex start = d->model->index( 0, d->modelColumn, d->root );

	result = d->model->match( start, role, data, 1, flags );

	if( result.isEmpty() )
		return -1;

	return result.first().row();
}

Picker::InsertPolicy
Picker::insertPolicy() const
{
	return d->insertPolicy;
}

void
Picker::setInsertPolicy( InsertPolicy policy )
{
	d->insertPolicy = policy;
}

QAbstractItemModel *
Picker::model() const
{
	return d->model;
}

void
Picker::setModel( QAbstractItemModel * model )
{
	if( !model )
	{
		qWarning( "QtMWidgets::Picker::setModel: cannot set a 0 model" );
		return;
	}

	if( d->model )
	{
		disconnect( d->model, SIGNAL( dataChanged( QModelIndex, QModelIndex ) ),
			this, SLOT( _q_dataChanged( QModelIndex, QModelIndex ) ) );
		disconnect( d->model, SIGNAL( rowsAboutToBeInserted( QModelIndex, int, int ) ),
			this, SLOT( _q_updateIndexBeforeChange() ) );
		disconnect( d->model, SIGNAL( rowsInserted( QModelIndex, int, int ) ),
			this, SLOT( _q_rowsInserted( QModelIndex, int, int ) ) );
		disconnect( d->model, SIGNAL( rowsAboutToBeRemoved( QModelIndex, int, int ) ),
			this, SLOT( _q_updateIndexBeforeChange() ) );
		disconnect( d->model, SIGNAL( rowsRemoved( QModelIndex, int, int ) ),
			this, SLOT( _q_rowsRemoved( QModelIndex, int, int ) ) );
		disconnect( d->model, SIGNAL( destroyed() ),
			this, SLOT( _q_modelDestroyed() ) );
		disconnect( d->model, SIGNAL( modelAboutToBeReset() ),
			this, SLOT( _q_updateIndexBeforeChange() ) );
		disconnect( d->model, SIGNAL( modelReset() ),
			this, SLOT( _q_modelReset() ) );

		if( d->model->QObject::parent() == this )
			delete d->model;
	}

	d->model = model;

	connect( model, SIGNAL( dataChanged( QModelIndex, QModelIndex ) ),
		this, SLOT( _q_dataChanged( QModelIndex, QModelIndex ) ) );
	connect( model, SIGNAL( rowsAboutToBeInserted( QModelIndex, int, int ) ),
		this, SLOT( _q_updateIndexBeforeChange() ) );
	connect( model, SIGNAL( rowsInserted( QModelIndex, int, int ) ),
		this, SLOT( _q_rowsInserted( QModelIndex, int, int ) ) );
	connect( model, SIGNAL( rowsAboutToBeRemoved( QModelIndex, int, int ) ),
		this, SLOT( _q_updateIndexBeforeChange() ) );
	connect( model, SIGNAL( rowsRemoved( QModelIndex, int, int ) ),
		this, SLOT( _q_rowsRemoved( QModelIndex, int, int ) ) );
	connect( model, SIGNAL( destroyed() ),
		this, SLOT( _q_modelDestroyed() ) );
	connect( model, SIGNAL( modelAboutToBeReset() ),
		this, SLOT( _q_updateIndexBeforeChange() ) );
	connect( model, SIGNAL( modelReset() ),
		this, SLOT( _q_modelReset() ) );

	bool currentReset = false;

	if( count() )
	{
		for( int pos = 0; pos < count(); ++pos )
		{
			if( d->model->index( pos, d->modelColumn, d->root ).flags() & Qt::ItemIsEnabled )
			{
				setCurrentIndex( pos );
				currentReset = true;
				break;
			}
		}
	}

	if( !currentReset )
		setCurrentIndex( -1 );
}

QModelIndex
Picker::rootModelIndex() const
{
	return QModelIndex( d->root );
}

void
Picker::setRootModelIndex( const QModelIndex & index )
{
	d->root = QPersistentModelIndex( index );
	update();
}

int
Picker::modelColumn() const
{
	return d->modelColumn;
}

void
Picker::setModelColumn( int visibleColumn )
{
	d->modelColumn = visibleColumn;

	setCurrentIndex( currentIndex() ); //update the text to the text of the new column;
}

int
Picker::currentIndex() const
{
	return d->currentIndex.row();
}

QString
Picker::currentText() const
{
	if( d->currentIndex.isValid() )
		return d->itemText( d->currentIndex );
	else
		return QString();
}

QVariant
Picker::currentData( int role ) const
{
	return d->currentIndex.data( role );
}

QString
Picker::itemText( int index ) const
{
	QModelIndex mi = d->model->index( index, d->modelColumn, d->root );
	return d->itemText( mi );
}

QVariant
Picker::itemData( int index, int role ) const
{
	QModelIndex mi = d->model->index( index, d->modelColumn, d->root );
	return d->model->data( mi, role );
}

void
Picker::insertItem( int index, const QString & text, const QVariant & userData )
{
	int itemCount = count();

	index = qBound( 0, index, itemCount );

	if( index >= d->maxCount )
		return;

	// For the common case where we are using the built in QStandardItemModel
	// construct a QStandardItem, reducing the number of expensive signals from
	// the model
	if( QStandardItemModel * m = qobject_cast< QStandardItemModel* > ( d->model ) )
	{
		QStandardItem * item = new QStandardItem( text );
		if( userData.isValid() ) item->setData( userData, Qt::UserRole );
		m->insertRow( index, item );
		++itemCount;
	}
	else
	{
		d->inserting = true;

		if( d->model->insertRows( index, 1, d->root ) )
		{
			QModelIndex item = d->model->index( index, d->modelColumn, d->root );

			if( !userData.isValid() )
				d->model->setData( item, text, Qt::DisplayRole );
			else
			{
				QMap< int, QVariant > values;

				if( !text.isNull() ) values.insert( Qt::DisplayRole, text );
				if( userData.isValid() ) values.insert( Qt::UserRole, userData );
				if( !values.isEmpty() ) d->model->setItemData( item, values );
			}

			d->inserting = false;

			_q_rowsInserted( d->root, index, index );

			++itemCount;
		}
		else
			d->inserting = false;
	}

	if( itemCount > d->maxCount )
		d->model->removeRows( itemCount - 1, itemCount - d->maxCount, d->root );
}

void
Picker::insertItems( int index, const QStringList & texts )
{
	if( texts.isEmpty() )
		return;

	index = qBound( 0, index, count() );

	int insertCount = qMin( d->maxCount - index, texts.count() );

	if( insertCount <= 0 )
		return;

	// For the common case where we are using the built in QStandardItemModel
	// construct a QStandardItem, reducing the number of expensive signals from
	// the model
	if( QStandardItemModel * m = qobject_cast< QStandardItemModel* > ( d->model ) )
	{
		QList< QStandardItem* > items;

		QStandardItem * hiddenRoot = m->invisibleRootItem();

		for( int i = 0; i < insertCount; ++i )
			items.append( new QStandardItem( texts.at( i ) ) );

		hiddenRoot->insertRows( index, items );
	}
	else
	{
		d->inserting = true;

		if( d->model->insertRows( index, insertCount, d->root ) )
		{
			QModelIndex item;

			for( int i = 0; i < insertCount; ++i )
			{
				item = d->model->index( i + index, d->modelColumn, d->root );
				d->model->setData( item, texts.at( i ), Qt::DisplayRole );
			}

			d->inserting = false;

			_q_rowsInserted( d->root, index, index + insertCount - 1 );

		}
		else
			d->inserting = false;
	}

	int mc = count();

	if( mc > d->maxCount )
		d->model->removeRows( d->maxCount, mc - d->maxCount, d->root );
}

void
Picker::removeItem( int index )
{
	if( index < 0 || index >= count() )
		return;

	d->model->removeRows( index, 1, d->root );
}

void
Picker::setItemText( int index, const QString & text )
{
	QModelIndex item = d->model->index( index, d->modelColumn, d->root );

	if( item.isValid() )
		d->model->setData( item, text, Qt::DisplayRole );
}

void
Picker::setItemData( int index, const QVariant & value, int role )
{
	QModelIndex item = d->model->index( index, d->modelColumn, d->root );

	if( item.isValid() )
		d->model->setData( item, value, role );
}

QSize
Picker::sizeHint() const
{
	return QSize();
}

QSize
Picker::minimumSizeHint() const
{
	return QSize();
}

void
Picker::clear()
{
	d->model->removeRows( 0, d->model->rowCount( d->root ), d->root );

#ifndef QT_NO_ACCESSIBILITY
	QAccessibleEvent event( this, QAccessible::NameChanged );
	QAccessible::updateAccessibility( &event );
#endif
}

void
Picker::setCurrentIndex( int index )
{
	QModelIndex mi = d->model->index( index, d->modelColumn, d->root );
	d->setCurrentIndex( mi );
}

void
Picker::setCurrentText( const QString & text )
{
	const int i = findText( text );

	if( i > -1 )
		setCurrentIndex( i );
}

void
Picker::_q_emitCurrentIndexChanged( const QModelIndex & index )
{
	emit currentIndexChanged( index.row() );

	const QString text = d->itemText( index );

	emit currentIndexChanged( text );

	emit currentTextChanged( text );

#ifndef QT_NO_ACCESSIBILITY
	QAccessibleEvent event( this, QAccessible::NameChanged );
	QAccessible::updateAccessibility( &event );
#endif
}

void
Picker::_q_dataChanged( const QModelIndex & topLeft,
	const QModelIndex & bottomRight )
{
	if( d->inserting || topLeft.parent() != d->root )
		return;

	if( d->currentIndex.row() >= topLeft.row() &&
		d->currentIndex.row() <= bottomRight.row() )
	{
		const QString text = itemText( d->currentIndex.row() );

		emit currentTextChanged( text );

		update();
	}
#ifndef QT_NO_ACCESSIBILITY
	QAccessibleEvent event( this, QAccessible::NameChanged );
	QAccessible::updateAccessibility( &event );
#endif
}

void
Picker::_q_updateIndexBeforeChange()
{
	d->indexBeforeChange = d->currentIndex.row();
}

void
Picker::_q_rowsInserted( const QModelIndex & parent, int start, int end )
{
	if( d->inserting || parent != d->root )
		return;

	// set current index if picker was previously empty
	if( start == 0 && ( end - start + 1 ) == count() &&
		!d->currentIndex.isValid() )
			setCurrentIndex( 0 );
	// need to emit changed if model updated index "silently"
	else if( d->currentIndex.row() != d->indexBeforeChange )
	{
		update();
		_q_emitCurrentIndexChanged( d->currentIndex );
	}
}

void
Picker::_q_rowsRemoved( const QModelIndex & parent, int start, int end )
{
	Q_UNUSED( start )
	Q_UNUSED( end )

	if( parent != d->root )
		return;

	// model has changed the currentIndex
	if( d->currentIndex.row() != d->indexBeforeChange )
	{
		if( !d->currentIndex.isValid() && count() )
		{
			setCurrentIndex( qMin( count() - 1, qMax( d->indexBeforeChange, 0 ) ) );
			return;
		}

		update();
		_q_emitCurrentIndexChanged( d->currentIndex );
	}
}

void
Picker::_q_modelDestroyed()
{
	setModel( new QStandardItemModel( 0, 1, this ) );
}

void
Picker::_q_modelReset()
{
	if( d->currentIndex.row() != d->indexBeforeChange )
		_q_emitCurrentIndexChanged( d->currentIndex );

	update();
}

void
Picker::paintEvent( QPaintEvent * event )
{

}

void
Picker::resizeEvent( QResizeEvent * event )
{

}

void
Picker::wheelEvent( QWheelEvent * event )
{

}

void
Picker::mousePressEvent( QMouseEvent * event )
{

}

void
Picker::mouseReleaseEvent( QMouseEvent * event )
{

}

void
Picker::mouseMoveEvent( QMouseEvent * event )
{

}

} /* namespace QtMWidgets */
