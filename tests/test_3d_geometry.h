/*************************************************************************/
/*  test_basis.h                                                         */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/


#ifndef TEST_3D_GEOMETRY_H
#define TEST_3D_GEOMETRY_H

#include "core/math/vector3.h"
#include "core/math/geometry_3d.h"
#include "tests/test_macros.h"
namespace Test3DGeometry {
	TEST_CASE("[Vector3]get_closest_points_between_segments") {
		const Vector3 p1 = Vector3(5, 7, 2);
		const Vector3 p2 = Vector3(3, 8, 1);
		const Vector3 q1 = Vector3(2, 3, 1);
		const Vector3 q2 = Vector3(8, 7, 2);
		Vector3 c1 = Vector3();
		Vector3 c2 = Vector3();
		Geometry3D::get_closest_points_between_segments(p1, p2, q1, q2, c1, c2);
		MESSAGE(c1);
		MESSAGE(c2);
		CHECK(c1 == Vector3(5, 7, 2));
		c2 = Vector3(round(c2.x), round(c2.y), round(c2.z));
		CHECK(c2 == Vector3(7,6,2));

	}
}
#endif
