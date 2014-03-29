
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
#include "private/cursorshifter.hpp"

// Qt include.
#include <QAbstractTextDocumentLayout>
#include <QTextLayout>
#include <QTextBlock>
#include <QPainter>
#include <QApplication>
#include <QBuffer>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QTimer>


namespace QtMWidgets {

static QTextLine currentTextLine( const QTextCursor & cursor )
{
	const QTextBlock block = cursor.block();

	if( !block.isValid() )
		return QTextLine();

	const QTextLayout * layout = block.layout();

	if( !layout )
		return QTextLine();

	const int relativePos = cursor.position() - block.position();

	return layout->lineForTextPosition( relativePos );
}

//
// TextEditMimeData
//

QStringList
TextEditMimeData::formats() const
{
	if( !fragment.isEmpty() )
		return QStringList() << QString::fromLatin1( "text/plain" )
			<< QString::fromLatin1( "text/html" )
			<< QString::fromLatin1( "application/vnd.oasis.opendocument.text" );
	else
		return QMimeData::formats();
}

QVariant
TextEditMimeData::retrieveData( const QString & mimeType,
	QVariant::Type type ) const
{
	if( !fragment.isEmpty() )
		setup();

	return QMimeData::retrieveData( mimeType, type );
}

void
TextEditMimeData::setup() const
{
	TextEditMimeData * that = const_cast< TextEditMimeData* > ( this );
	that->setData( QLatin1String( "text/html" ),
		fragment.toHtml( "utf-8" ).toUtf8() );

	{
		QBuffer buffer;
		QTextDocumentWriter writer( &buffer, "ODF" );
		writer.write( fragment );
		buffer.close();
		that->setData( QLatin1String(
			"application/vnd.oasis.opendocument.text" ), buffer.data() );
	}

	that->setText( fragment.toPlainText() );
	fragment = QTextDocumentFragment();
}


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
		,	cursorFlashTimer( 0 )
		,	cursorShown( true )
		,	hasFocus( false )
		,	shifter( 0 )
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
	QMimeData * createMimeDataFromSelection() const;
	void insertFromMimeData( const QMimeData * source );
	bool cursorMoveKeyEvent( QKeyEvent * e );
	void pageUpDown( QTextCursor::MoveOperation op,
		QTextCursor::MoveMode moveMode );

	inline TextEdit * q_func() { return static_cast< TextEdit* >( q ); }
	inline const TextEdit * q_func() const { return static_cast< const TextEdit* >( q ); }

	QTextDocument * doc;
	QString placeholderText;
	QTextCursor cursor;
	bool isReadOnly;
	int cursorWidth;
	QTimer * cursorFlashTimer;
	bool cursorShown;
	bool hasFocus;
	CursorShifter * shifter;
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

	cursorFlashTimer = new QTimer( q );
	cursorFlashTimer->start( QApplication::cursorFlashTime() / 2 );

	QObject::connect( cursorFlashTimer, SIGNAL( timeout() ),
		q, SLOT( _q_cursorFlashTimer() ) );

	shifter = new CursorShifter( q );

	QObject::connect( shifter, SIGNAL( posChanged( const QPoint & ) ),
		q, SLOT( _q_cursorShifterPosChanged( const QPoint & ) ) );
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
	TextEdit * q = q_func();

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
	}

	const int xOffset = topLeftCorner.x();
	const int yOffset = topLeftCorner.y();

	QRect r = e->rect();
	p.translate( -xOffset, -yOffset );
	r.translate( xOffset, yOffset );

	doc->drawContents( &p, r );

	if( hasFocus && cursorShown )
	{
		const QRect c = q->cursorRect();

		QPen pen( Qt::black );
		pen.setWidth( cursorWidth );
		p.setPen( pen );
		p.drawLine( c.center().x(), c.top(), c.center().x(), c.bottom() );
	}
}

