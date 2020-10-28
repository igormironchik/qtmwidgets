
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
#include "private/drawing.hpp"
#include "color.hpp"
#include "scroller.hpp"
#include "fingergeometry.hpp"
#include "private/utils.hpp"

// Qt include.
#include <QStandardItemModel>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QFontMetrics>
#include <QBrush>
#include <QPen>

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
		,	modelColumn( 0 )
		,	drawItemOffset( 0 )
		,	indexBeforeChange( -1 )
		,	inserting( false )
		,	maxCount( INT_MAX )
		,	minStringLength( 6 )
		,	maxStringWidth( 25 )
		,	stringLength( minStringLength )
		,	itemsCount( 5 )
		,	itemTopMargin( 7 )
		,	itemSideMargin( 0 )
		,	stringHeight( 0 )
		,	leftMouseButtonPressed( false )
		,	mouseWasMoved( false )
		,	wasPainted( false )
		,	mouseMoveDelta( 0 )
		,	scroller( 0 )
	{}

	void init();
	void setCurrentIndex( const QModelIndex & mi );
	QString itemText( const QModelIndex & index ) const;
	QSize minimumSizeHint( const QStyleOption & opt );
	QSize sizeHint( const QStyleOption & opt );
	void computeStringWidth();
	void drawItem( QPainter * p, const QStyleOption & opt, int offset,
		const QModelIndex & index );
	void normalizeOffset();
	void makePrevIndex( QPersistentModelIndex & index );
	void makeNextIndex( QPersistentModelIndex & index );
	QString makeString( const QString & text, const QRect & r, int flags,
		const QStyleOption & opt );
	void drawTick( const QRect & r, QPainter * p );
	void setCurrentIndex( const QPoint & pos );
	QModelIndex indexForPos( const QPoint & pos );
	void initDrawOffsetForFirstUse();
	bool isIndexesVisible( const QModelIndex & topLeft,
		const QModelIndex & bottomRight );
	bool isRowsVisible( int start, int end );

	Picker * q;
	QAbstractItemModel * model;
	int modelColumn;
	QPersistentModelIndex currentIndex;
	QPersistentModelIndex root;
	QPersistentModelIndex topItemIndex;
	int drawItemOffset;
	int indexBeforeChange;
	bool inserting;
	int maxCount;
	int minStringLength;
	int maxStringWidth;
	int stringLength;
	int itemsCount;
	int itemTopMargin;
	int itemSideMargin;
	int stringHeight;
	QPoint mousePos;
	bool leftMouseButtonPressed;
	bool mouseWasMoved;
	bool wasPainted;
	int mouseMoveDelta;
	QColor highlightColor;
	Scroller * scroller;
}; // class PickerPrivate

