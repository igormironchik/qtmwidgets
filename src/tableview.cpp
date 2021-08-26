
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
#include "tableview.hpp"
#include "textlabel.hpp"
#include "private/tableview_p.hpp"
#include "fingergeometry.hpp"

// Qt include.
#include <QVBoxLayout>
#include <QPainter>
#include <QSpacerItem>
#include <QMouseEvent>
#include <QPainter>
#include <QPicture>


namespace QtMWidgets {

//
// MinimumSizeLabel
//

class MinimumSizeLabel
	:	public QLabel
{
public:
	MinimumSizeLabel( QWidget * parent = 0 )
		:	QLabel( parent )
	{
		setMinimumSize( 0, 0 );
		setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
	}

	QSize minimumSizeHint() const override
	{
		if( text().isEmpty() && pixmap( Qt::ReturnByValue ).isNull() && !movie() &&
			picture( Qt::ReturnByValue ).isNull() )
				return QSize( 0, 0 );
		else
		{
			const QSize labelSizeHint = QLabel::sizeHint();

			return QSize( qMax( labelSizeHint.width(), FingerGeometry::width() ),
				qMax( labelSizeHint.height(), FingerGeometry::height() ) );
		}
	}

	QSize sizeHint() const override
	{
		return minimumSizeHint();
	}
}; // class MinimumSizeLabel


//
// TableViewCellLayout
//

class TableViewCellLayout
	:	public QLayout
{
public:
	explicit TableViewCellLayout( QWidget * parent );
	virtual ~TableViewCellLayout();

	void setImageLabel( MinimumSizeLabel * label );
	void setTextLabel( TextLabel * label );
	void setDetailedTextLabel( TextLabel * label );
	void setAccessoryWidget( QWidget * w );

	void addItem( QLayoutItem * item ) override;
	int count() const override;
	QLayoutItem * itemAt( int index ) const override;
	void setGeometry( const QRect & rect ) override;
	QLayoutItem * takeAt( int index ) override;
	bool hasHeightForWidth() const override;
	int heightForWidth( int w ) const override;

	QSize minimumSize() const override;
	QSize sizeHint() const override;

private:
	void setTextGeometry( const QRect & r, int imageOffset,
		int accessoryWidth );

private:
	MinimumSizeLabel * imageLabel;
	TextLabel * textLabel;
	TextLabel * detailedTextLabel;
	QWidget * accessoryWidget;
}; // class TableViewCellLayout

TableViewCellLayout::TableViewCellLayout( QWidget * parent )
	:	QLayout( parent )
	,	imageLabel( 0 )
	,	textLabel( 0 )
	,	detailedTextLabel( 0 )
	,	accessoryWidget( 0 )
{
}

TableViewCellLayout::~TableViewCellLayout()
{
}

void
TableViewCellLayout::setImageLabel( MinimumSizeLabel * label )
{
	imageLabel = label;

	update();
}

void
TableViewCellLayout::setTextLabel( TextLabel * label )
{
	textLabel = label;

	update();
}

void
TableViewCellLayout::setDetailedTextLabel( TextLabel * label )
{
	detailedTextLabel = label;

	update();
}

void
TableViewCellLayout::setAccessoryWidget( QWidget * w )
{
	accessoryWidget = w;

	update();
}

void
TableViewCellLayout::addItem( QLayoutItem * item )
{
	Q_UNUSED( item )
}

int
TableViewCellLayout::count() const
{
	return 4;
}

QLayoutItem *
TableViewCellLayout::itemAt( int index ) const
{
	Q_UNUSED( index )

	return 0;
}

void
TableViewCellLayout::setTextGeometry( const QRect & r, int imageOffset,
	int accessoryWidth )
{
	const int width = r.width() - imageOffset - accessoryWidth -
		( accessoryWidth > 0 ? spacing() : 0 );
	const int textHeight = textLabel->heightForWidth( width );
	const int detailedTextHeight = detailedTextLabel->heightForWidth( width );

	if( detailedTextHeight == 0 && textHeight != 0 )
	{
		textLabel->setGeometry( r.x() + imageOffset, r.y() +
				( r.height() > textHeight ? ( r.height() - textHeight ) / 2 : 0 ),
			width, textHeight );
		detailedTextLabel->setGeometry( r.x(), r.y(), 0, 0 );
	}
	else if( textHeight == 0 && detailedTextHeight != 0 )
	{
		detailedTextLabel->setGeometry( r.x() + imageOffset, r.y() +
				( r.height() > detailedTextHeight ?
					( r.height() - detailedTextHeight ) / 2 : 0 ),
			width, detailedTextHeight );
		textLabel->setGeometry( r.x(), r.y(), 0, 0 );
	}
	else if( textHeight != 0 && detailedTextHeight != 0 )
	{
		const int h = ( r.height() > textHeight + detailedTextHeight ?
			( r.height() - textHeight - detailedTextHeight ) / 2 : 0 );
		textLabel->setGeometry( r.x() + imageOffset, r.y() + h,
			width, textHeight );
		detailedTextLabel->setGeometry( r.x() + imageOffset, r.y() + h +
			textHeight, width, detailedTextHeight );
	}
}

void
TableViewCellLayout::setGeometry( const QRect & rect )
{
	QLayout::setGeometry( rect );

	const QMargins m = contentsMargins();
	const QRect r = rect.adjusted( m.left(), m.top(), -m.right(), -m.bottom() );

	int imageOffset = 0;

	const QSize accessorySizeHint = accessoryWidget->sizeHint();

	if( imageLabel->pixmap( Qt::ReturnByValue ).isNull() || !imageLabel->text().isEmpty() )
	{
		if( accessorySizeHint.isEmpty() && textLabel->text().isEmpty() &&
			detailedTextLabel->text().isEmpty() )
		{
			imageLabel->setGeometry( r );
			textLabel->setGeometry( r.x(), r.y(), 0, 0 );
			detailedTextLabel->setGeometry( r.x(), r.y(), 0, 0 );
			accessoryWidget->setGeometry( r.x(), r.y(), 0, 0 );

			return;
		}

		const QSize imageSize = imageLabel->sizeHint();

		imageLabel->setGeometry( r.x(), r.y(),
			imageSize.width(), imageSize.height() );

		imageOffset += imageSize.width() + spacing();
	}
	else
		imageLabel->setGeometry( r.x(), r.y(), 0, 0 );

	if( accessorySizeHint.isEmpty() )
	{
		accessoryWidget->setGeometry( r.x(), r.y(), 0, 0 );

		setTextGeometry( r, imageOffset, 0 );
	}
	else if( textLabel->text().isEmpty() && detailedTextLabel->text().isEmpty() &&
		!accessorySizeHint.isEmpty() )
	{
		textLabel->setGeometry( r.x(), r.y(), 0, 0 );
		detailedTextLabel->setGeometry( r.x(), r.y(), 0, 0 );

		const int offset = ( accessorySizeHint.height() < r.height() ?
			( r.height() - accessorySizeHint.height() ) / 2 : 0 );

		accessoryWidget->setGeometry( r.x() + imageOffset, r.y() + offset,
			r.width() - imageOffset, r.height() - offset * 2 );
	}
	else if( ( !textLabel->text().isEmpty() || !detailedTextLabel->text().isEmpty() ) &&
		!accessorySizeHint.isEmpty() )
	{
		const int offset = ( accessorySizeHint.height() < r.height() ?
			( r.height() - accessorySizeHint.height() ) / 2 : 0 );

		accessoryWidget->setGeometry( r.x() + r.width() -
			accessorySizeHint.width(), r.y() + offset,
			accessorySizeHint.width(), r.height() - offset * 2 );

		setTextGeometry( r, imageOffset, accessorySizeHint.width() );
	}
	else
	{
		textLabel->setGeometry( r.x(), r.y(), 0, 0 );
		detailedTextLabel->setGeometry( r.x(), r.y(), 0, 0 );
		accessoryWidget->setGeometry( r.x(), r.y(), 0, 0 );
	}
}

QLayoutItem *
TableViewCellLayout::takeAt( int index )
{
	Q_UNUSED( index )

	return 0;
}

bool
TableViewCellLayout::hasHeightForWidth() const
{
	return true;
}

int
TableViewCellLayout::heightForWidth( int w ) const
{
	const QSize imageSize = imageLabel->sizeHint();
	const QSize accessorySize = accessoryWidget->sizeHint();
	const QMargins m = contentsMargins();

	const int textWidth = qMax( 10,
		w - imageSize.width() - accessorySize.width() );

	int height = textLabel->heightForWidth( textWidth ) +
		detailedTextLabel->heightForWidth( textWidth ) + m.top() + m.bottom();

	height = qMax( height, imageSize.height() + m.top() + m.bottom() );
	height = qMax( height, accessorySize.height() + m.top() + m.bottom() );

	return height;
}

QSize
TableViewCellLayout::minimumSize() const
{
	const QSize imageSize = imageLabel->minimumSizeHint();

	int width = imageSize.width();
	int height = imageSize.height();

	const QSize textSize = textLabel->minimumSizeHint();
	const QSize detailedTextSize = detailedTextLabel->minimumSizeHint();

	width += qMax( textSize.width(), detailedTextSize.width() );
	height = qMax( height,
		textSize.height() + detailedTextSize.height() );

	const QSize accessorySize = accessoryWidget->minimumSizeHint();

	width += accessorySize.width();
	height = qMax( height, accessorySize.height() );

	return QSize( width + contentsMargins().left() + contentsMargins().right(),
		height + contentsMargins().top() + contentsMargins().bottom() );
}

QSize
TableViewCellLayout::sizeHint() const
{
	const QSize imageSize = imageLabel->sizeHint();

	int width = imageSize.width();
	int height = imageSize.height();

	const QSize textSize = textLabel->sizeHint();
	const QSize detailedTextSize = detailedTextLabel->sizeHint();

	width += qMax( textSize.width(), detailedTextSize.width() );
	height = qMax( height,
		textSize.height() + detailedTextSize.height() );

	const QSize accessorySize = accessoryWidget->sizeHint();

	width += accessorySize.width();
	height = qMax( height, accessorySize.height() );

	return QSize( width + contentsMargins().left() + contentsMargins().right(),
		height + contentsMargins().top() + contentsMargins().bottom() );
}


//
// TableViewCellPrivate
//

void
TableViewCellPrivate::init()
{
	q->setMinimumHeight( FingerGeometry::height() );
	q->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
	q->setBackgroundRole( QPalette::Base );
	q->setAutoFillBackground( true );

	highlightColor = q->palette().color( QPalette::Highlight );
	highlightColor.setAlpha( 75 );


	layout = new TableViewCellLayout( q );
	layout->setContentsMargins( 3, 3, 3, 3 );

	imageLabel = new MinimumSizeLabel( q );
	imageLabel->setAlignment( Qt::AlignCenter );
	imageLabel->setWordWrap( true );
	layout->setImageLabel( imageLabel );

	{
		textLabel = new TextLabel( q );
		QTextOption opt = textLabel->textOption();
		opt.setAlignment( Qt::AlignVCenter | opt.alignment() );
		textLabel->setTextOption( opt );
		layout->setTextLabel( textLabel );
	}

	{
		detailedTextLabel = new TextLabel( q );
		QFont f = detailedTextLabel->font();
		f.setPointSize( qMax( f.pointSize() - 1, 5 ) );
		detailedTextLabel->setFont( f );
		QTextOption opt = detailedTextLabel->textOption();
		opt.setAlignment( Qt::AlignVCenter | opt.alignment() );
		detailedTextLabel->setTextOption( opt );
		layout->setDetailedTextLabel( detailedTextLabel );
	}

	accessoryWidget = new QWidget( q );
	layout->setAccessoryWidget( accessoryWidget );
}


//
// TableViewCell
//

TableViewCell::TableViewCell( QWidget * parent )
	:	QWidget( parent )
	,	d( new TableViewCellPrivate( this ) )
{
	d->init();
}

TableViewCell::TableViewCell( TableViewCellPrivate * dd, QWidget * parent )
	:	QWidget( parent )
	,	d( dd )
{
	d->init();
}

TableViewCell::~TableViewCell()
{
}

QLabel *
TableViewCell::imageLabel() const
{
	return d->imageLabel;
}

TextLabel *
TableViewCell::textLabel() const
{
	return d->textLabel;
}

TextLabel *
TableViewCell::detailedTextLabel() const
{
	return d->detailedTextLabel;
}

QWidget *
TableViewCell::accessoryWidget() const
{
	return d->accessoryWidget;
}

void
TableViewCell::setAccessoryWidget( QWidget * accessory )
{
	if( accessory == d->accessoryWidget || !accessory )
		return;

	delete d->accessoryWidget;
	d->accessoryWidget = 0;
	if( accessory->parentWidget() != this )
		accessory->setParent( this );
	d->accessoryWidget = accessory;
	d->layout->setAccessoryWidget( d->accessoryWidget );
	d->accessoryWidget->show();

	updateGeometry();
}

bool
TableViewCell::highlightOnClick() const
{
	return d->highlightOnClick;
}

void
TableViewCell::setHighlightOnClick( bool on )
{
	d->highlightOnClick = on;
}

QSize
TableViewCell::minimumSizeHint() const
{
	int width = d->imageLabel->sizeHint().width();
	int height = qMax( d->imageLabel->sizeHint().height(),
		FingerGeometry::height() );

	const int textWidth = qMax( d->textLabel->sizeHint().width(),
		d->detailedTextLabel->sizeHint().width() );
	const int textHeight = d->textLabel->sizeHint().height() +
		d->detailedTextLabel->sizeHint().height();

	width += textWidth;
	height = qMax( height, textHeight );

	const int accessoryWidth = d->accessoryWidget->sizeHint().width();
	const int accessoryHeight = d->accessoryWidget->sizeHint().height();

	width += accessoryWidth;
	height = qMax( height, accessoryHeight );

	width += d->layout->contentsMargins().left() +
		d->layout->contentsMargins().right();
	height += d->layout->contentsMargins().bottom() +
		d->layout->contentsMargins().top();

	return QSize( width, height );
}

QSize
TableViewCell::sizeHint() const
{
	return minimumSizeHint();
}

void
TableViewCell::paintEvent( QPaintEvent * e )
{
	QWidget::paintEvent( e );

	if( d->clicked && d->highlightOnClick )
	{
		QPainter p( this );

		p.setPen( d->highlightColor );
		p.setBrush( d->highlightColor );

		p.drawRect( rect().adjusted( 0, 0, -1, -1 ) );
	}
}

void
TableViewCell::mousePressEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton )
	{
		d->clicked = true;

		if( d->highlightOnClick )
			update();
	}

	e->ignore();
}

