
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

#ifndef QTMWIDGETS__TEXTEDIT_HPP__INCLUDE
#define QTMWIDGETS__TEXTEDIT_HPP__INCLUDE

// QtMWidgets include.
#include "scrollarea.hpp"

// Qt include.
#include <QScopedPointer>
#include <QTextDocument>
#include <QTextCursor>
#include <QMimeData>
#include <QTextDocumentFragment>
#include <QClipboard>


namespace QtMWidgets {

//
// TextEditMimeData
//

class TextEditMimeData
	:	public QMimeData
{
public:
	inline TextEditMimeData( const QTextDocumentFragment & aFragment )
		:	fragment( aFragment )
	{
	}

	virtual QStringList formats() const;

protected:
	virtual QVariant retrieveData( const QString & mimeType,
		QVariant::Type type ) const;

private:
	void setup() const;

	mutable QTextDocumentFragment fragment;
}; // class TextEditMimeData


//
// TextEdit
//

class TextEditPrivate;

/*!
	\brief The TextEdit class provides a widget that is used to edit and
	display both plain and rich text.
*/
class TextEdit
	:	public ScrollArea
{
	Q_OBJECT

	/*!
		\property plainText

		This property gets and sets the text editor's contents as plain
		text. Previous contents are removed and undo/redo history is reset
		when the property is set.

		If the text edit has another content type, it will not be replaced
		by plain text if you call toPlainText(). The only exception to this
		is the non-break space, \e{nbsp;}, that will be converted into
		standard space.

		By default, for an editor with no contents, this property contains
		an empty string.

		\sa html
	*/
	Q_PROPERTY( QString plainText READ toPlainText WRITE setPlainText )
	/*!
		\property undoRedoEnabled
		\brief whether undo and redo are enabled

		Users are only able to undo or redo actions if this property is
		true, and if there is an action that can be undone (or redone).
	*/
	Q_PROPERTY( bool undoRedoEnabled READ isUndoRedoEnabled
		WRITE setUndoRedoEnabled )
	/*!
		\property document

		Document of the TextEdit.
	*/
	Q_PROPERTY( QTextDocument * document READ document WRITE setDocument
		DESIGNABLE false )
	/*!
		\property placeholderText

		The text that will be displayed to prompt user when
		nothing entered yet.
	*/
	Q_PROPERTY( QString placeholderText READ placeholderText
		WRITE setPlaceholderText )
	/*!
		\property readOnly

		Is this TextEdit read-only?
	*/
	Q_PROPERTY( bool readOnly READ isReadOnly WRITE setReadOnly )
	/*!
		\property documentTitle

		Title of the document
	*/
	Q_PROPERTY( QString documentTitle READ documentTitle
		WRITE setDocumentTitle )

signals:
	//! Text changed.
	void textChanged();
	/*!
		This signal is emitted whenever undo operations become available
		(\a available is true) or unavailable (\a available is false).
	*/
	void undoAvailable( bool b );
	/*!
		This signal is emitted whenever redo operations become available
		(\a available is true) or unavailable (\a available is false).
	*/
	void redoAvailable( bool b );
	/*!
		This signal is emitted when text is selected or de-selected
		in the text edit.

		When text is selected this signal will be emitted with \a yes set
		to true. If no text has been selected or if the selected text is
		de-selected this signal is emitted with \a yes set to false.

		If \a yes is true then copy() can be used to copy the selection
		to the clipboard. If \a yes is false then copy() does nothing.
	*/
	void copyAvailable( bool b );
	//! This signal is emitted whenever the selection changes.
	void selectionChanged();
	//! This signal is emitted whenever the position of the cursor changed.
	void cursorPositionChanged();

public:
	TextEdit( QWidget * parent = 0 );
	explicit TextEdit( const QString & text, QWidget * parent = 0 );
	virtual ~TextEdit();

	//! \return Document.
	QTextDocument * document() const;
	//! Set document.
	void setDocument( QTextDocument * document );

	//! \return Placeholder text.
	QString placeholderText() const;
	//! Set placeholder text.
	void setPlaceholderText( const QString & placeholderText );

	//! \return Text cursor.
	QTextCursor textCursor() const;
	//! Set text cursor.
	void setTextCursor( const QTextCursor & cursor );

	//! \return Is text edit in read only mode.
	bool isReadOnly() const;
	//! Set read only mode.
	void setReadOnly( bool ro );

	//! \return Document title.
	inline QString documentTitle() const
		{ return document()->metaInformation( QTextDocument::DocumentTitle ); }
	//! Set document title.
	inline void setDocumentTitle( const QString & title )
		{ document()->setMetaInformation( QTextDocument::DocumentTitle, title ); }

	//! \return Is undo-redo enabled.
	inline bool isUndoRedoEnabled() const
		{ return document()->isUndoRedoEnabled(); }
	//! Set undo-redo mode.
	inline void setUndoRedoEnabled( bool enable )
		{ document()->setUndoRedoEnabled( enable ); }

	/*!
		Finds the next occurrence of the string, \a exp, using the given
		\a options. Returns true if exp was found and changes the cursor
		to select the match; otherwise returns false.
	*/
	bool find( const QString & exp, QTextDocument::FindFlags options = 0 );

	//! \return Text as plain text.
	QString toPlainText() const;

	//! \return Text as HTML.
	QString toHtml() const;

	//! Ensure cursor visible.
	void ensureCursorVisible();

	//! \return Cursor for position.
	QTextCursor cursorForPosition( const QPoint & pos ) const;
	//! \return Cursor rect for the \a cursor cursor.
	QRect cursorRect( const QTextCursor & cursor ) const;
	//! \return Cursor rect.
	QRect cursorRect() const;

	/*!
		\return The reference of the anchor at position pos, or an empty
		string if no anchor exists at that point.
	*/
	QString anchorAt( const QPoint & pos ) const;

	//! \return Cursor width.
	int cursorWidth() const;
	//! Set cursor width.
	void setCursorWidth( int width );

	/*!
		Moves the cursor by performing the given operation.

		If mode is QTextCursor::KeepAnchor, the cursor selects the text it
		moves over. This is the same effect that the user achieves when they
		hold down the Shift key and move the cursor with the cursor keys.
	*/
	void moveCursor( QTextCursor::MoveOperation operation,
		QTextCursor::MoveMode mode = QTextCursor::MoveAnchor );

public slots:
	//! Set plain text.
	void setPlainText( const QString & text );
	//! Set HTML.
	void setHtml( const QString & text );
	//! Set text.
	void setText( const QString & text );

	//! Undo.
	void undo();
	//! Redo.
	void redo();

	//! Cut text.
	void cut();
	//! Copy text.
	void copy();
	//! Paste text.
	void paste( QClipboard::Mode mode = QClipboard::Clipboard );

	//! Clear.
	void clear();
	//! Select all.
	void selectAll();

	//! Insert plain text.
	void insertPlainText( const QString & text );
	//! Insert HTML.
	void insertHtml( const QString & text );
	//! Append text.
	void append( const QString & text );

	//! Scroll to anchor.
	void scrollToAnchor( const QString & name );

	virtual QVariant inputMethodQuery( Qt::InputMethodQuery property ) const;

protected:
	virtual void keyPressEvent( QKeyEvent * e );
	virtual void resizeEvent( QResizeEvent * e );
	virtual bool eventFilter( QObject * obj, QEvent * e );
	virtual void mousePressEvent( QMouseEvent * e );
	virtual void mouseReleaseEvent( QMouseEvent * e );
	virtual void focusInEvent( QFocusEvent * e );
	virtual void focusOutEvent( QFocusEvent * e );
	virtual void inputMethodEvent( QInputMethodEvent * e );

	virtual void scrollContentsBy( int dx, int dy );

private slots:
	void _q_cursorFlashTimer();
	void _q_cursorShifterPosChanged( const QPoint & pos );

private:
	Q_DISABLE_COPY( TextEdit )

	inline TextEditPrivate * d_func()
		{ return reinterpret_cast< TextEditPrivate* > ( d.data() ); }
	inline const TextEditPrivate * d_func() const
		{ return reinterpret_cast< const TextEditPrivate* >( d.data() ); }
}; // class TextEdit

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__TEXTEDIT_HPP__INCLUDE
