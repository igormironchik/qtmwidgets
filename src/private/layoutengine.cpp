
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

// QtMWidgets include.
#include "layoutengine.hpp"


namespace QtMWidgets {

//
// qSmartMinSize
//

QSize qSmartMinSize( const QSize & sizeHint, const QSize & minSizeHint,
	const QSize & minSize, const QSize & maxSize,
	const QSizePolicy & sizePolicy )
{
	QSize s( 0, 0 );

	if( sizePolicy.horizontalPolicy() != QSizePolicy::Ignored )
	{
		if( sizePolicy.horizontalPolicy() & QSizePolicy::ShrinkFlag )
			s.setWidth( minSizeHint.width() );
		else
			s.setWidth( qMax( sizeHint.width(), minSizeHint.width() ) );
	}

	if( sizePolicy.verticalPolicy() != QSizePolicy::Ignored )
	{
		if( sizePolicy.verticalPolicy() & QSizePolicy::ShrinkFlag )
			s.setHeight( minSizeHint.height() );
		else
			s.setHeight( qMax( sizeHint.height(), minSizeHint.height() ) );
	}

	s = s.boundedTo( maxSize );

	if( minSize.width() > 0 )
		s.setWidth( minSize.width() );

	if( minSize.height() > 0 )
		s.setHeight( minSize.height() );

	return s.expandedTo( QSize( 0, 0 ) );
}

//
// qSmartMinSize
//

QSize qSmartMinSize( const QWidget * w )
{
	return qSmartMinSize( w->sizeHint(), w->minimumSizeHint(),
		w->minimumSize(), w->maximumSize(),
		w->sizePolicy() );
}

//
// qSmartMaxSize
//

QSize qSmartMaxSize( const QSize & sizeHint,
	const QSize & minSize, const QSize & maxSize,
	const QSizePolicy & sizePolicy, Qt::Alignment align )
{
	if( align & Qt::AlignHorizontal_Mask && align & Qt::AlignVertical_Mask )
		return QSize( QLAYOUTSIZE_MAX, QLAYOUTSIZE_MAX );

	QSize s = maxSize;
	QSize hint = sizeHint.expandedTo( minSize );

	if( s.width() == QWIDGETSIZE_MAX && !( align & Qt::AlignHorizontal_Mask ) )
		if( !( sizePolicy.horizontalPolicy() & QSizePolicy::GrowFlag ) )
			s.setWidth( hint.width() );

	if( s.height() == QWIDGETSIZE_MAX && !( align & Qt::AlignVertical_Mask ) )
		if( !( sizePolicy.verticalPolicy() & QSizePolicy::GrowFlag ) )
			s.setHeight( hint.height() );

	if( align & Qt::AlignHorizontal_Mask )
		s.setWidth( QLAYOUTSIZE_MAX );

	if( align & Qt::AlignVertical_Mask )
		s.setHeight( QLAYOUTSIZE_MAX );

	return s;
}

//
// qSmartMaxSize
//

QSize qSmartMaxSize( const QWidget * w, Qt::Alignment align )
{
	return qSmartMaxSize( w->sizeHint().expandedTo( w->minimumSizeHint() ),
		w->minimumSize(), w->maximumSize(),
		w->sizePolicy(), align );
}

} /* namespace QtMWidgets */