void
TableViewCell::mouseReleaseEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton && d->clicked )
	{
		d->clicked = false;

		if( d->highlightOnClick )
			update();
	}

	e->ignore();
}


//
// TableViewSectionPrivate
//

void
TableViewSectionPrivate::init()
{
	q->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
	q->setBackgroundRole( QPalette::Base );
	q->setAutoFillBackground( true );

	layout = new QVBoxLayout( q );
	layout->setSpacing( 0 );
	layout->setContentsMargins( 0, 0, 0, 0 );

	QSizePolicy sp( QSizePolicy::Minimum, QSizePolicy::Fixed );
	sp.setHeightForWidth( true );

	header = new TextLabel( q );
	header->setBackgroundRole( QPalette::Midlight );
	header->setAutoFillBackground( true );
	header->setMargin( 11 );
	header->setSizePolicy( sp );
	layout->addWidget( header );

	footer = new TextLabel( q );
	QFont font = footer->font();
	font.setPointSize( qMax( font.pointSize() - 1, 5 ) );
	footer->setFont( font );
	footer->setBackgroundRole( QPalette::Midlight );
	footer->setAutoFillBackground( true );
	footer->setMargin( 11 );
	footer->setSizePolicy( sp );
	layout->addWidget( footer );
}


//
// RowsSeparator
//