QMimeData *
TextEditPrivate::createMimeDataFromSelection() const
{
	const QTextDocumentFragment fragment( cursor );
	return new TextEditMimeData( fragment );
}

void
TextEditPrivate::insertFromMimeData( const QMimeData * source )
{
	TextEdit * q = q_func();

	if ( q->isReadOnly() || !source )
		return;

	bool hasData = false;

	QTextDocumentFragment fragment;

	if( source->hasFormat( QLatin1String( "application/x-qrichtext" ) ) )
	{
		QString richtext = QString::fromUtf8( source->data(
			QLatin1String( "application/x-qrichtext" ) ) );
		richtext.prepend( QLatin1String(
			"<meta name=\"qrichtext\" content=\"1\" />" ) );
		fragment = QTextDocumentFragment::fromHtml( richtext, doc );
		hasData = true;
	}
	else if( source->hasHtml() )
	{
		fragment = QTextDocumentFragment::fromHtml( source->html(), doc );
		hasData = true;
	}
	else
	{
		QString text = source->text();
		if( !text.isNull() )
		{
			fragment = QTextDocumentFragment::fromPlainText( text );
			hasData = true;
		}
	}

	if( hasData )
		cursor.insertFragment( fragment );

	q->ensureCursorVisible();
}

bool
TextEditPrivate::cursorMoveKeyEvent( QKeyEvent * e )
{
	TextEdit * q = q_func();

	if( cursor.isNull() )
		return false;

	const QTextCursor oldSelection = cursor;
	const int oldCursorPos = cursor.position();

	QTextCursor::MoveMode mode = QTextCursor::MoveAnchor;
	QTextCursor::MoveOperation op = QTextCursor::NoMove;

	if( e == QKeySequence::MoveToNextChar )
		op = QTextCursor::Right;
	else if( e == QKeySequence::MoveToPreviousChar )
		op = QTextCursor::Left;
	else if( e == QKeySequence::SelectNextChar )
	{
	   op = QTextCursor::Right;
	   mode = QTextCursor::KeepAnchor;
	}
	else if( e == QKeySequence::SelectPreviousChar )
	{
		op = QTextCursor::Left;
		mode = QTextCursor::KeepAnchor;
	}
	else if( e == QKeySequence::SelectNextWord )
	{
		op = QTextCursor::WordRight;
		mode = QTextCursor::KeepAnchor;
	}
	else if( e == QKeySequence::SelectPreviousWord )
	{
		op = QTextCursor::WordLeft;
		mode = QTextCursor::KeepAnchor;
	}
	else if( e == QKeySequence::SelectStartOfLine )
	{
		op = QTextCursor::StartOfLine;
		mode = QTextCursor::KeepAnchor;
	}
	else if( e == QKeySequence::SelectEndOfLine )
	{
		op = QTextCursor::EndOfLine;
		mode = QTextCursor::KeepAnchor;
	}
	else if( e == QKeySequence::SelectStartOfBlock )
	{
		op = QTextCursor::StartOfBlock;
		mode = QTextCursor::KeepAnchor;
	}
	else if( e == QKeySequence::SelectEndOfBlock )
	{
		op = QTextCursor::EndOfBlock;
		mode = QTextCursor::KeepAnchor;
	}
	else if( e == QKeySequence::SelectStartOfDocument )
	{
		op = QTextCursor::Start;
		mode = QTextCursor::KeepAnchor;
	}
	else if( e == QKeySequence::SelectEndOfDocument )
	{
		op = QTextCursor::End;
		mode = QTextCursor::KeepAnchor;
	}
	else if( e == QKeySequence::SelectPreviousLine )
	{
		op = QTextCursor::Up;
		mode = QTextCursor::KeepAnchor;
	}
	else if( e == QKeySequence::SelectNextLine )
	{
		op = QTextCursor::Down;
		mode = QTextCursor::KeepAnchor;

		{
			QTextBlock block = cursor.block();
			QTextLine line = currentTextLine( cursor );
			if( !block.next().isValid()
				&& line.isValid()
				&& line.lineNumber() == block.layout()->lineCount() - 1 )
					op = QTextCursor::End;
		}
	}
	else if( e == QKeySequence::MoveToNextWord )
		op = QTextCursor::WordRight;
	else if( e == QKeySequence::MoveToPreviousWord )
		op = QTextCursor::WordLeft;
	else if( e == QKeySequence::MoveToEndOfBlock )
		op = QTextCursor::EndOfBlock;
	else if( e == QKeySequence::MoveToStartOfBlock )
		op = QTextCursor::StartOfBlock;
	else if( e == QKeySequence::MoveToNextLine )
		op = QTextCursor::Down;
	else if( e == QKeySequence::MoveToPreviousLine )
		op = QTextCursor::Up;
	else if( e == QKeySequence::MoveToPreviousLine )
		op = QTextCursor::Up;
	else if( e == QKeySequence::MoveToStartOfLine )
		op = QTextCursor::StartOfLine;
	else if( e == QKeySequence::MoveToEndOfLine )
		op = QTextCursor::EndOfLine;
	else if( e == QKeySequence::MoveToStartOfDocument )
		op = QTextCursor::Start;
	else if( e == QKeySequence::MoveToEndOfDocument )
		op = QTextCursor::End;
	else
		return false;

	bool visualNavigation = cursor.visualNavigation();
	cursor.setVisualNavigation( true );
	const bool moved = cursor.movePosition( op, mode );
	cursor.setVisualNavigation( visualNavigation );
	q->ensureCursorVisible();

	bool ignoreNavigationEvents = false;
	bool isNavigationEvent = e->key() == Qt::Key_Up || e->key() == Qt::Key_Down;

#ifdef QT_KEYPAD_NAVIGATION
	ignoreNavigationEvents = ignoreNavigationEvents ||
		QApplication::keypadNavigationEnabled();
	isNavigationEvent = isNavigationEvent ||
		( QApplication::navigationMode() == Qt::NavigationModeKeypadDirectional
		 && ( e->key() == Qt::Key_Left || e->key() == Qt::Key_Right ) );
#else
	isNavigationEvent = isNavigationEvent || e->key() == Qt::Key_Left ||
		e->key() == Qt::Key_Right;
#endif

	if( moved )
	{
		if( cursor.position() != oldCursorPos )
			emit q->cursorPositionChanged();
	}
	else if( ignoreNavigationEvents && isNavigationEvent &&
			oldSelection.anchor() == cursor.anchor())
	{
		return false;
	}

	return true;
}

