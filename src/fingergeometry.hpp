
/*
	SPDX-FileCopyrightText: 2014-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
*/

#ifndef QTMWIDGETS__PRIVATE__FINGERGEOMETRY_HPP__INCLUDED
#define QTMWIDGETS__PRIVATE__FINGERGEOMETRY_HPP__INCLUDED


namespace QtMWidgets {

//
// FingerGeometry
//

/*!
	FingerGeometry provides information about width and
	height of finger.
*/
class FingerGeometry {
public:
	//! \return Width of finger.
	static int width();

	//! \return Height of finger.
	static int height();

	//! \return Bounce of the touch.
	static int touchBounce();

	//! \return Bounce of the long touch.
	static int longTouchBounce();
}; // class FingerGeometry

} /* namespace QtMWidgets */

#endif // QTMWIDGETS__PRIVATE__FINGERGEOMETRY_HPP__INCLUDED
