
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
#include "textedit.hpp"
#include "private/scrollarea_p.hpp"

// Qt include.
#include <QAbstractTextDocumentLayout>
#include <QTextLayout>
#include <QTextBlock>
#include <QPainter>


namespace QtMWidgets {

//
// TextEditPrivate
//

class TextEditPrivate
	:	public ScrollAreaPrivate
{
public:
	TextEditPrivate( TextEdit * parent )
		:	ScrollAreaPrivate( parent )
		,	doc( 0 )
		,	isReadOnly( false )
		,	cursorWidth( 1 )
	{
	}

	virtual ~TextEditPrivate()
	{
	}

	void init();

	inline QPoint mapToContents( const QPoint & point ) const
	{
		return QPoint( point.x() + topLeftCorner.x(),
			point.y() + topLeftCorner.y() );
	}

	QRectF rectForPosition( int pos ) const;
	QPointF anchorPosition( const QString & name ) const;
	void paintContents( QPaintEvent * e );

	inline TextEdit * q_func() { return static_cast< TextEdit* >( q ); }
	inline const TextEdit * q_func() const { return static_cast< const TextEdit* >( q ); }

	QTextDocument * doc;
	QString placeholderText;
	QTextCursor cursor;
	bool isReadOnly;
	int cursorWidth;
}; // class TextEditPrivate

void
TextEditPrivate::init()
{
	TextEdit * q = q_func();

	doc = new QTextDocument( q );
	cursor = QTextCursor( doc );

	doc->setPageSize( QSize( 0, 0 ) );
	doc->documentLayout()->setPaintDevice( q->viewport() );
	doc->setDefaultFont( q->font() );
	doc->setUndoRedoEnabled( false ); // flush undo buffer.
	doc->setUndoRedoEnabled( true );

	q->viewport()->setBackgroundRole( QPalette::Base );
	q->setFocusPolicy( Qt::WheelFocus );
	q->setAttribute( Qt::WA_KeyCompression );
	q->setAttribute( Qt::WA_InputMethodEnabled );
	q->setInputMethodHints( Qt::ImhMultiLine );
	q->viewport()->setCursor( Qt::IBeamCursor );

	q->viewport()->installEventFilter( q );
}

QRectF
TextEditPrivate::rectForPosition( int position ) const
{
	const QTextBlock block = doc->findBlock( position );

	if( !block.isValid() )
		return QRectF();

	const QAbstractTextDocumentLayout * docLayout = doc->documentLayout();
	const QTextLayout * layout = block.layout();
	const QPointF layoutPos = docLayout->blockBoundingRect( block ).topLeft();
	int relativePos = position - block.position();

	QTextLine line = layout->lineForTextPosition( relativePos );

	QRectF r;

	if( line.isValid() )
	{
		qreal x = line.cursorToX( relativePos );

		r = QRectF( layoutPos.x() + x, layoutPos.y() + line.y(),
			cursorWidth, line.height() );
	}
	else
	{
		QFontMetrics fm( doc->defaultFont() );

		r = QRectF( layoutPos.x(), layoutPos.y(), cursorWidth,
			fm.ascent() + fm.descent() );
	}

	return r;
}

QPointF
TextEditPrivate::anchorPosition( const QString & name ) const
{
	if( name.isEmpty() )
		return QPointF();

	QRectF r;

	for( QTextBlock block = doc->begin(); block.isValid(); block = block.next() )
	{
		QTextCharFormat format = block.charFormat();
		if( format.isAnchor() && format.anchorNames().contains( name ) )
		{
			r = rectForPosition( block.position() );
			break;
		}

		for( QTextBlock::Iterator it = block.begin(); !it.atEnd(); ++it )
		{
			QTextFragment fragment = it.fragment();
			format = fragment.charFormat();
			if( format.isAnchor() && format.anchorNames().contains( name ) )
			{
				r = rectForPosition( fragment.position() );
				block = QTextBlock();
				break;
			}
		}
	}

	if( !r.isValid() )
		return QPointF();

	return QPointF( 0, r.top() );
}

void
TextEditPrivate::paintContents( QPaintEvent * e )
{
	QPainter p( viewport );

	if( doc->isEmpty() && !placeholderText.isEmpty() )
	{
		const QRect r = viewport->rect();

		QColor col = q->palette().text().color();
		col.setAlpha( 128 );
		p.setPen( col );
		const int margin = int( doc->documentMargin() );
		p.drawText( r.adjusted( margin, margin, -margin, -margin ),
			Qt::AlignTop | Qt::TextWordWrap, placeholderText );

		return;
	}
}


//
// TextEdit
//

TextEdit::TextEdit( QWidget * parent )
	:	ScrollArea( new TextEditPrivate( this ), parent )
{
	TextEditPrivate * d = d_func();

	d->init();
}

TextEdit::TextEdit( const QString & text, QWidget * parent )
	:	ScrollArea( new TextEditPrivate( this ), parent )
{
	TextEditPrivate * d = d_func();

	d->init();

	if( !text.isEmpty() )
		setHtml( text );
}

TextEdit::~TextEdit()
{
}

QTextDocument *
TextEdit::document() const
{
	const TextEditPrivate * d = d_func();

	return d->doc;
}

void
TextEdit::setDocument( QTextDocument * document )
{
	TextEditPrivate * d = d_func();

	QTextDocument * doc = d->doc;

	d->doc = document;
	d->cursor = QTextCursor( d->doc );

	doc->deleteLater();
}

QString
TextEdit::placeholderText() const
{
	const TextEditPrivate * d = d_func();

	return d->placeholderText;
}

void
TextEdit::setPlaceholderText( const QString & placeholderText )
{
	TextEditPrivate * d = d_func();

	if( d->placeholderText != placeholderText )
	{
		d->placeholderText = placeholderText;

		if( d->doc->isEmpty() )
			update();
	}
}

QTextCursor
TextEdit::textCursor() const
{
	const TextEditPrivate * d = d_func();

	return d->cursor;
}

void
TextEdit::setTextCursor( const QTextCursor & cursor )
{
	TextEditPrivate * d = d_func();

	const bool posChanged = cursor.position() != d->cursor.position();

	d->cursor = cursor;
	ensureCursorVisible();

	if( posChanged )
		emit cursorPositionChanged();
}

bool
TextEdit::isReadOnly() const
{
	const TextEditPrivate * d = d_func();

	return d->isReadOnly;
}

void
TextEdit::setReadOnly( bool ro )
{
	TextEditPrivate * d = d_func();

	d->isReadOnly = ro;
}

bool
TextEdit::find( const QString & exp, QTextDocument::FindFlags options )
{
	TextEditPrivate * d = d_func();

	QTextCursor search = d->doc->find( exp, d->cursor, options );

	if( search.isNull() )
		return false;

	setTextCursor( search );

	return true;
}

QString
TextEdit::toPlainText() const
{
	const TextEditPrivate * d = d_func();

	return d->doc->toPlainText();
}

QString
TextEdit::toHtml() const
{
	const TextEditPrivate * d = d_func();

	return d->doc->toHtml();
}

void
TextEdit::ensureCursorVisible()
{
	TextEditPrivate * d = d_func();

	QRectF crect = d->rectForPosition( d->cursor.position() )
		.adjusted( -5, 0, 5, 0 );

	scrollContentsBy( crect.topLeft().x() - d->topLeftCorner.x(),
		crect.topLeft().y() - d->topLeftCorner.y() );
}

QTextCursor
TextEdit::cursorForPosition( const QPoint & pos ) const
{
	const TextEditPrivate * d = d_func();

	int cursorPos = d->doc->documentLayout()->hitTest(
		d->mapToContents( pos ), Qt::FuzzyHit );

	if( cursorPos == -1 )
		cursorPos = 0;

	QTextCursor c( d->doc );

	c.setPosition( cursorPos );

	return c;
}

QRect
TextEdit::cursorRect( const QTextCursor & cursor ) const
{
	const TextEditPrivate * d = d_func();

	return d->rectForPosition( cursor.position() ).toRect();
}

QRect
TextEdit::cursorRect() const
{
	const TextEditPrivate * d = d_func();

	return cursorRect( d->cursor );
}

QString
TextEdit::anchorAt( const QPoint & pos ) const
{
	const TextEditPrivate * d = d_func();

	return d->doc->documentLayout()->anchorAt( d->mapToContents( pos ) );
}

int
TextEdit::cursorWidth() const
{
	const TextEditPrivate * d = d_func();

	return d->cursorWidth;
}

void
TextEdit::setCursorWidth( int width )
{
	TextEditPrivate * d = d_func();

	if( d->cursorWidth != width )
		d->cursorWidth = width;
}

void
TextEdit::moveCursor( QTextCursor::MoveOperation operation,
	QTextCursor::MoveMode mode )
{
	TextEditPrivate * d = d_func();

	const bool moved = d->cursor.movePosition( operation, mode );
	ensureCursorVisible();

	if( moved )
		emit cursorPositionChanged();
}

void
TextEdit::setPlainText( const QString & text )
{
	TextEditPrivate * d = d_func();

	const bool previousUndoRedoState = isUndoRedoEnabled();

	const QTextCharFormat charFormatForInsertion = d->cursor.charFormat();
	d->cursor = QTextCursor();
	QTextCursor formatCursor( d->doc );
	formatCursor.beginEditBlock();
	d->doc->setPlainText( text );
	d->doc->setUndoRedoEnabled( false );
	formatCursor.select( QTextCursor::Document );
	formatCursor.setCharFormat( charFormatForInsertion );
	formatCursor.endEditBlock();

	d->cursor = QTextCursor( d->doc );

	emit textChanged();

	d->doc->setUndoRedoEnabled( previousUndoRedoState );
	d->doc->setModified( false );

	ensureCursorVisible();

	emit cursorPositionChanged();
}

void
TextEdit::setHtml( const QString & text )
{
	TextEditPrivate * d = d_func();

	const bool previousUndoRedoState = isUndoRedoEnabled();

	d->cursor = QTextCursor();

	d->doc->setHtml( text );

	d->cursor = QTextCursor( d->doc );

	emit textChanged();

	d->doc->setUndoRedoEnabled( previousUndoRedoState );
	d->doc->setModified( false );

	ensureCursorVisible();

	emit cursorPositionChanged();
}

void
TextEdit::setText( const QString & text )
{
	Qt::TextFormat format =
		Qt::mightBeRichText( text ) ? Qt::RichText : Qt::PlainText;

	if( format == Qt::RichText )
		setHtml( text );
	else
		setPlainText( text );
}

void
TextEdit::undo()
{
	TextEditPrivate * d = d_func();

	const int oldCursorPos = d->cursor.position();

	d->doc->undo( &d->cursor );

	if( d->cursor.position() != oldCursorPos )
		emit cursorPositionChanged();

	ensureCursorVisible();
}

void
TextEdit::redo()
{
	TextEditPrivate * d = d_func();

	const int oldCursorPos = d->cursor.position();

	d->doc->redo( &d->cursor );

	if( d->cursor.position() != oldCursorPos )
		emit cursorPositionChanged();

	ensureCursorVisible();
}

void
TextEdit::clear()
{
	setText( QString() );
}

void
TextEdit::selectAll()
{
	TextEditPrivate * d = d_func();

	d->cursor.select( QTextCursor::Document );
}

void
TextEdit::insertPlainText( const QString & text )
{
	TextEditPrivate * d = d_func();

	d->cursor.insertText( text );
}

void
TextEdit::insertHtml( const QString & text )
{
	TextEditPrivate * d = d_func();

	d->cursor.insertHtml( text );
}

void
TextEdit::append( const QString & text )
{
	TextEditPrivate * d = d_func();

	QTextCursor tmp( d->doc );
	tmp.beginEditBlock();
	tmp.movePosition( QTextCursor::End );

	if( !d->doc->isEmpty() )
		tmp.insertBlock( d->cursor.blockFormat(), d->cursor.charFormat() );
	else
		tmp.setCharFormat( d->cursor.charFormat() );

	// preserve the char format
	QTextCharFormat oldCharFormat = d->cursor.charFormat();

	if( Qt::mightBeRichText( text ) )
		tmp.insertHtml( text );
	else
		tmp.insertText( text );

	if( !d->cursor.hasSelection() )
		d->cursor.setCharFormat( oldCharFormat );

	tmp.endEditBlock();
}

void
TextEdit::scrollToAnchor( const QString & name )
{
	TextEditPrivate * d = d_func();

	if( name.isEmpty() )
		return;

	QPointF p = d->anchorPosition( name );

	const int newPosition = qRound( p.y() );

	scrollContentsBy( -d->topLeftCorner.x(),
		newPosition - d->topLeftCorner.y() );
}

void
TextEdit::keyPressEvent( QKeyEvent * e )
{

}

void
TextEdit::keyReleaseEvent( QKeyEvent * e )
{

}

void
TextEdit::resizeEvent( QResizeEvent * e )
{
	ScrollArea::resizeEvent( e );
}

bool
TextEdit::eventFilter( QObject * obj, QEvent * e )
{
	if( obj == viewport() && e->type() == QEvent::Paint )
	{
		TextEditPrivate * d = d_func();

		QPaintEvent * pe = static_cast< QPaintEvent* > ( e );

		d->paintContents( pe );

		return true;
	}

	return false;
}

void
TextEdit::scrollContentsBy( int dx, int dy )
{

}

} /* namespace QtMWidgets */
