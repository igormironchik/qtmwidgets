
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
#include "textlabel.hpp"

// Qt include.
#include <QStaticText>
#include <QPainter>
#include <QResizeEvent>
#include <QFontMetrics>


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
}; // class TextLabelPrivate

void
TextLabelPrivate::init()
{
	staticText.setTextFormat( Qt::AutoText );

	QTextOption opt;
	opt.setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
	opt.setFlags( QTextOption::IncludeTrailingSpaces );
	opt.setWrapMode( QTextOption::WordWrap );
	staticText.setTextOption( opt );

	staticText.setTextWidth( q->fontMetrics().averageCharWidth() * 10 );

	QSizePolicy sp( QSizePolicy::Preferred, QSizePolicy::Preferred );
	sp.setHeightForWidth( true );
	q->setSizePolicy( sp );
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

int
TextLabel::heightForWidth( int w ) const
{
	if( text().isEmpty() )
		return 2 * frameWidth();

	QStaticText st = d->staticText;
	st.setTextWidth( w );

	const QMargins margins = contentsMargins();

	return st.size().height() + 2 * frameWidth() + margins.top() +
		margins.bottom() + 2 * d->margin;
}

QSize
TextLabel::minimumSizeHint() const
{
	if( text().isEmpty() )
		return QSize( 2 * frameWidth(), 2 * frameWidth() );

	QStaticText st = d->staticText;

	if( width() > 0 )
		st.setTextWidth( width() );
	else
		st.setTextWidth( fontMetrics().averageCharWidth() * 10 );

	const QSizeF size = st.size();
	const int frame = 2 * frameWidth();

	const QMargins margins = contentsMargins();

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
