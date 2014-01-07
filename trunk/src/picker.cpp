
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
#include <QPainter>
#include <QStyleOption>
#include <QFontMetrics>
#include <QLinearGradient>
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
		,	insertPolicy( Picker::InsertAtBottom )
		,	modelColumn( 0 )
		,	indexBeforeChange( -1 )
		,	inserting( false )
		,	maxCount( INT_MAX )
		,	minStringLength( 6 )
		,	stringLength( minStringLength )
		,	maxStringWidth( 25 )
		,	itemsCount( 5 )
		,	itemTopMargin( 7 )
		,	itemSideMargin( 40 )
		,	drawItemOffset( 0 )
		,	stringHeight( 0 )
		,	leftMouseButtonPressed( false )
	{}

	void init();
	void setCurrentIndex( const QModelIndex & mi );
	QString itemText( const QModelIndex & index ) const;
	void drawBackground( QPainter * p, const QStyleOption & opt );
	QSize minimumSizeHint( const QStyleOption & opt );
	QSize sizeHint( const QStyleOption & opt );
	void computeStringWidth();
	void drawItem( QPainter * p, const QStyleOption & opt, int offset,
		const QModelIndex & index );
	void normalizeOffset();
	void makePrevTopItemIndex();
	void makeNextTopItemIndex();
	QString makeString( const QString & text, const QRect & r, int flags,
		const QStyleOption & opt );
	void drawTick( const QRect & r, QPainter * p );

	Picker * q;
	QAbstractItemModel * model;
	Picker::InsertPolicy insertPolicy;
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

void
PickerPrivate::drawBackground( QPainter * p, const QStyleOption & opt )
{
	p->setRenderHint( QPainter::Antialiasing );

	QLinearGradient firstVertLineGradient( QPointF( 0.0, 0.0 ),
		QPointF( 0.0, 1.0 ) );
	firstVertLineGradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	firstVertLineGradient.setColorAt( 0.0, QColor( 60, 65, 70 ) );
	firstVertLineGradient.setColorAt( 0.15, QColor( 100, 100, 110 ) );
	firstVertLineGradient.setColorAt( 0.5, QColor( 135, 135, 140 ) );
	firstVertLineGradient.setColorAt( 0.85, QColor( 100, 100, 110 ) );
	firstVertLineGradient.setColorAt( 1.0, QColor( 60, 65, 70 ) );

	QLinearGradient secondVertLineGradient( QPointF( 0.0, 0.0 ),
		QPointF( 0.0, 1.0 ) );
	secondVertLineGradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	secondVertLineGradient.setColorAt( 0.0, QColor( 80, 80, 90 ) );
	secondVertLineGradient.setColorAt( 0.15, QColor( 155, 155, 175 ) );
	secondVertLineGradient.setColorAt( 0.5, QColor( 215, 215, 235 ) );
	secondVertLineGradient.setColorAt( 0.85, QColor( 155, 155, 175 ) );
	secondVertLineGradient.setColorAt( 1.0, QColor( 80, 80, 90 ) );

	QLinearGradient thirdVertLineGradient( QPointF( 0.0, 0.0 ),
		QPointF( 0.0, 1.0 ) );
	thirdVertLineGradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	thirdVertLineGradient.setColorAt( 0.0, QColor( 80, 80, 95 ) );
	thirdVertLineGradient.setColorAt( 0.15, QColor( 205, 205, 210 ) );
	thirdVertLineGradient.setColorAt( 0.5, QColor( 205, 205, 225 ) );
	thirdVertLineGradient.setColorAt( 0.85, QColor( 205, 205, 210 ) );
	thirdVertLineGradient.setColorAt( 1.0, QColor( 80, 80, 80 ) );

	p->setPen( QPen( QBrush( firstVertLineGradient ), 1.0 ) );
	p->drawLine( 1, 2, 1, opt.rect.height() - 2 );
	p->drawLine( opt.rect.width() - 1, 2,
		opt.rect.width() - 1, opt.rect.height() - 2 );

	p->setPen( QPen( QBrush( secondVertLineGradient ), 1.0 ) );
	p->drawLine( 2, 1, 2, opt.rect.height() - 1 );
	p->drawLine( opt.rect.width() - 2, 1,
		opt.rect.width() - 2, opt.rect.height() - 1 );

	p->setPen( QPen( QBrush( thirdVertLineGradient ), 1.0 ) );
	p->drawLine( 3, 0, 3, opt.rect.height() );
	p->drawLine( opt.rect.width() - 3, 0,
		opt.rect.width() - 3, opt.rect.height() );

	const QRect leftTop( QPoint( 0, 0 ), QSize( 3, 3 ) );
	const QRect rightTop( QPoint( opt.rect.width() - 3, 0 ), QSize( 3, 3 ) );
	const QRect leftBottom( QPoint( 0, opt.rect.height() - 3 ), QSize( 3, 3 ) );
	const QRect rightBottom( QPoint( opt.rect.width() - 3,
		opt.rect.height() - 3 ), QSize( 3, 3 ) );

	p->setPen( QColor( 50, 50, 65 ) );
	p->drawArc( leftTop, 270 * 16, 90 * 16 );
	p->drawArc( rightTop, 180 * 16, 90 * 16 );
	p->drawArc( leftBottom, 0, 90 * 16 );
	p->drawArc( rightBottom, 90 * 16, 90 * 16 );

	QLinearGradient backgroundGradient( QPointF( 0.0, 0.0 ),
		QPointF( 0.0, 1.0 ) );
	backgroundGradient.setCoordinateMode( QGradient::ObjectBoundingMode );
	backgroundGradient.setColorAt( 0.0, QColor( 80, 80, 80 ) );
	backgroundGradient.setColorAt( 0.15, QColor( 215, 215, 220 ) );
	backgroundGradient.setColorAt( 0.5, QColor( 255, 255, 255 ) );
	backgroundGradient.setColorAt( 0.85, QColor( 215, 215, 220 ) );
	backgroundGradient.setColorAt( 1.0, QColor( 80, 80, 80 ) );

	p->setPen( QPen( QBrush( backgroundGradient ), 1.0 ) );
	p->setBrush( backgroundGradient );
	p->drawRect( 4, 0, opt.rect.width() - 2 * 4, opt.rect.height() );
}