class RowsSeparator
	:	public QWidget
{
public:
	RowsSeparator( QWidget * parent = 0 )
		:	QWidget( parent )
	{
		setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
	}

	QSize sizeHint() const override
	{
		return QSize( 1, 1 );
	}

protected:
	void paintEvent( QPaintEvent * ) override
	{
		QPainter p( this );

		p.setPen( palette().color( QPalette::Midlight ) );
		p.drawLine( 11, 0, width(), 0 );
	}
}; // class RowsSeparator


//
// TableViewSection
//

TableViewSection::TableViewSection( QWidget * parent )
	:	QWidget( parent )
	,	d( new TableViewSectionPrivate( this ) )
{
	d->init();
}

TableViewSection::~TableViewSection()
{
}

TextLabel *
TableViewSection::header() const
{
	return d->header;
}

TextLabel *
TableViewSection::footer() const
{
	return d->footer;
}

void
TableViewSection::addCell( TableViewCell * cell )
{
	insertCell( d->cells.size(), cell );
}

void
TableViewSection::insertCell( int index, TableViewCell * cell )
{
	if( !cell || index < 0 )
		return;

	if( index > d->cells.size() )
		index = d->cells.size();

	int actualIndex = ( index == 0 ? 1 : index + d->cells.size() );

	if( index != 0 )
	{
		RowsSeparator * s = new RowsSeparator( this );
		d->layout->insertWidget( actualIndex, s );
		d->separators.insert( index - 1, s );
		s->show();
		++actualIndex;
	}

	if( cell->parent() != this )
		cell->setParent( this );
	d->layout->insertWidget( actualIndex, cell );
	d->cells.insert( index, cell );
	cell->setHighlightOnClick( d->highlightCellOnClick );
	cell->show();
}