void
TextEditPrivate::pageUpDown( QTextCursor::MoveOperation op,
	QTextCursor::MoveMode moveMode )
{
	TextEdit * q = q_func();

	bool moved = false;
	qreal lastY = q->cursorRect( cursor ).top();
	qreal distance = 0;
	// move using movePosition to keep the cursor's x
	do {
		qreal y = q->cursorRect( cursor ).top();
		distance += qAbs( y - lastY );
		lastY = y;
		moved = cursor.movePosition( op, moveMode );
	} while( moved && distance < viewport->height() );

	if( moved )
	{
		if( op == QTextCursor::Up )
			cursor.movePosition( QTextCursor::Down, moveMode );
		else
			cursor.movePosition( QTextCursor::Up, moveMode );
	}

	q->ensureCursorVisible();
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

	d->scrollContentsBy( d->topLeftCorner.x() - crect.topLeft().x(),
		d->topLeftCorner.y() - crect.topLeft().y() );

	scrollContentsBy( 0, 0 );

	startScrollIndicatorsAnimation();
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
TextEdit::cut()
{
	TextEditPrivate * d = d_func();

	if( isReadOnly() || !d->cursor.hasSelection() )
		return;

	copy();

	d->cursor.removeSelectedText();
}

void
TextEdit::copy()
{
	TextEditPrivate * d = d_func();

	if( !d->cursor.hasSelection() )
		return;

	QMimeData * data = d->createMimeDataFromSelection();

	QApplication::clipboard()->setMimeData( data );
}

void
TextEdit::paste( QClipboard::Mode mode )
{
	TextEditPrivate * d = d_func();

	const QMimeData * md = QApplication::clipboard()->mimeData( mode );

	if( md )
		d->insertFromMimeData( md );
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
	TextEditPrivate * d = d_func();

	d->shifter->immediatelyHide();

	if( e == QKeySequence::SelectAll )
	{
		e->accept();
		selectAll();
		return;
	}
	else if( e == QKeySequence::Copy )
	{
		e->accept();
		copy();
		return;
	}

	if( e == QKeySequence::SelectPreviousPage )
	{
		e->accept();
		d->pageUpDown( QTextCursor::Up, QTextCursor::KeepAnchor );
		return;
	}
	else if( e ==QKeySequence::SelectNextPage )
	{
		e->accept();
		d->pageUpDown( QTextCursor::Down, QTextCursor::KeepAnchor );
		return;
	}

	if( !isReadOnly() )
	{
		if( e == QKeySequence::MoveToPreviousPage )
		{
			e->accept();
			d->pageUpDown( QTextCursor::Up, QTextCursor::MoveAnchor );
			return;
		}
		else if( e == QKeySequence::MoveToNextPage )
		{
			e->accept();
			d->pageUpDown( QTextCursor::Down, QTextCursor::MoveAnchor );
			return;
		}
	}

	if( d->cursorMoveKeyEvent( e ) )
	{
		e->accept();
		ensureCursorVisible();
		return;
	}

	if( isReadOnly() )
	{
		e->ignore();
		return;
	}

	if( e->key() == Qt::Key_Direction_L || e->key() == Qt::Key_Direction_R )
	{
		QTextBlockFormat fmt;
		fmt.setLayoutDirection( ( e->key() == Qt::Key_Direction_L ) ?
			Qt::LeftToRight : Qt::RightToLeft );
		d->cursor.mergeBlockFormat( fmt );
		e->accept();
		ensureCursorVisible();
	}

	if( e->key() == Qt::Key_Backspace &&
		!( e->modifiers() & ~Qt::ShiftModifier ) )
	{
		QTextBlockFormat blockFmt = d->cursor.blockFormat();
		QTextList * list = d->cursor.currentList();

		if( list && d->cursor.atBlockStart() && !d->cursor.hasSelection() )
		{
			list->remove( d->cursor.block() );
		}
		else if( d->cursor.atBlockStart() && blockFmt.indent() > 0 )
		{
			blockFmt.setIndent( blockFmt.indent() - 1 );
			d->cursor.setBlockFormat( blockFmt );
		}
		else
		{
			QTextCursor localCursor = d->cursor;
			localCursor.deletePreviousChar();
		}

		e->accept();
		ensureCursorVisible();
	}
	else if( e == QKeySequence::InsertParagraphSeparator )
	{
		d->cursor.insertBlock();
		e->accept();
		ensureCursorVisible();
	}
	else if( e == QKeySequence::InsertLineSeparator )
	{
		d->cursor.insertText( QString( QChar::LineSeparator ) );
		e->accept();
		ensureCursorVisible();
	}
	else if( e == QKeySequence::Undo )
		undo();
	else if( e == QKeySequence::Redo )
		redo();
	else if( e == QKeySequence::Cut )
		cut();
	else if( e == QKeySequence::Paste )
	{
		QClipboard::Mode mode = QClipboard::Clipboard;
		if( QGuiApplication::clipboard()->supportsSelection() )
		{
			if( e->modifiers() == ( Qt::CTRL | Qt::SHIFT ) &&
				e->key() == Qt::Key_Insert )
					mode = QClipboard::Selection;
		}

		paste( mode );
	}
	else if( e == QKeySequence::Delete )
	{
		QTextCursor localCursor = d->cursor;
		localCursor.deleteChar();
	}
	else if( e == QKeySequence::DeleteEndOfWord )
	{
		if ( !d->cursor.hasSelection() )
			d->cursor.movePosition( QTextCursor::NextWord,
				QTextCursor::KeepAnchor );
		d->cursor.removeSelectedText();
	}
	else if( e == QKeySequence::DeleteStartOfWord )
	{
		if( !d->cursor.hasSelection() )
			d->cursor.movePosition( QTextCursor::PreviousWord,
				QTextCursor::KeepAnchor );
		d->cursor.removeSelectedText();
	}
	else if( e == QKeySequence::DeleteEndOfLine )
	{
		QTextBlock block = d->cursor.block();
		if( d->cursor.position() == block.position() + block.length() - 2 )
			d->cursor.movePosition( QTextCursor::Right,
				QTextCursor::KeepAnchor );
		else
			d->cursor.movePosition( QTextCursor::EndOfBlock,
				QTextCursor::KeepAnchor );
		d->cursor.removeSelectedText();
	}
	else
	{
		QString text = e->text();

		if( !text.isEmpty() &&
			( text.at( 0 ).isPrint() || text.at( 0 ) == QLatin1Char( '\t' ) ) )
				d->cursor.insertText( text );
		else
		{
			e->ignore();
			return;
		}
	}

	e->accept();
	ensureCursorVisible();
	setScrolledAreaSize( d->doc->size().toSize() );
	viewport()->update();
}

void
TextEdit::resizeEvent( QResizeEvent * e )
{
	ScrollArea::resizeEvent( e );

	TextEditPrivate * d = d_func();

	d->doc->setTextWidth( viewport()->width() );

	setScrolledAreaSize( d->doc->size().toSize() );
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
TextEdit::mousePressEvent( QMouseEvent * e )
{
	TextEditPrivate * d = d_func();

	const QTextCursor c = cursorForPosition( d->mapToContents( e->pos() ) );

	setTextCursor( c );

	if( !isReadOnly() && !d->doc->isEmpty() )
	{
		const QRect cr = cursorRect();

		const QPoint pos = mapToGlobal( QPoint( cr.center().x(),
			cr.y() + cr.height() ) );

		d->shifter->setCursorPos( pos );
		d->shifter->popup();
	}

	AbstractScrollArea::mousePressEvent( e );
}

void
TextEdit::focusInEvent( QFocusEvent * e )
{
	TextEditPrivate * d = d_func();

	d->hasFocus = true;

	AbstractScrollArea::focusInEvent( e );
}

void
TextEdit::focusOutEvent( QFocusEvent * e )
{
	TextEditPrivate * d = d_func();

	d->hasFocus = false;

	d->shifter->immediatelyHide();

	AbstractScrollArea::focusOutEvent( e );
}

void
TextEdit::scrollContentsBy( int dx, int dy )
{
	Q_UNUSED( dx )
	Q_UNUSED( dy )

	viewport()->update();
}

void
TextEdit::_q_cursorFlashTimer()
{
	TextEditPrivate * d = d_func();

	d->cursorShown = !d->cursorShown;;

	if( d->hasFocus )
		viewport()->update( cursorRect() );
}

void
TextEdit::_q_cursorShifterPosChanged( const QPoint & pos )
{
	TextEditPrivate * d = d_func();

	const int oldPos = d->cursor.position();
	QPoint p = mapFromGlobal( pos );
	p.setY( p.y() - fontMetrics().ascent() / 2 );
	const int newPos = cursorForPosition( p ).position();

	if( oldPos != newPos )
	{
		d->cursor.setPosition( newPos );

		viewport()->repaint();

		const QRect cr = cursorRect();

		d->shifter->setCursorPos( mapToGlobal( QPoint( cr.center().x(),
			cr.y() + cr.height() ) ) );
	}
}

} /* namespace QtMWidgets */
