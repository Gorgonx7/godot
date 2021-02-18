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
#include "vector"
namespace Test3DGeometry {
	/// <summary>
	/// Static maths function tests
	/// </summary>
	TEST_CASE("[Geometry3D] get_closest_points_between_segments") {
		struct Case {
			String Name;
			Vector3 Point1, Point2, Point3, Point4;
			Vector3 Got1, Got2;
			Vector3 Want1, Want2;
			Case(String name, Vector3 point1, Vector3 point2, Vector3 point3, Vector3 point4, Vector3 want1, Vector3 want2) :
					Name(name), Point1(point1), Point2(point2), Point3(point3), Point4(point4), Want1(want1), Want2(want2) {};
		};
		std::vector<Case> tt;
		tt.push_back(Case("Test small lines", Vector3(1,-1,1),Vector3(1,1,-1),Vector3(-1,-1,-1),Vector3(-1,1,1),Vector3(1,0,0), Vector3(-1,0,0)));
		
		for (std::vector<Case>::iterator it = std::begin(tt); it != std::end(tt); ++it) {
			Geometry3D::get_closest_points_between_segments(it->Point1, it->Point2, it->Point3, it->Point4, it->Got1, it->Got2);

			CHECK(it->Got1 == it->Want1);
			CHECK(it->Got2 == it->Want2);
		}
	}
	TEST_CASE("[Geometry3D] get_closest_distance_between_segments") {
		// Tests an invalid triangle returns zero
		const Vector3 p_from_a = Vector3(5,7,300);
		const Vector3 p_to_a = Vector3(13,240,26);
		const Vector3 p_from_b = Vector3(-10,2,1);
		const Vector3 p_to_b = Vector3(40,500,300);
		float out = Geometry3D::get_closest_distance_between_segments(p_from_a, p_to_a, p_from_b, p_to_b);
		CHECK(out == 0.0f);
	}
	TEST_CASE("[Geometry3D] build_box_planes") {
		RandomNumberGenerator rng = RandomNumberGenerator();
		rng.set_seed(time(0));
		const Vector3 extents = Vector3(rng.randi(), rng.randi(), rng.randi());
		Vector<Plane> box = Geometry3D::build_box_planes(extents);
		CHECK(box.size() == 6);
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
		// Should equal (p_sides * p_lats) * 2 + p_sides
		CHECK(capsule.size() == 126);
	}
	TEST_CASE("[Geometry3D] build_cylinder_planes") {
		real_t p_radius = 3.0f;
		real_t p_height = 10.0f;
		int p_sides = 10;
		Vector3::Axis p_axis = Vector3::Axis();
		Vector<Plane> planes = Geometry3D::build_cylinder_planes(p_radius, p_height, p_sides, p_axis);
		CHECK(&planes != nullptr);
		//Should equal p_sides + 2
		CHECK(planes.size() == 12);
	}
	TEST_CASE("[Geometry3D] build_sphere_planes") {
		real_t p_radius = 10.0f;
		int p_lats = 10;
		int p_lons = 3;
		Vector3::Axis p_axis = Vector3::Axis();
		Vector<Plane> planes = Geometry3D::build_sphere_planes(p_radius, p_lats, p_lons, p_axis);
		CHECK(&planes != nullptr);
		CHECK(planes.size() == 63);
	}
	TEST_CASE("[Geometry3D] build_convex_mesh") {
		const Vector3 extents = Vector3(5, 10, 5);
		Vector<Plane> box = Geometry3D::build_box_planes(extents);
		Geometry3D::MeshData mesh = Geometry3D::build_convex_mesh(box);
		CHECK(&mesh != nullptr);

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
	TEST_CASE("[Geometry3D] segment_intersects_convex") {
		const Vector3 p_from = Vector3();
		const Vector3 p_to = Vector3();
		const Plane *p_planes;
		int p_plane_count;
		Vector3 *p_res = new Vector3();
		Vector3 *p_norm = new Vector3();
		bool output = Geometry3D::segment_intersects_convex(p_from, p_to, p_planes, p_plane_count, p_res, p_norm);
		CHECK(&output != nullptr);
	}
	TEST_CASE("[Geometry3D] segment_intersects_cylinder") {
		const Vector3 p_from = Vector3();
		const Vector3 p_to = Vector3();
		real_t p_height = 0.0f;
		real_t p_radius = 0.0f;
		Vector3 *r_res = nullptr;
		Vector3 *r_norm = nullptr;
		bool output = Geometry3D::segment_intersects_cylinder(p_from, p_to, p_height, p_radius, r_res, r_norm);
		CHECK(&output != nullptr);
	}
	TEST_CASE("[Geometry3D] segment_intersects_cylinder") {
		const Vector3 p_from = Vector3();
		const Vector3 p_to = Vector3();
		const Vector3 p_sphere_pos = Vector3();
		real_t p_sphere_radius = 0.0f;
		Vector3 *r_res = nullptr;
		Vector3 *r_norm = nullptr;
		bool output = Geometry3D::segment_intersects_sphere(p_from, p_to, p_sphere_pos, p_sphere_radius, r_res, r_norm);
		CHECK(&output != nullptr);
	}
	TEST_CASE("[Geometry3D] segment_intersects_triangle") {
		const Vector3 p_from = Vector3();
		const Vector3 p_to = Vector3();
		const Vector3 p_v0 = Vector3();
		const Vector3 p_v1 = Vector3();
		const Vector3 p_v2 = Vector3();
		Vector3 *r_res = nullptr;
		bool output = Geometry3D::segment_intersects_triangle(p_from, p_to, p_v0, p_v1, p_v2, r_res);
		CHECK(&output != nullptr);
	}
	TEST_CASE("[Geometry3D] separate_objects") {
		Vector<Face3> p_array = Vector<Face3>();
		Vector<Vector<Face3>> output = Geometry3D::separate_objects(p_array);
		CHECK(&output != nullptr);
	}
	TEST_CASE("[Geometry3D] tetrahedron_get_barycentric_coords") {
		const Vector3 p_a = Vector3();
		const Vector3 p_b = Vector3();
		const Vector3 p_c = Vector3();
		const Vector3 p_d = Vector3();
		const Vector3 p_pos = Vector3();
		Color output = Geometry3D::tetrahedron_get_barycentric_coords(p_a, p_b, p_c, p_d, p_pos);
		CHECK(&output != nullptr);
	}
	TEST_CASE("[Geometry3D] triangle_box_overlap") {
		struct Case {
			String Name;
			Vector3 Boxcenter;
			Vector3 Boxhalfsize;
			Vector3 * Triverts;
			bool Want;
			Case(String name, Vector3 center, Vector3 halfsize, Vector3 *verts, bool w) :
					Name(name), Boxcenter(center), Boxhalfsize(halfsize), Triverts(verts), Want(w){};
		};
		std::vector<Case> tt = std::vector<Case>();
		Vector3 GoodTriangle[3] = { Vector3(3,2,3), Vector3(2,2,1), Vector3(2,1,1) };
		tt.push_back(Case("Test triangle in box is found", Vector3(0,0,0), Vector3(5,5,5), GoodTriangle, true));
		Vector3 BadTriangle[3] = { Vector3(100,100,100), Vector3(-100,-100,-100), Vector3(10,10,10) };
		tt.push_back(Case("Test triangle in box is found", Vector3(1000,1000,1000), Vector3(1,1,1), BadTriangle, false));
		for (std::vector<Case>::iterator it = std::begin(tt); it != std::end(tt); ++it) {
			bool output = Geometry3D::triangle_box_overlap(it->Boxcenter, it->Boxhalfsize, it->Triverts);
			CHECK(output == it->Want);
		}
	}
	TEST_CASE("[Geometry3D] triangle_get_barycentric_coords") {
		const Vector3 p_a = Vector3();
		const Vector3 p_b = Vector3();
		const Vector3 p_c = Vector3();
		const Vector3 p_pos = Vector3();
		Vector3 output = Geometry3D::triangle_get_barycentric_coords(p_a, p_b, p_c, p_pos);
		CHECK(&output != nullptr);
	}
	TEST_CASE("[Geometry3D] triangle_sphere_intersection_test") {
		const Vector3 *p_triangle = new Vector3();
		const Vector3 p_normal = Vector3();
		const Vector3 p_sphere_pos = Vector3();
		real_t p_sphere_radius = 0.0f;
		Vector3 r_triangle_contact = Vector3();
		Vector3 r_sphere_contact = Vector3();
		bool output = Geometry3D::triangle_sphere_intersection_test(p_triangle, p_normal, p_sphere_pos, p_sphere_radius, r_triangle_contact, r_sphere_contact);
		CHECK(&output != nullptr);
	}
	TEST_CASE("[Geometry3D] wrap_geometry") {
		Vector<Face3> p_array;
		real_t *p_error;
		Vector<Face3> output = Geometry3D::wrap_geometry(p_array, p_error);
		CHECK(&output != nullptr);
	}
	}
#endif
