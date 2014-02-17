
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
#include "private/tableview_p.hpp"

// Qt include.
#include <QVBoxLayout>
#include <QPainter>
#include <QSpacerItem>


namespace QtMWidgets {

static const int minimumCellHeight = 44;


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

	QSize minimumSizeHint() const
	{
		if( text().isEmpty() && !pixmap() )
			return QSize( 0, 0 );
		else
		{
			const QSize labelSizeHint = QLabel::sizeHint();

			return QSize( qMax( labelSizeHint.width(), minimumCellHeight ),
				qMax( labelSizeHint.height(), minimumCellHeight ) );
		}
	}

	QSize sizeHint() const
	{
		return minimumSizeHint();
	}
}; // class MinimumSizeLabel


//
// TableViewCellPrivate
//

void
TableViewCellPrivate::init()
{
	q->setMinimumHeight( minimumCellHeight );
	q->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
	q->setBackgroundRole( QPalette::Base );

	layout = new QHBoxLayout( q );
	layout->setContentsMargins( 3, 3, 3, 3 );

	imageLabel = new MinimumSizeLabel( q );
	imageLabel->setBackgroundRole( QPalette::Base );
	layout->addWidget( imageLabel );

	QVBoxLayout * vl = new QVBoxLayout();
	vl->setSpacing( 0 );
	vl->setContentsMargins( 0, 0, 0, 0 );

	textLabel = new MinimumSizeLabel( q );
	textLabel->setWordWrap( true );
	textLabel->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	textLabel->setBackgroundRole( QPalette::Base );

	detailedTextLabel = new MinimumSizeLabel( q );
	detailedTextLabel->setWordWrap( true );
	detailedTextLabel->setBackgroundRole( QPalette::Base );
	QFont f = detailedTextLabel->font();
	f.setPointSize( qMax( f.pointSize() - 5, 5 ) );
	detailedTextLabel->setFont( f );

	vl->addWidget( textLabel );
	vl->addWidget( detailedTextLabel );

	layout->addLayout( vl );

	accessoryWidget = new QWidget( q );
	accessoryWidget->setBackgroundRole( QPalette::Base );
	layout->addWidget( accessoryWidget );
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

QLayout *
TableViewCell::layout() const
{
	return d->layout;
}

QLabel *
TableViewCell::imageLabel() const
{
	return d->imageLabel;
}

QLabel *
TableViewCell::textLabel() const
{
	return d->textLabel;
}

QLabel *
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

	d->layout->removeWidget( d->accessoryWidget );
	delete d->accessoryWidget;
	d->accessoryWidget = 0;
	if( accessory->parentWidget() != this )
		accessory->setParent( this );
	d->accessoryWidget = accessory;
	d->layout->addWidget( d->accessoryWidget );
	d->accessoryWidget->show();
}

QSize
TableViewCell::minimumSizeHint() const
{
	int width = qMax( d->imageLabel->sizeHint().width(),
		minimumCellHeight );
	int height = qMax( d->imageLabel->sizeHint().height(),
		minimumCellHeight );

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

	return QSize( width, height );
}

QSize
TableViewCell::sizeHint() const
{
	return minimumSizeHint();
}


//
// TableViewSectionPrivate
//

void
TableViewSectionPrivate::init()
{
	q->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
	q->setBackgroundRole( QPalette::Base );
	q->setAutoFillBackground( true );

	layout = new QVBoxLayout( q );
	layout->setSpacing( 0 );
	layout->setContentsMargins( 0, 0, 0, 0 );

	header = new MinimumSizeLabel( q );
	header->setWordWrap( true );
	header->setBackgroundRole( QPalette::Midlight );
	header->setAutoFillBackground( true );
	header->setMargin( 11 );
	header->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
	layout->addWidget( header );

	footer = new MinimumSizeLabel( q );
	footer->setWordWrap( true );
	footer->setBackgroundRole( QPalette::Midlight );
	footer->setAutoFillBackground( true );
	footer->setMargin( 11 );
	footer->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
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

	QSize sizeHint() const
	{
		return QSize( 1, 1 );
	}

protected:
	void paintEvent( QPaintEvent * )
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

QLabel *
TableViewSection::header() const
{
	return d->header;
}

QLabel *
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


//
// TableViewPrivate
//

void
TableViewPrivate::init()
{
	TableView * q = q_func();

	widget = new QWidget( q->viewport() );
	layout = new QVBoxLayout( widget );
	layout->setSpacing( 0 );
	layout->setContentsMargins( 6, 6, 6, 6 );

	QSpacerItem * spacer = new QSpacerItem( 10, 10, QSizePolicy::Minimum,
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

} /* namespace QtMWidgets */
