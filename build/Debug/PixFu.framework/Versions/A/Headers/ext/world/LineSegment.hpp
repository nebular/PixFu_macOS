//
// Created by rodo on 2020-02-29.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma once

#include <iostream>
#include <string>
#include <cmath>

namespace Pix {

	typedef struct structLineSegment {
		float sx, sy;
		float ex, ey;
		float radius;

		inline float angle() { return atan2(ey - sy, ex - sx); }

		inline structLineSegment scaled(float s) { return {sx * s, sy * s, ex * s, ey * s, radius}; }

		inline structLineSegment translated(float x, float y) { return {sx + x, sy + y, ex + x, ey + y, radius}; }

		inline structLineSegment rotated(float rads) {
			// rotation of points about the orgin using http://en.wikipedia.org/wiki/Transformation_matrix#Rotation
			float c = cosf(rads), s = sinf(rads);
			return {sx * c - sy * s, sy * c + sx * s, ex * c - ey * s, ey * c + ex * s};
		}

		inline void scale(float s) {
			sx *= s;
			sy *= s;
			ex *= s;
			ey *= s;
		}

		inline void translate(float x, float y) {
			sx += x;
			sy += y;
			ex += x;
			ey += y;
		}

		inline void rotate(float rads) {
			// rotation of points about the orgin using http://en.wikipedia.org/wiki/Transformation_matrix#Rotation
			float c = cosf(rads), s = sinf(rads);
			sx = sx * c - sy * s;
			sy = sy * c + sx * s;
			ex = ex * c - ey * s;
			ey = ey * c + ex * s;
		}

	} LineSegment_t;
}

#pragma clang diagnostic pop