void
TableViewSection::removeCell( TableViewCell * cell )
{
	const int index = d->cells.indexOf( cell );

	if( index != -1 )
		removeCell( index );
}

TableViewCell *
TableViewSection::removeCell( int index )
{
	if( index >= 0 && index < d->cells.size() )
	{
		TableViewCell * cell = d->cells.at( index );

		if( index != 0 )
		{
			RowsSeparator * s = d->separators.at( index - 1 );
			d->layout->removeWidget( s );
			d->separators.removeAt( index - 1 );
			delete s;
		}

		d->layout->removeWidget( cell );
		cell->setParent( 0 );
		cell->hide();

		d->cells.removeAt( index );

		adjustSize();

		return cell;
	}
	else
		return 0;
}

int
TableViewSection::cellsCount() const
{
	return d->cells.size();
}

TableViewCell *
TableViewSection::cellAt( int index )
{
	if( index >= 0 && index < d->cells.size() )
		return d->cells.at( index );
	else
		return 0;
}

bool
TableViewSection::highlightCellOnClick() const
{
	return d->highlightCellOnClick;
}

void
TableViewSection::setHighlightCellOnClick( bool on )
{
	if( d->highlightCellOnClick != on )
	{
		d->highlightCellOnClick = on;

		foreach( TableViewCell * cell, d->cells )
			cell->setHighlightOnClick( on );
	}
}


