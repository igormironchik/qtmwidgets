
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// QtMWidgets include.
#include "textlabel.hpp"

// Qt include.
#include <QStaticText>
#include <QPainter>
#include <QResizeEvent>
#include <QFontMetrics>
#include <QTextDocument>


namespace QtMWidgets {

//
// TextLabelPrivate
//

class TextLabelPrivate {
public:
	TextLabelPrivate( TextLabel * parent )
		:	q( parent )
		,	margin( 0 )
	{
	}

	void init();

	TextLabel * q;
	QStaticText staticText;
	int margin;
	QColor color;
}; // class TextLabelPrivate

void
TextLabelPrivate::init()
{
	staticText.setTextFormat( Qt::AutoText );

	QTextOption opt;
	opt.setAlignment( Qt::AlignLeft );
	opt.setFlags( QTextOption::IncludeTrailingSpaces );
	opt.setWrapMode( QTextOption::WordWrap );
	staticText.setTextOption( opt );

	staticText.setTextWidth( q->fontMetrics().averageCharWidth() * 10 );

	QSizePolicy sp( QSizePolicy::Preferred, QSizePolicy::Preferred );
	sp.setHeightForWidth( true );
	q->setSizePolicy( sp );

	color = q->palette().color( QPalette::WindowText );
}


//
// TextLabel
//

TextLabel::TextLabel( QWidget * parent, Qt::WindowFlags f )
	:	QFrame( parent, f )
	,	d( new TextLabelPrivate( this ) )
{
	d->init();
}

TextLabel::TextLabel( const QString & text,
	QWidget * parent, Qt::WindowFlags f )
	:	QFrame( parent, f )
	,	d( new TextLabelPrivate( this ) )
{
	d->init();
	setText( text );
}

TextLabel::~TextLabel()
{
}

QString
TextLabel::text() const
{
	return d->staticText.text();
}

void
TextLabel::setText( const QString & text )
{
	d->staticText.setText( text );

	update();
}

Qt::TextFormat
TextLabel::textFormat() const
{
	return d->staticText.textFormat();
}

void
TextLabel::setTextFormat( Qt::TextFormat format )
{
	d->staticText.setTextFormat( format );

	update();
}

QTextOption
TextLabel::textOption() const
{
	return d->staticText.textOption();
}

void
TextLabel::setTextOption( const QTextOption & textOption )
{
	d->staticText.setTextOption( textOption );

	update();
}

void
TextLabel::setFont( const QFont & font )
{
	QFrame::setFont( font );

	d->staticText.prepare( QTransform(), font );

	update();
}

int
TextLabel::margin() const
{
	return d->margin;
}

void
TextLabel::setMargin( int margin )
{
	if( d->margin == margin )
		return;

	d->margin = margin;

	const QMargins margins = contentsMargins();

	d->staticText.setTextWidth( width() - margins.left() -
		margins.right() - 2 * frameWidth() - 2 * d->margin );

	update();
}

const QColor &
TextLabel::color() const
{
	return d->color;
}

void
TextLabel::setColor( const QColor & c )
{
	if( d->color != c )
	{
		d->color = c;

		update();
	}
}

bool
TextLabel::hasHeightForWidth() const
{
	return true;
}

int
TextLabel::heightForWidth( int w ) const
{
	if( text().isEmpty() )
		return 2 * frameWidth();

	const QMargins margins = contentsMargins();

	const qreal width = w - 2 * frameWidth() - margins.left() -
		margins.right() - 2 * d->margin;

	QTextDocument doc;
	doc.setHtml( d->staticText.text() );
	doc.setTextWidth( width );
	doc.setDefaultTextOption( d->staticText.textOption() );

	return doc.size().height() +
		2 * frameWidth() + margins.top() +
		margins.bottom() + 2 * d->margin;
}

QSize
TextLabel::minimumSizeHint() const
{
	if( text().isEmpty() )
		return QSize( 2 * frameWidth(), 2 * frameWidth() );

	const QMargins margins = contentsMargins();

	QTextDocument doc;
	doc.setHtml( d->staticText.text() );
	doc.setDefaultTextOption( d->staticText.textOption() );
	doc.setTextWidth( fontMetrics().averageCharWidth() * 10 );

	const QSizeF size = doc.size();
	const int frame = 2 * frameWidth();

	return QSize( size.width() + frame + margins.left() + margins.right() +
		2 * d->margin,
		size.height() + frame + margins.top() + margins.bottom() +
		2 * d->margin );
}

QSize
TextLabel::sizeHint() const
{
	return minimumSizeHint();
}

void
TextLabel::paintEvent( QPaintEvent * e )
{
	QFrame::paintEvent( e );

	QPainter p( this );
	const QRect cr = contentsRect();

	p.setClipRect( cr );
	p.setPen( d->color );

	int vAlign = d->staticText.textOption().alignment() & Qt::AlignVertical_Mask;

	QPoint topLeft = cr.topLeft();

	switch( vAlign )
	{
		case Qt::AlignBottom :
			topLeft = QPoint( topLeft.x() + d->margin,
				cr.bottomLeft().y() - qRound( d->staticText.size().height() ) -
				d->margin );
		break;

		case Qt::AlignVCenter :
			topLeft = QPoint( topLeft.x() + d->margin,
				topLeft.y() + cr.height() / 2 -
					qRound( d->staticText.size().height() ) / 2 );
		break;

		default :
		{
			if( d->margin != 0 )
				topLeft += QPoint( d->margin, d->margin );
		}
		break;
	}

	p.drawStaticText( topLeft, d->staticText );
}

void
TextLabel::resizeEvent( QResizeEvent * e )
{
	const QMargins margins = contentsMargins();

	d->staticText.setTextWidth( e->size().width() - 2 * frameWidth() -
		margins.left() - margins.right() - 2 * d->margin );

	e->accept();
}

} /* namespace QtMWidgets */