void
PickerPrivate::init()
{
	q->setSizePolicy( QSizePolicy( QSizePolicy::Preferred,
		QSizePolicy::Fixed ) );

	q->setModel( new QStandardItemModel( 0, 1, q ) );

	scroller = new Scroller( q, q );

	QStyleOption opt;
	opt.initFrom( q );

	highlightColor = opt.palette.color( QPalette::Highlight );
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

QSize
PickerPrivate::minimumSizeHint( const QStyleOption & opt )
{
	stringHeight = opt.fontMetrics.boundingRect( QLatin1String( "A" ) ).height();

	itemSideMargin = opt.fontMetrics.averageCharWidth() * 3;

	itemTopMargin = stringHeight / 3;

	const int minStringWidth = opt.fontMetrics.averageCharWidth() *
		( minStringLength + 2 );
	const int widgetWidth = minStringWidth + 2 * itemSideMargin;
	const int widgetHeight = ( stringHeight + itemTopMargin ) *
		itemsCount - itemTopMargin;

	return QSize( widgetWidth, widgetHeight );
}

QSize
PickerPrivate::sizeHint( const QStyleOption & opt )
{
	computeStringWidth();

	stringHeight = opt.fontMetrics.boundingRect( QLatin1String( "A" ) ).height();

	itemTopMargin = stringHeight / 3;

	itemSideMargin = opt.fontMetrics.averageCharWidth() * 3;

	const int widgetWidth = maxStringWidth + 2 * itemSideMargin +
		2 * opt.fontMetrics.averageCharWidth();
	const int widgetHeight = ( stringHeight + itemTopMargin ) *
		itemsCount - itemTopMargin;

	return QSize( widgetWidth, widgetHeight );
}

void
PickerPrivate::computeStringWidth()
{
	maxStringWidth = 25;

	const int rowCount = q->count();

	if( rowCount > 0 )
	{
		const QFontMetrics & fm = q->fontMetrics();

		for( int i = 0; i < rowCount; ++i )
		{
			const int width = fm.boundingRect( q->itemText( i ) ).width();

			if( width > maxStringWidth )
				maxStringWidth = width;
		}
	}
}

void
PickerPrivate::drawItem( QPainter * p, const QStyleOption & opt, int offset,
	const QModelIndex & index )
{
	if( index.flags() & Qt::ItemIsEnabled )
	{
		if( index != currentIndex )
			p->setPen( opt.palette.color( QPalette::WindowText ) );
		else
			p->setPen( highlightColor );
	}
	else
		p->setPen( lighterColor( opt.palette.color( QPalette::WindowText ), 75 ) );

	const QRect r( opt.rect.x() + itemSideMargin, offset,
		opt.rect.width() - itemSideMargin * 2, stringHeight );

	const int flags = Qt::AlignLeft | Qt::TextSingleLine;

	p->drawText( r, flags,
		makeString( itemText( index ), r, flags, opt ) );

	if( index.flags() & Qt::ItemIsEnabled && index == currentIndex )
	{
		const QRect tickRect( opt.rect.x() + itemSideMargin -
				opt.fontMetrics.averageCharWidth() -
				opt.fontMetrics.averageCharWidth() / 2,
			offset,
			opt.fontMetrics.averageCharWidth(),
			stringHeight );

		drawTick( tickRect, p );
	}
}

QString
PickerPrivate::makeString( const QString & text, const QRect & r,
	int flags, const QStyleOption & opt )
{
	return accomodateString( text, r, flags, opt );
}

void
PickerPrivate::drawTick( const QRect & r, QPainter * p )
{
	p->save();

	p->setRenderHint( QPainter::Antialiasing );

	QPen pen = p->pen();
	pen.setWidth( 2 );
	pen.setCapStyle( Qt::RoundCap );
	p->setPen( pen );

	p->setViewport( r );
	p->setWindow( 0, 0, 6, 7 );
	p->drawLine( 0, 4, 2, 6 );
	p->drawLine( 2, 6, 5, 1 );

	p->restore();
}

void
PickerPrivate::normalizeOffset()
{
	if( q->count() < itemsCount )
	{
		const int freeItemsCount = ( itemsCount - q->count() );

		const int maxOffset = freeItemsCount * stringHeight +
			( freeItemsCount > 0 ? freeItemsCount - 1 : 0 ) * itemTopMargin;

		if( drawItemOffset < 0 )
			drawItemOffset = 0;
		else if( drawItemOffset > maxOffset )
			drawItemOffset = maxOffset;
	}
	else
	{
		int fullItemsCount = drawItemOffset /
			( itemTopMargin + stringHeight );

		drawItemOffset -= ( itemTopMargin + stringHeight ) * fullItemsCount;

		if( fullItemsCount < 0 )
		{
			for( int i = fullItemsCount; i < 0; ++i )
				makeNextIndex( topItemIndex );
		}
		else if( fullItemsCount > 0 )
		{
			for( int i = 0; i < fullItemsCount; ++i )
				makePrevIndex( topItemIndex );
		}

		if( drawItemOffset > 0 )
		{
			drawItemOffset -= ( itemTopMargin + stringHeight );
			makePrevIndex( topItemIndex );
		}
	}
}

void
PickerPrivate::makePrevIndex( QPersistentModelIndex & index )
{
	if( index.row() == 0 )
		index = model->index( q->count() - 1, modelColumn, root );
	else
		index = model->index( index.row() - 1, modelColumn, root );
}

void
PickerPrivate::makeNextIndex( QPersistentModelIndex & index )
{
	if( index.row() == q->count() - 1 )
		index = model->index( 0, modelColumn, root );
	else
		index = model->index( index.row() + 1, modelColumn, root );
}

void
PickerPrivate::setCurrentIndex( const QPoint & pos )
{
	const QModelIndex index = indexForPos( pos );

	if( index.isValid() && ( index.flags() & Qt::ItemIsEnabled ) )
	{
		setCurrentIndex( index );
		emit q->activated( itemText( index ) );
		emit q->activated( index.row() );
	}
}

QModelIndex
PickerPrivate::indexForPos( const QPoint & pos )
{
	int offset = drawItemOffset;

	for( int i = 0; i < itemsCount + 1; ++i )
	{
		const QRect r( q->rect().x() + itemSideMargin, offset,
			q->rect().width() - itemSideMargin, stringHeight );

		if( r.contains( pos ) )
		{
			QPersistentModelIndex index = topItemIndex;

			for( int j = 0; j < i; ++j )
				makeNextIndex( index );

			return index;
		}

		offset += stringHeight + itemTopMargin;
	}

	return QModelIndex();
}

void
PickerPrivate::initDrawOffsetForFirstUse()
{
	if( !wasPainted && q->count() < itemsCount )
	{
		const int freeItemsCount = ( itemsCount - q->count() );

		const int maxOffset = freeItemsCount * stringHeight +
			( freeItemsCount > 0 ? freeItemsCount - 1 : 0 ) * itemTopMargin;

		drawItemOffset = maxOffset / 2;

		wasPainted = true;
	}
}

bool
PickerPrivate::isIndexesVisible( const QModelIndex & topLeft,
	const QModelIndex & bottomRight )
{
	return isRowsVisible( topLeft.row(), bottomRight.row() );
}

bool
PickerPrivate::isRowsVisible( int start, int end )
{
	int visibleItemsCount = itemsCount;

	if( q->count() > itemsCount )
		++visibleItemsCount;

	QPersistentModelIndex index = topItemIndex;

	for( int i = 0; i < visibleItemsCount; ++i )
	{
		if( index.row() >= start && index.row() <= end )
			return true;

		makeNextIndex( index );
	}

	return false;
}


//
// Picker
//

Picker::Picker( QWidget * parent, Qt::WindowFlags f )
	:	QWidget( parent, f )
	,	d( new PickerPrivate( this ) )
{
	d->init();

	connect( d->scroller, &Scroller::scroll,
		this, &Picker::_q_scroll );
}

Picker::~Picker()
{
	try {
		disconnect( d->model, &QAbstractItemModel::destroyed,
			this, &Picker::_q_modelDestroyed );
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
		disconnect( d->model, &QAbstractItemModel::dataChanged,
			this, &Picker::_q_dataChanged );
		disconnect( d->model, &QAbstractItemModel::rowsAboutToBeInserted,
			this, &Picker::_q_updateIndexBeforeChange );
		disconnect( d->model, &QAbstractItemModel::rowsInserted,
			this, &Picker::_q_rowsInserted );
		disconnect( d->model, &QAbstractItemModel::rowsAboutToBeRemoved,
			this, &Picker::_q_updateIndexBeforeChange );
		disconnect( d->model, &QAbstractItemModel::rowsRemoved,
			this, &Picker::_q_rowsRemoved );
		disconnect( d->model, &QAbstractItemModel::destroyed,
			this, &Picker::_q_modelDestroyed );
		disconnect( d->model, &QAbstractItemModel::modelAboutToBeReset,
			this, &Picker::_q_updateIndexBeforeChange );
		disconnect( d->model, &QAbstractItemModel::modelReset,
			this, &Picker::_q_modelReset );

		if( d->model->QObject::parent() == this )
			delete d->model;
	}

	d->model = model;

	connect( model, &QAbstractItemModel::dataChanged,
		this, &Picker::_q_dataChanged );
	connect( model, &QAbstractItemModel::rowsAboutToBeInserted,
		this, &Picker::_q_updateIndexBeforeChange );
	connect( model, &QAbstractItemModel::rowsInserted,
		this, &Picker::_q_rowsInserted );
	connect( model, &QAbstractItemModel::rowsAboutToBeRemoved,
		this, &Picker::_q_updateIndexBeforeChange );
	connect( model, &QAbstractItemModel::rowsRemoved,
		this, &Picker::_q_rowsRemoved );
	connect( model, &QAbstractItemModel::destroyed,
		this, &Picker::_q_modelDestroyed );
	connect( model, &QAbstractItemModel::modelAboutToBeReset,
		this, &Picker::_q_updateIndexBeforeChange );
	connect( model, &QAbstractItemModel::modelReset,
		this, &Picker::_q_modelReset );

	bool currentReset = false;

	if( count() )
	{
		for( int pos = 0; pos < count(); ++pos )
		{
			if( d->model->index( pos, d->modelColumn, d->root ).flags() & Qt::ItemIsEnabled )
			{
				d->topItemIndex = d->model->index( pos, d->modelColumn, d->root );
				setCurrentIndex( pos );
				currentReset = true;
				break;
			}
		}
	}

	if( !currentReset )
	{
		setCurrentIndex( -1 );
		d->topItemIndex = QModelIndex();
	}
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

QColor
Picker::highlightColor() const
{
	return d->highlightColor;
}

void
Picker::setHighlightColor( const QColor & c )
{
	if( d->highlightColor != c )
	{
		d->highlightColor = c;

		update();
	}
}

Scroller *
Picker::scroller() const
{
	return d->scroller;
}

QSize
Picker::sizeHint() const
{
	QStyleOption opt;
	opt.initFrom( this );

	return d->sizeHint( opt );
}

QSize
Picker::minimumSizeHint() const
{
	QStyleOption opt;
	opt.initFrom( this );

	return d->minimumSizeHint( opt );
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
	scrollTo( index );
}

void
Picker::setCurrentText( const QString & text )
{
	const int i = findText( text );

	if( i > -1 )
		setCurrentIndex( i );
}

void
Picker::scrollTo( int index )
{
	if( count() > d->itemsCount )
	{
		QModelIndex mi = d->model->index( index, d->modelColumn, d->root );
		QPersistentModelIndex top = mi;

		for( int i = 0; i < d->itemsCount / 2; ++i )
			d->makePrevIndex( top );

		d->topItemIndex = top;
		d->drawItemOffset = 0;

		update();
	}
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
	else if( d->isIndexesVisible( topLeft, bottomRight ) )
		update();

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
	{
		d->topItemIndex = d->model->index( 0, d->modelColumn, d->root );
		setCurrentIndex( 0 );
	}
	// need to emit changed if model updated index "silently"
	else if( d->currentIndex.row() != d->indexBeforeChange )
	{
		update();
		_q_emitCurrentIndexChanged( d->currentIndex );
	}
	else if( d->isRowsVisible( start, end ) )
		update();
}

void
Picker::_q_rowsRemoved( const QModelIndex & parent, int start, int end )
{
	if( parent != d->root )
		return;

	// model has changed the currentIndex
	if( d->currentIndex.row() != d->indexBeforeChange )
	{
		if( !d->currentIndex.isValid() && count() )
		{
			const int index = qMin( count() - 1, qMax( d->indexBeforeChange, 0 ) );
			d->topItemIndex = d->model->index( index, d->modelColumn, d->root );
			setCurrentIndex( index );
			return;
		}

		update();
		_q_emitCurrentIndexChanged( d->currentIndex );
	}
	else if( d->isRowsVisible( start, end ) )
		update();
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
Picker::_q_scroll( int dx, int dy )
{
	Q_UNUSED( dx )

	d->drawItemOffset += dy;
	update();
}

void
Picker::paintEvent( QPaintEvent * )
{
	QStyleOption opt;
	opt.initFrom( this );

	QPainter p( this );

	drawCylinder( &p, opt.rect, palette().color( QPalette::Dark ) );

	if( count() > 0 )
	{
		if( !d->topItemIndex.isValid() )
		{
			d->topItemIndex = d->model->index( 0, d->modelColumn, d->root );
			d->drawItemOffset = 0;
		}

		d->initDrawOffsetForFirstUse();
		d->normalizeOffset();

		int maxCount = d->itemsCount;

		if( count() >= d->itemsCount )
			++maxCount;

		int offset = d->drawItemOffset;

		for( int i = d->topItemIndex.row(), itemsCount = 0, scanedItems = 0;
			( itemsCount < maxCount ) && ( scanedItems < count() ) ;
			++i, ++scanedItems, ++itemsCount )
		{
			if( i == count() )
				i = 0;

			d->drawItem( &p, opt, offset,
				d->model->index( i, d->modelColumn, d->root ) );

			offset += d->itemTopMargin + d->stringHeight;
		}
	}
}

void
Picker::wheelEvent( QWheelEvent * event )
{
	QPoint numPixels = event->pixelDelta();
	QPoint numDegrees = event->angleDelta();

	if( !numPixels.isNull() )
	{
		d->drawItemOffset += numPixels.y();
		update();
	}
	else if( !numDegrees.isNull() )
	{
		d->drawItemOffset += numDegrees.y() / 8;
		update();
	}

	event->accept();
}

void
Picker::mousePressEvent( QMouseEvent * event )
{
	if( event->button() == Qt::LeftButton )
	{
		d->mouseWasMoved = false;
		d->mouseMoveDelta = 0;
		d->mousePos = event->pos();
		d->leftMouseButtonPressed = true;

		event->accept();
	}
	else
		event->ignore();
}

void
Picker::mouseReleaseEvent( QMouseEvent * event )
{
	if( event->button() == Qt::LeftButton )
	{
		d->leftMouseButtonPressed = false;

		if( !d->mouseWasMoved
			|| d->mouseMoveDelta <= FingerGeometry::touchBounce() )
				d->setCurrentIndex( event->pos() );

		event->accept();
	}
	else
		event->ignore();
}

void
Picker::mouseMoveEvent( QMouseEvent * event )
{
	if( d->leftMouseButtonPressed )
	{
		d->mouseWasMoved = true;

		const int delta = event->pos().y() - d->mousePos.y();
		d->mouseMoveDelta += qAbs( delta );
		d->drawItemOffset += delta;
		d->mousePos = event->pos();
		update();

		event->accept();
	}
	else
		event->ignore();
}

} /* namespace QtMWidgets */