//
// TableViewPrivate
//

void
TableViewPrivate::init()
{
	TableView * q = q_func();

	widget = new QWidget( q->viewport() );
	QSizePolicy sp = QSizePolicy( QSizePolicy::Minimum,
		QSizePolicy::Minimum );
	sp.setHeightForWidth( true );
	widget->setSizePolicy( sp );
	layout = new QVBoxLayout( widget );
	layout->setSpacing( 0 );
	layout->setContentsMargins( 6, 6, 6, 6 );

	QSpacerItem * spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum,
		QSizePolicy::Expanding );
	layout->addItem( spacer );

	q->setBackgroundRole( QPalette::Midlight );
	q->setAutoFillBackground( true );
	q->setWidgetResizable( true );
	q->setWidget( widget );
}


//
// TableView
//

TableView::TableView( QWidget * parent )
	:	ScrollArea( new TableViewPrivate( this ), parent )
{
	TableViewPrivate * d = d_func();
	d->init();
}

TableView::~TableView()
{
}

void
TableView::addSection( TableViewSection * section )
{
	TableViewPrivate * d = d_func();

	insertSection( d->sections.size(), section );
}

void
TableView::insertSection( int index, TableViewSection * section )
{
	TableViewPrivate * d = d_func();

	if( !section || index < 0 )
		return;

	if( index > d->sections.size() )
		index = d->sections.size();

	if( section->parent() != d->widget )
		section->setParent( d->widget );
	d->layout->insertWidget( index, section );
	d->sections.insert( index, section );
	section->setHighlightCellOnClick( d->highlightCellOnClick );
	section->show();
}

void
TableView::removeSection( TableViewSection * section )
{
	TableViewPrivate * d = d_func();

	const int index = d->sections.indexOf( section );

	if( index != -1 )
		removeSection( index );
}

TableViewSection *
TableView::removeSection( int index )
{
	TableViewPrivate * d = d_func();

	if( index >= 0 && index < d->sections.size() )
	{
		TableViewSection * s = d->sections.at( index );

		d->layout->removeWidget( s );
		s->setParent( 0 );
		s->hide();

		d->sections.removeAt( index );

		d->widget->adjustSize();

		return s;
	}
	else
		return 0;
}

int
TableView::sectionsCount() const
{
	const TableViewPrivate * d = d_func();

	return d->sections.size();
}

TableViewSection *
TableView::sectionAt( int index )
{
	TableViewPrivate * d = d_func();

	if( index >= 0 && index < d->sections.size() )
		return d->sections.at( index );
	else
		return 0;
}

bool
TableView::highlightCellOnClick() const
{
	const TableViewPrivate * d = d_func();

	return d->highlightCellOnClick;
}

void
TableView::setHighlightCellOnClick( bool on )
{
	TableViewPrivate * d = d_func();

	if( d->highlightCellOnClick != on )
	{
		d->highlightCellOnClick = on;

		foreach( TableViewSection * sect, d->sections )
			sect->setHighlightCellOnClick( on );
	}
}

} /* namespace QtMWidgets */
