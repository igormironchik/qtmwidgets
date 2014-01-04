
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

#ifndef QTMWIDGETS__PICKER_HPP__INCLUDED
#define QTMWIDGETS__PICKER_HPP__INCLUDED

// Qt include.
#include <QScopedPointer>
#include <QWidget>
#include <QAbstractItemModel>
#include <QVariant>


namespace QtMWidgets {

//
// Picker
//

class PickerPrivate;

/*!
	Picker is a cylinder with strings where one string can
	be selected. Selected string is highlighted and has
	a tick.

	The interfase of the Picker is similar to the QComboBox interface.
	Picker like a QComboBox uses model/view framework too. By default
	picker uses QStandardItemModel as underlying model.
*/
class Picker
	:	public QWidget
{
	Q_OBJECT

	Q_ENUMS( InsertPolicy )

	Q_PROPERTY( int count READ count )
	Q_PROPERTY( int currentIndex READ currentIndex WRITE setCurrentIndex
		NOTIFY currentIndexChanged )
	Q_PROPERTY( QString currentText READ currentText WRITE setCurrentText
		NOTIFY currentTextChanged USER true )
	Q_PROPERTY( QVariant currentData READ currentData )
	Q_PROPERTY( InsertPolicy insertPolicy READ insertPolicy WRITE setInsertPolicy )
	Q_PROPERTY( int modelColumn READ modelColumn WRITE setModelColumn )
	Q_PROPERTY( int maxCount READ maxCount WRITE setMaxCount )

signals:
	/*!
		This signal is sent when the user chooses an item in the picker.
		The item's \a index is passed. Note that this signal is sent even
		when the choice is not changed. If you need to know when the
		choice actually changes, use signal currentIndexChanged().
	*/
	void activated( int index );
	/*!
		This signal is sent when the user chooses an item in the picker.
		The item's \a text is passed. Note that this signal is sent even
		when the choice is not changed. If you need to know when the
		choice actually changes, use signal currentIndexChanged().
	*/
	void activated( const QString & text );
	/*!
		This signal is sent whenever the currentIndex in the picker
		changes either through user interaction or programmatically. The
		item's \a index is passed or -1 if the Picker becomes empty or the
		currentIndex was reset.
	*/
	void currentIndexChanged( int index );
	/*!
		This signal is sent whenever the currentIndex in the picker
		changes either through user interaction or programmatically. The
		item's \a text is passed.
	*/
	void currentIndexChanged( const QString & text );
	/*!
		This signal is sent whenever currentText changes. The new value
		is passed as \a text.
	*/
	void currentTextChanged( const QString & text );

public:
	/*!
		This enum specifies what the Picker should do when a new string is
		entered by the user.
	*/
	enum InsertPolicy {
		//! The string will be inserted as the first item.
		InsertAtTop = 0,
		//! The string will be inserted after the last item.
		InsertAtBottom = 1,
		//! The string is inserted in the alphabetic order.
		InsertAlphabetically = 2
	}; // enum InsertPolicy

public:
	Picker( QWidget * parent = 0, Qt::WindowFlags f = 0 );

	~Picker();

	/*!
		\brief the number of items in the picker

		By default, for an empty picker, this property has a value of 0.
	*/
	int count() const;
	/*!
		\brief the maximum number of items allowed in the picker

		\note If you set the maximum number to be less then the current
		amount of items in the picker, the extra items will be
		truncated. This also applies if you have set an external model on
		the picker.

		By default, this property's value is derived from the highest
		signed integer available (typically 2147483647).
	*/
	int maxCount() const;
	/*!
		Set the maximum number of items allowed in the picker.
	*/
	void setMaxCount( int max );

	/*!
		Returns the index of the item containing the given \a text; otherwise
		returns -1.

		The \a flags specify how the items in the picker are searched.
	*/
	inline int findText( const QString & text,
		Qt::MatchFlags flags =
			static_cast< Qt::MatchFlags > ( Qt::MatchExactly | Qt::MatchCaseSensitive ) ) const
	{
		return findData( text, Qt::DisplayRole, flags );
	}

	/*!
		Returns the index of the item containing the given \a data for the
		given \a role; otherwise returns -1.

		The \a flags specify how the items in the combobox are searched.
	*/
	int findData( const QVariant & data, int role = Qt::UserRole,
		Qt::MatchFlags flags =
			static_cast< Qt::MatchFlags > ( Qt::MatchExactly | Qt::MatchCaseSensitive ) ) const;

	/*!
		\brief the policy used to determine where user-inserted items should
		appear in the picker

		The default value is \l InsertAtBottom, indicating that new items will appear
		at the bottom of the list of items.

		\sa InsertPolicy
	*/
	InsertPolicy insertPolicy() const;
	/*!
		Set the policy used to determine where user-inserted items should
		appear in the picker.

		\sa InsertPolicy
	*/
	void setInsertPolicy( InsertPolicy policy );

	/*!
		Returns the model used by the picker.
	*/
	QAbstractItemModel * model() const;
	/*!
		Sets the model to be \a model. \a model must not be 0.
		If you want to clear the contents of a model, call clear().

		\sa clear()
	*/
	void setModel( QAbstractItemModel * model );

	/*!
		Returns the root model item index for the items in the picker.

		\sa setRootModelIndex()
	*/
	QModelIndex rootModelIndex() const;
	/*!
		Sets the root model item \a index for the items in the picker.

		\sa rootModelIndex()
	*/
	void setRootModelIndex( const QModelIndex & index );

	/*!
		\brief the column in the model that is visible.

		By default, this property has a value of 0.
	*/
	int modelColumn() const;
	/*!
		Set the column in the model that is visible.
	*/
	void setModelColumn( int visibleColumn );

	/*!
		\brief the index of the current item in the picker.

		The current index can change when inserting or removing items.

		By default, for an empty picker or a picker in which no current
		item is set, this property has a value of -1.
	*/
	int currentIndex() const;
	/*!
		\brief the current text

		The current text is the value of the current item or
		an empty string if the picker is empty or no current item is set.

		The setter setCurrentText() if there is a matching text in the list,
		currentIndex is set to the corresponding index.
	*/
	QString currentText() const;
	/*!
		\brief the data for the current item

		By default, for an empty picker or a picker in which no current
		item is set, this property contains an invalid QVariant.
	*/
	QVariant currentData( int role = Qt::UserRole ) const;

	/*!
		Returns the text for the given \a index in the picker.
	*/
	QString itemText( int index ) const;
	/*!
		Returns the data for the given \a role in the given \a index in the
		picker, or QVariant::Invalid if there is no data for this role.
	*/
	QVariant itemData( int index, int role = Qt::UserRole ) const;

	/*!
		Adds an item to the picker with the given \a text, and
		containing the specified \a userData (stored in the Qt::UserRole).
		The item is appended to the list of existing items.
	*/
	inline void addItem( const QString & text,
		const QVariant & userData = QVariant() )
	{
		insertItem( count(), text, userData );
	}

	/*!
		Adds each of the strings in the given \a texts to the picker. Each item
		is appended to the list of existing items in turn.
	*/
	inline void addItems( const QStringList & texts )
	{
		insertItems( count(), texts );
	}

	/*!
		Inserts the \a text and \a userData (stored in the Qt::UserRole)
		into the picker at the given \a index.

		If the index is equal to or higher than the total number of items,
		the new item is appended to the list of existing items. If the
		index is zero or negative, the new item is prepended to the list
		of existing items.

		\sa insertItems()
	*/
	void insertItem( int index, const QString & text,
		const QVariant & userData = QVariant() );

	/*!
		Inserts the strings from the \a texts into the picker as separate items,
		starting at the \a index specified.

		If the index is equal to or higher than the total number of items,
		the new items are appended to the list of existing items. If the index
		is zero or negative, the new items are prepended to the list of existing
		items.

		\sa insertItem()
	*/
	void insertItems( int index, const QStringList & texts );

	/*!
		Removes the item at the given \a index from the picker.
		This will update the current index if the index is removed.

		This function does nothing if \a index is out of range.
	*/
	void removeItem( int index );

	/*!
		Sets the \a text for the item on the given \a index in the picker.
	*/
	void setItemText( int index, const QString & text );

	/*!
		Sets the data \a role for the item on the given \a index in the picker
		to the specified \a value.
	*/
	void setItemData( int index, const QVariant & value,
		int role = Qt::UserRole );

	QSize sizeHint() const;
	QSize minimumSizeHint() const;

public slots:
	/*!
		Clears the picker, removing all items.

		Note: If you have set an external model on the combobox this model
		will still be cleared when calling this function.
	*/
	void clear();
	/*!
		Set the current item to the item with index \a index.

		If index is out of range this function does nothing.
	*/
	void setCurrentIndex( int index );
	/*!
		Set the current item to item with text \a text.

		If there is no item with such text this function
		does nothing.
	*/
	void setCurrentText( const QString & text );

private slots:
	void _q_emitCurrentIndexChanged( const QModelIndex & index );
	void _q_dataChanged( const QModelIndex &, const QModelIndex & );
	void _q_updateIndexBeforeChange();
	void _q_rowsInserted( const QModelIndex & parent, int start, int end );
	void _q_rowsRemoved( const QModelIndex & parent, int start, int end );
	void _q_modelDestroyed();
	void _q_modelReset();

private:
	friend class PickerPrivate;

	Q_DISABLE_COPY( Picker )

	QScopedPointer< PickerPrivate > d;
}; // class Picker

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PICKER_HPP__INCLUDED