QSize
PickerPrivate::minimumSizeHint( const QStyleOption & opt )
{
	stringHeight = opt.fontMetrics.height();

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

	stringHeight = opt.fontMetrics.height();

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
	p->setRenderHint( QPainter::TextAntialiasing );

	if( index.flags() & Qt::ItemIsEnabled )
	{
		if( index != currentIndex )
			p->setPen( opt.palette.color( QPalette::WindowText ) );
		else
			p->setPen( opt.palette.color( QPalette::Highlight ) );
	}
	else
		p->setPen( opt.palette.color( QPalette::Mid ) );

	const QRect r( opt.rect.x() + itemSideMargin, offset,
		opt.rect.width() - itemSideMargin, stringHeight );

	const int flags = Qt::AlignLeft | Qt::TextSingleLine;

	p->drawText( r, flags,
		makeString( itemText( index ), r, flags, opt ) );

	if( index.flags() & Qt::ItemIsEnabled && index == currentIndex )
	{
		const QRect tickRect( opt.rect.x() + itemSideMargin -
			opt.fontMetrics.averageCharWidth() - 15, offset,
			opt.fontMetrics.averageCharWidth(),
			stringHeight - itemTopMargin );

		drawTick( tickRect, p );
	}
}

QString
PickerPrivate::makeString( const QString & text, const QRect & r,
	int flags, const QStyleOption & opt )
{
	const QRect & b = opt.fontMetrics.boundingRect( r, flags, text );

	QString res = text;

	if( b.width() > r.width() )
	{
		const int averageCount = r.width() / opt.fontMetrics.averageCharWidth();

		res = text.left( averageCount - 6 );
		res.append( QLatin1String( "..." ) );
		res.append( text.right( averageCount - res.length() ) );
	}

	return res;
}

void
PickerPrivate::drawTick( const QRect & r, QPainter * p )
{
	p->save();

	QPen pen = p->pen();
	pen.setWidth( 2 );
	p->setPen( pen );

	p->setViewport( r );
	p->setWindow( 0, 0, 6, 7 );
	p->drawLine( 0, 4, 2, 7 );
	p->drawLine( 2, 7, 6, 0 );

	p->restore();
}

void
PickerPrivate::normalizeOffset()
{
	if( q->count() <= itemsCount )
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
				makeNextTopItemIndex();
		}
		else if( fullItemsCount > 0 )
		{
			for( int i = 0; i < fullItemsCount; ++i )
				makePrevTopItemIndex();
		}

		if( drawItemOffset > 0 )
		{
			drawItemOffset -= ( itemTopMargin + stringHeight );
			makePrevTopItemIndex();
		}
	}
}

void
PickerPrivate::makePrevTopItemIndex()
{
	if( topItemIndex.row() == 0 )
		topItemIndex = model->index( q->count() - 1, modelColumn, root );
	else
		topItemIndex = model->index( topItemIndex.row() - 1, modelColumn, root );
}

void
PickerPrivate::makeNextTopItemIndex()
{
	if( topItemIndex.row() == q->count() - 1 )
		topItemIndex = model->index( 0, modelColumn, root );
	else
		topItemIndex = model->index( topItemIndex.row() + 1, modelColumn, root );
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
			const int index = qMin( count() - 1, qMax( d->indexBeforeChange, 0 ) );
			d->topItemIndex = d->model->index( index, d->modelColumn, d->root );
			setCurrentIndex( index );
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
	QStyleOption opt;
	opt.initFrom( this );

	QPainter p( this );

	d->drawBackground( &p, opt );

	if( count() > 0 )
	{
		d->normalizeOffset();

		int maxCount = d->itemsCount;

		if( !d->topItemIndex.isValid() )
		{
			d->topItemIndex = d->model->index( 0, d->modelColumn, d->root );
			d->drawItemOffset = 0;
		}

		if( count() >= d->itemsCount )
			++maxCount;

		int offset = d->drawItemOffset;

		for( int i = d->topItemIndex.row(), itemsCount = 0, scanedItems = 1;
			( itemsCount < maxCount ) && ( scanedItems < count() ) ;
			++i, +scanedItems )
		{
			if( i == count() )
				i = 0;

			d->drawItem( &p, opt, offset,
				d->model->index( i, d->modelColumn, d->root ) );
			++itemsCount;
			offset += d->itemTopMargin + d->stringHeight;
		}
	}
}

void
Picker::resizeEvent( QResizeEvent * event )
{

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
		d->mousePos = event->pos();
		d->leftMouseButtonPressed = true;
		event->accept();
	}
}

void
Picker::mouseReleaseEvent( QMouseEvent * event )
{
	d->leftMouseButtonPressed = false;
	event->accept();
}

void
Picker::mouseMoveEvent( QMouseEvent * event )
{
	if( d->leftMouseButtonPressed )
	{
		d->drawItemOffset += event->pos().y() - d->mousePos.y();
		d->mousePos = event->pos();
		event->accept();
		update();
	}
}

} /* namespace QtMWidgets */
