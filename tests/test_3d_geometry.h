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
#include "core/math/plane.h"
#include "core/math/random_number_generator.h"
namespace Test3DGeometry {
	/// <summary>
	/// Static maths function tests
	/// </summary>
	TEST_CASE("[Geometry3D] get_closest_points_between_segments") {
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
	TEST_CASE("[Geometry3D] get_closest_distance_between_segments") {
		const Vector3 p_from_a = Vector3(5,4,3);
		const Vector3 p_to_a = Vector3(13,24,26);
		const Vector3 p_from_b = Vector3(2,2,1);
		const Vector3 p_to_b = Vector3(4,5,6);
		float out = Geometry3D::get_closest_distance_between_segments(p_from_a, p_to_a, p_from_b, p_to_b);
		MESSAGE(out);
		CHECK(out == 0.0f);
	}
	TEST_CASE("[Geometry3D] build_box_planes") {
		RandomNumberGenerator rng = RandomNumberGenerator();
		rng.set_seed(time(0));
		const Vector3 extents = Vector3(rng.randi(), rng.randi(), rng.randi());
		Vector<Plane> box = Geometry3D::build_box_planes(extents);
		CHECK(extents.x == box[0].d);
		CHECK(box[0].normal == Vector3(1, 0, 0));
		CHECK(extents.x == box[1].d);
		CHECK(box[1].normal == Vector3(-1, 0, 0));
		CHECK(extents.y == box[2].d);
		CHECK(box[2].normal == Vector3(0, 1, 0));
		CHECK(extents.y == box[3].d);
		CHECK(box[3].normal == Vector3(0, -1, 0));
		CHECK(extents.z == box[4].d);
		CHECK(box[4].normal == Vector3(0, 0, 1));
		CHECK(extents.z == box[5].d);
		CHECK(box[5].normal == Vector3(0, 0, -1));
	}
	TEST_CASE("[Geometry3D] build_capsule_planes") {
		real_t p_radius = 10;
		real_t p_height = 20;
		int p_sides = 6;
		int p_lats = 10;
		Vector3::Axis p_axis = Vector3::Axis();
		Vector<Plane> capsule = Geometry3D::build_capsule_planes(p_radius, p_height, p_sides, p_lats, p_axis);
		CHECK(capsule.size() == 126);
		
	}
	TEST_CASE("[Geometry3D] build_convex_mesh") {
		const Vector3 extents = Vector3(5, 10, 5);
		Vector<Plane> box = Geometry3D::build_box_planes(extents);
		Geometry3D::MeshData mesh = Geometry3D::build_convex_mesh(box);
		CHECK(&mesh != nullptr);
		
	}
	TEST_CASE("[Geometry3D] build_cylinder_planes") {
		real_t p_radius = 0.0f;
		real_t p_height = 0.0f;
		int p_sides = 0;
		Vector3::Axis p_axis = Vector3::Axis();
		Vector<Plane> planes = Geometry3D::build_cylinder_planes(p_radius, p_height, p_sides, p_axis);
		CHECK(&planes != nullptr);
	}
	TEST_CASE("[Geometry3D] build_sphere_planes") {
		real_t p_radius = 0.0f;
		int p_lats = 0;
		int p_lons = 0;
		Vector3::Axis p_axis = Vector3::Axis();
		Vector<Plane> planes = Geometry3D::build_sphere_planes(p_radius, p_lats, p_lons, p_axis);
		CHECK(&planes != nullptr);
	}
	TEST_CASE("[Geometry3D] clip_polygon") {
		const Vector<Vector3> polygon = Vector<Vector3>();
		const Plane p_plane = Plane();
		Vector<Vector3> output = Geometry3D::clip_polygon(polygon, p_plane);
		CHECK(&output != nullptr);	
	}
	TEST_CASE("[Geometry3D] compute_convex_mesh_points") {
		const Plane plane = Plane();
		int planeCount = 0;

		Vector<Vector3> vectors =  Geometry3D::compute_convex_mesh_points(&plane, planeCount);
		CHECK(&vectors != nullptr);
		
	}
	TEST_CASE("[Geometry3D] generate_edf") {
		const Vector<bool> p_voxels = Vector<bool>();
		const Vector3i p_size = Vector3i();
		bool p_negative = false;
		Vector<uint32_t> edf = Geometry3D::generate_edf(p_voxels, p_size, p_negative);
		p_negative = true;
		Vector<uint32_t> edffalse = Geometry3D::generate_edf(p_voxels, p_size, p_negative);
		CHECK(&edf != nullptr);
		CHECK(&edffalse != nullptr);
	}
	TEST_CASE("[Geometry3D] generate_sdf8") {
		const Vector<uint32_t> p_positive = Vector<uint32_t>();
		const Vector<uint32_t> p_negative = Vector<uint32_t>();
		Vector<int8_t> sdf8 = Geometry3D::generate_sdf8(p_positive, p_negative);
		CHECK(&sdf8 != nullptr);
	}
	TEST_CASE("[Geometry3D] get_closest_point_to_segment") {
		const Vector3 p_point = Vector3();
		Vector3 p_segment[2] = {Vector3(), Vector3()};
		Vector3 output = Geometry3D::get_closest_point_to_segment(p_point, p_segment);
		CHECK(&output != nullptr);
	}
	TEST_CASE("[Geometry3D] get_closest_point_to_segment") {
		const Vector3 p_point= Vector3();
		Vector3 p_segment[2] = { Vector3(), Vector3() };
		Vector3 point = Geometry3D::get_closest_point_to_segment_uncapped(p_point, p_segment);
		CHECK(&point != nullptr);
	}
	TEST_CASE("[Geometry3D] octahedron_map_decode") {
		const Vector2 p_uv = Vector2();
		Vector3 output = Geometry3D::octahedron_map_decode(p_uv);
		CHECK(&output != nullptr);
	}
	TEST_CASE("[Geometry3D] planeBoxOverlap") {
		Vector3 normal = Vector3();
		float d = 0.0f;
		Vector3 maxbox = Vector3();
		bool overlap = Geometry3D::planeBoxOverlap(normal, d, maxbox);
		CHECK(&overlap != nullptr);
	}
	TEST_CASE("[Geometry3D] point_in_projected_triangle") {
		const Vector3 p_point = Vector3();
		const Vector3 p_v1 = Vector3();
		const Vector3 p_v2 = Vector3();
		const Vector3 p_v3 = Vector3();
		bool output = Geometry3D::point_in_projected_triangle(p_point, p_v1, p_v2, p_v3);
		CHECK(&output != nullptr);
	}
	TEST_CASE("[Geometry3D] ray_intersects_triangle") {
		const Vector3 p_from = Vector3();
		const Vector3 p_dir = Vector3();
		const Vector3 p_v = Vector3();
		const Vector3 p_v1 = Vector3();
		const Vector3 p_v2 = Vector3();
		Vector3 *r_res = nullptr;
		bool output = Geometry3D::ray_intersects_triangle(p_from, p_dir, p_v, p_v1, p_v2, r_res);
		CHECK(&output != nullptr);
	}
	}
#endif
