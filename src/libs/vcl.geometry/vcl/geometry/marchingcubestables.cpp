/*
 * This file is part of the Visual Computing Library (VCL) release under the
 * MIT license.
 *
 * Copyright (c) 2015 Basil Fierz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <vcl/geometry/marchingcubestables.h>

// clang-format off
namespace Vcl { namespace Geometry {
	int32_t caseToNumPolys[256] = {
		0, 1, 1, 2, 1, 2, 2, 3,  1, 2, 2, 3, 2, 3, 3, 2,  1, 2, 2, 3, 2, 3, 3, 4,  2, 3, 3, 4, 3, 4, 4, 3,
		1, 2, 2, 3, 2, 3, 3, 4,  2, 3, 3, 4, 3, 4, 4, 3,  2, 3, 3, 2, 3, 4, 4, 3,  3, 4, 4, 3, 4, 5, 5, 2,
		1, 2, 2, 3, 2, 3, 3, 4,  2, 3, 3, 4, 3, 4, 4, 3,  2, 3, 3, 4, 3, 4, 4, 5,  3, 4, 4, 5, 4, 5, 5, 4,
		2, 3, 3, 4, 3, 4, 2, 3,  3, 4, 4, 5, 4, 5, 3, 2,  3, 4, 4, 3, 4, 5, 3, 2,  4, 5, 5, 4, 5, 2, 4, 1,
		1, 2, 2, 3, 2, 3, 3, 4,  2, 3, 3, 4, 3, 4, 4, 3,  2, 3, 3, 4, 3, 4, 4, 5,  3, 2, 4, 3, 4, 3, 5, 2,
		2, 3, 3, 4, 3, 4, 4, 5,  3, 4, 4, 5, 4, 5, 5, 4,  3, 4, 4, 3, 4, 5, 5, 4,  4, 3, 5, 2, 5, 4, 2, 1,
		2, 3, 3, 4, 3, 4, 4, 5,  3, 4, 4, 5, 2, 3, 3, 2,  3, 4, 4, 5, 4, 5, 5, 2,  4, 3, 5, 4, 3, 2, 4, 1,
		3, 4, 4, 5, 4, 5, 3, 4,  4, 5, 5, 2, 3, 4, 2, 1,  2, 3, 3, 2, 3, 4, 2, 1,  3, 2, 4, 1, 2, 1, 1, 0
	};

	int32_t edgeVertexList[256 * 5 * 4] = {
		-1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  8,  3, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  1,  9, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  8,  3, -1,   9,  8,  1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  2, 10, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  8,  3, -1,   1,  2, 10, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  2, 10, -1,   0,  2,  9, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 2,  8,  3, -1,   2, 10,  8, -1,  10,  9,  8, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3, 11,  2, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0, 11,  2, -1,   8, 11,  0, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  9,  0, -1,   2,  3, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1, 11,  2, -1,   1,  9, 11, -1,   9,  8, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3, 10,  1, -1,  11, 10,  3, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0, 10,  1, -1,   0,  8, 10, -1,   8, 11, 10, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3,  9,  0, -1,   3, 11,  9, -1,  11, 10,  9, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  8, 10, -1,  10,  8, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4,  7,  8, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4,  3,  0, -1,   7,  3,  4, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  1,  9, -1,   8,  4,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4,  1,  9, -1,   4,  7,  1, -1,   7,  3,  1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  2, 10, -1,   8,  4,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3,  4,  7, -1,   3,  0,  4, -1,   1,  2, 10, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  2, 10, -1,   9,  0,  2, -1,   8,  4,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 2, 10,  9, -1,   2,  9,  7, -1,   2,  7,  3, -1,   7,  9,  4, -1,  -1, -1, -1, -1,
		 8,  4,  7, -1,   3, 11,  2, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		11,  4,  7, -1,  11,  2,  4, -1,   2,  0,  4, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  0,  1, -1,   8,  4,  7, -1,   2,  3, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4,  7, 11, -1,   9,  4, 11, -1,   9, 11,  2, -1,   9,  2,  1, -1,  -1, -1, -1, -1,
		 3, 10,  1, -1,   3, 11, 10, -1,   7,  8,  4, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1, 11, 10, -1,   1,  4, 11, -1,   1,  0,  4, -1,   7, 11,  4, -1,  -1, -1, -1, -1,
		 4,  7,  8, -1,   9,  0, 11, -1,   9, 11, 10, -1,  11,  0,  3, -1,  -1, -1, -1, -1,
		 4,  7, 11, -1,   4, 11,  9, -1,   9, 11, 10, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  5,  4, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  5,  4, -1,   0,  8,  3, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  5,  4, -1,   1,  5,  0, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 8,  5,  4, -1,   8,  3,  5, -1,   3,  1,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  2, 10, -1,   9,  5,  4, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3,  0,  8, -1,   1,  2, 10, -1,   4,  9,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 5,  2, 10, -1,   5,  4,  2, -1,   4,  0,  2, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 2, 10,  5, -1,   3,  2,  5, -1,   3,  5,  4, -1,   3,  4,  8, -1,  -1, -1, -1, -1,
		 9,  5,  4, -1,   2,  3, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0, 11,  2, -1,   0,  8, 11, -1,   4,  9,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  5,  4, -1,   0,  1,  5, -1,   2,  3, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 2,  1,  5, -1,   2,  5,  8, -1,   2,  8, 11, -1,   4,  8,  5, -1,  -1, -1, -1, -1,
		10,  3, 11, -1,  10,  1,  3, -1,   9,  5,  4, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4,  9,  5, -1,   0,  8,  1, -1,   8, 10,  1, -1,   8, 11, 10, -1,  -1, -1, -1, -1,
		 5,  4,  0, -1,   5,  0, 11, -1,   5, 11, 10, -1,  11,  0,  3, -1,  -1, -1, -1, -1,
		 5,  4,  8, -1,   5,  8, 10, -1,  10,  8, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  7,  8, -1,   5,  7,  9, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  3,  0, -1,   9,  5,  3, -1,   5,  7,  3, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  7,  8, -1,   0,  1,  7, -1,   1,  5,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  5,  3, -1,   3,  5,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  7,  8, -1,   9,  5,  7, -1,  10,  1,  2, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		10,  1,  2, -1,   9,  5,  0, -1,   5,  3,  0, -1,   5,  7,  3, -1,  -1, -1, -1, -1,
		 8,  0,  2, -1,   8,  2,  5, -1,   8,  5,  7, -1,  10,  5,  2, -1,  -1, -1, -1, -1,
		 2, 10,  5, -1,   2,  5,  3, -1,   3,  5,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 7,  9,  5, -1,   7,  8,  9, -1,   3, 11,  2, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  5,  7, -1,   9,  7,  2, -1,   9,  2,  0, -1,   2,  7, 11, -1,  -1, -1, -1, -1,
		 2,  3, 11, -1,   0,  1,  8, -1,   1,  7,  8, -1,   1,  5,  7, -1,  -1, -1, -1, -1,
		11,  2,  1, -1,  11,  1,  7, -1,   7,  1,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  5,  8, -1,   8,  5,  7, -1,  10,  1,  3, -1,  10,  3, 11, -1,  -1, -1, -1, -1,
		 5,  7,  0, -1,   5,  0,  9, -1,   7, 11,  0, -1,   1,  0, 10, -1,  11, 10,  0, -1,
		11, 10,  0, -1,  11,  0,  3, -1,  10,  5,  0, -1,   8,  0,  7, -1,   5,  7,  0, -1,
		11, 10,  5, -1,   7, 11,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		10,  6,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  8,  3, -1,   5, 10,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  0,  1, -1,   5, 10,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  8,  3, -1,   1,  9,  8, -1,   5, 10,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  6,  5, -1,   2,  6,  1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  6,  5, -1,   1,  2,  6, -1,   3,  0,  8, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  6,  5, -1,   9,  0,  6, -1,   0,  2,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 5,  9,  8, -1,   5,  8,  2, -1,   5,  2,  6, -1,   3,  2,  8, -1,  -1, -1, -1, -1,
		 2,  3, 11, -1,  10,  6,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		11,  0,  8, -1,  11,  2,  0, -1,  10,  6,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  1,  9, -1,   2,  3, 11, -1,   5, 10,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 5, 10,  6, -1,   1,  9,  2, -1,   9, 11,  2, -1,   9,  8, 11, -1,  -1, -1, -1, -1,
		 6,  3, 11, -1,   6,  5,  3, -1,   5,  1,  3, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  8, 11, -1,   0, 11,  5, -1,   0,  5,  1, -1,   5, 11,  6, -1,  -1, -1, -1, -1,
		 3, 11,  6, -1,   0,  3,  6, -1,   0,  6,  5, -1,   0,  5,  9, -1,  -1, -1, -1, -1,
		 6,  5,  9, -1,   6,  9, 11, -1,  11,  9,  8, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 5, 10,  6, -1,   4,  7,  8, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4,  3,  0, -1,   4,  7,  3, -1,   6,  5, 10, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  9,  0, -1,   5, 10,  6, -1,   8,  4,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		10,  6,  5, -1,   1,  9,  7, -1,   1,  7,  3, -1,   7,  9,  4, -1,  -1, -1, -1, -1,
		 6,  1,  2, -1,   6,  5,  1, -1,   4,  7,  8, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  2,  5, -1,   5,  2,  6, -1,   3,  0,  4, -1,   3,  4,  7, -1,  -1, -1, -1, -1,
		 8,  4,  7, -1,   9,  0,  5, -1,   0,  6,  5, -1,   0,  2,  6, -1,  -1, -1, -1, -1,
		 7,  3,  9, -1,   7,  9,  4, -1,   3,  2,  9, -1,   5,  9,  6, -1,   2,  6,  9, -1,
		 3, 11,  2, -1,   7,  8,  4, -1,  10,  6,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 5, 10,  6, -1,   4,  7,  2, -1,   4,  2,  0, -1,   2,  7, 11, -1,  -1, -1, -1, -1,
		 0,  1,  9, -1,   4,  7,  8, -1,   2,  3, 11, -1,   5, 10,  6, -1,  -1, -1, -1, -1,
		 9,  2,  1, -1,   9, 11,  2, -1,   9,  4, 11, -1,   7, 11,  4, -1,   5, 10,  6, -1,
		 8,  4,  7, -1,   3, 11,  5, -1,   3,  5,  1, -1,   5, 11,  6, -1,  -1, -1, -1, -1,
		 5,  1, 11, -1,   5, 11,  6, -1,   1,  0, 11, -1,   7, 11,  4, -1,   0,  4, 11, -1,
		 0,  5,  9, -1,   0,  6,  5, -1,   0,  3,  6, -1,  11,  6,  3, -1,   8,  4,  7, -1,
		 6,  5,  9, -1,   6,  9, 11, -1,   4,  7,  9, -1,   7, 11,  9, -1,  -1, -1, -1, -1,
		10,  4,  9, -1,   6,  4, 10, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4, 10,  6, -1,   4,  9, 10, -1,   0,  8,  3, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		10,  0,  1, -1,  10,  6,  0, -1,   6,  4,  0, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 8,  3,  1, -1,   8,  1,  6, -1,   8,  6,  4, -1,   6,  1, 10, -1,  -1, -1, -1, -1,
		 1,  4,  9, -1,   1,  2,  4, -1,   2,  6,  4, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3,  0,  8, -1,   1,  2,  9, -1,   2,  4,  9, -1,   2,  6,  4, -1,  -1, -1, -1, -1,
		 0,  2,  4, -1,   4,  2,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 8,  3,  2, -1,   8,  2,  4, -1,   4,  2,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		10,  4,  9, -1,  10,  6,  4, -1,  11,  2,  3, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  8,  2, -1,   2,  8, 11, -1,   4,  9, 10, -1,   4, 10,  6, -1,  -1, -1, -1, -1,
		 3, 11,  2, -1,   0,  1,  6, -1,   0,  6,  4, -1,   6,  1, 10, -1,  -1, -1, -1, -1,
		 6,  4,  1, -1,   6,  1, 10, -1,   4,  8,  1, -1,   2,  1, 11, -1,   8, 11,  1, -1,
		 9,  6,  4, -1,   9,  3,  6, -1,   9,  1,  3, -1,  11,  6,  3, -1,  -1, -1, -1, -1,
		 8, 11,  1, -1,   8,  1,  0, -1,  11,  6,  1, -1,   9,  1,  4, -1,   6,  4,  1, -1,
		 3, 11,  6, -1,   3,  6,  0, -1,   0,  6,  4, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 6,  4,  8, -1,  11,  6,  8, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 7, 10,  6, -1,   7,  8, 10, -1,   8,  9, 10, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  7,  3, -1,   0, 10,  7, -1,   0,  9, 10, -1,   6,  7, 10, -1,  -1, -1, -1, -1,
		10,  6,  7, -1,   1, 10,  7, -1,   1,  7,  8, -1,   1,  8,  0, -1,  -1, -1, -1, -1,
		10,  6,  7, -1,  10,  7,  1, -1,   1,  7,  3, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  2,  6, -1,   1,  6,  8, -1,   1,  8,  9, -1,   8,  6,  7, -1,  -1, -1, -1, -1,
		 2,  6,  9, -1,   2,  9,  1, -1,   6,  7,  9, -1,   0,  9,  3, -1,   7,  3,  9, -1,
		 7,  8,  0, -1,   7,  0,  6, -1,   6,  0,  2, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 7,  3,  2, -1,   6,  7,  2, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 2,  3, 11, -1,  10,  6,  8, -1,  10,  8,  9, -1,   8,  6,  7, -1,  -1, -1, -1, -1,
		 2,  0,  7, -1,   2,  7, 11, -1,   0,  9,  7, -1,   6,  7, 10, -1,   9, 10,  7, -1,
		 1,  8,  0, -1,   1,  7,  8, -1,   1, 10,  7, -1,   6,  7, 10, -1,   2,  3, 11, -1,
		11,  2,  1, -1,  11,  1,  7, -1,  10,  6,  1, -1,   6,  7,  1, -1,  -1, -1, -1, -1,
		 8,  9,  6, -1,   8,  6,  7, -1,   9,  1,  6, -1,  11,  6,  3, -1,   1,  3,  6, -1,
		 0,  9,  1, -1,  11,  6,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 7,  8,  0, -1,   7,  0,  6, -1,   3, 11,  0, -1,  11,  6,  0, -1,  -1, -1, -1, -1,
		 7, 11,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 7,  6, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3,  0,  8, -1,  11,  7,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  1,  9, -1,  11,  7,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 8,  1,  9, -1,   8,  3,  1, -1,  11,  7,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		10,  1,  2, -1,   6, 11,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  2, 10, -1,   3,  0,  8, -1,   6, 11,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 2,  9,  0, -1,   2, 10,  9, -1,   6, 11,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 6, 11,  7, -1,   2, 10,  3, -1,  10,  8,  3, -1,  10,  9,  8, -1,  -1, -1, -1, -1,
		 7,  2,  3, -1,   6,  2,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 7,  0,  8, -1,   7,  6,  0, -1,   6,  2,  0, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 2,  7,  6, -1,   2,  3,  7, -1,   0,  1,  9, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  6,  2, -1,   1,  8,  6, -1,   1,  9,  8, -1,   8,  7,  6, -1,  -1, -1, -1, -1,
		10,  7,  6, -1,  10,  1,  7, -1,   1,  3,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		10,  7,  6, -1,   1,  7, 10, -1,   1,  8,  7, -1,   1,  0,  8, -1,  -1, -1, -1, -1,
		 0,  3,  7, -1,   0,  7, 10, -1,   0, 10,  9, -1,   6, 10,  7, -1,  -1, -1, -1, -1,
		 7,  6, 10, -1,   7, 10,  8, -1,   8, 10,  9, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 6,  8,  4, -1,  11,  8,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3,  6, 11, -1,   3,  0,  6, -1,   0,  4,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 8,  6, 11, -1,   8,  4,  6, -1,   9,  0,  1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  4,  6, -1,   9,  6,  3, -1,   9,  3,  1, -1,  11,  3,  6, -1,  -1, -1, -1, -1,
		 6,  8,  4, -1,   6, 11,  8, -1,   2, 10,  1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  2, 10, -1,   3,  0, 11, -1,   0,  6, 11, -1,   0,  4,  6, -1,  -1, -1, -1, -1,
		 4, 11,  8, -1,   4,  6, 11, -1,   0,  2,  9, -1,   2, 10,  9, -1,  -1, -1, -1, -1,
		10,  9,  3, -1,  10,  3,  2, -1,   9,  4,  3, -1,  11,  3,  6, -1,   4,  6,  3, -1,
		 8,  2,  3, -1,   8,  4,  2, -1,   4,  6,  2, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  4,  2, -1,   4,  6,  2, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  9,  0, -1,   2,  3,  4, -1,   2,  4,  6, -1,   4,  3,  8, -1,  -1, -1, -1, -1,
		 1,  9,  4, -1,   1,  4,  2, -1,   2,  4,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 8,  1,  3, -1,   8,  6,  1, -1,   8,  4,  6, -1,   6, 10,  1, -1,  -1, -1, -1, -1,
		10,  1,  0, -1,  10,  0,  6, -1,   6,  0,  4, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4,  6,  3, -1,   4,  3,  8, -1,   6, 10,  3, -1,   0,  3,  9, -1,  10,  9,  3, -1,
		10,  9,  4, -1,   6, 10,  4, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4,  9,  5, -1,   7,  6, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  8,  3, -1,   4,  9,  5, -1,  11,  7,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 5,  0,  1, -1,   5,  4,  0, -1,   7,  6, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		11,  7,  6, -1,   8,  3,  4, -1,   3,  5,  4, -1,   3,  1,  5, -1,  -1, -1, -1, -1,
		 9,  5,  4, -1,  10,  1,  2, -1,   7,  6, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 6, 11,  7, -1,   1,  2, 10, -1,   0,  8,  3, -1,   4,  9,  5, -1,  -1, -1, -1, -1,
		 7,  6, 11, -1,   5,  4, 10, -1,   4,  2, 10, -1,   4,  0,  2, -1,  -1, -1, -1, -1,
		 3,  4,  8, -1,   3,  5,  4, -1,   3,  2,  5, -1,  10,  5,  2, -1,  11,  7,  6, -1,
		 7,  2,  3, -1,   7,  6,  2, -1,   5,  4,  9, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  5,  4, -1,   0,  8,  6, -1,   0,  6,  2, -1,   6,  8,  7, -1,  -1, -1, -1, -1,
		 3,  6,  2, -1,   3,  7,  6, -1,   1,  5,  0, -1,   5,  4,  0, -1,  -1, -1, -1, -1,
		 6,  2,  8, -1,   6,  8,  7, -1,   2,  1,  8, -1,   4,  8,  5, -1,   1,  5,  8, -1,
		 9,  5,  4, -1,  10,  1,  6, -1,   1,  7,  6, -1,   1,  3,  7, -1,  -1, -1, -1, -1,
		 1,  6, 10, -1,   1,  7,  6, -1,   1,  0,  7, -1,   8,  7,  0, -1,   9,  5,  4, -1,
		 4,  0, 10, -1,   4, 10,  5, -1,   0,  3, 10, -1,   6, 10,  7, -1,   3,  7, 10, -1,
		 7,  6, 10, -1,   7, 10,  8, -1,   5,  4, 10, -1,   4,  8, 10, -1,  -1, -1, -1, -1,
		 6,  9,  5, -1,   6, 11,  9, -1,  11,  8,  9, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3,  6, 11, -1,   0,  6,  3, -1,   0,  5,  6, -1,   0,  9,  5, -1,  -1, -1, -1, -1,
		 0, 11,  8, -1,   0,  5, 11, -1,   0,  1,  5, -1,   5,  6, 11, -1,  -1, -1, -1, -1,
		 6, 11,  3, -1,   6,  3,  5, -1,   5,  3,  1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  2, 10, -1,   9,  5, 11, -1,   9, 11,  8, -1,  11,  5,  6, -1,  -1, -1, -1, -1,
		 0, 11,  3, -1,   0,  6, 11, -1,   0,  9,  6, -1,   5,  6,  9, -1,   1,  2, 10, -1,
		11,  8,  5, -1,  11,  5,  6, -1,   8,  0,  5, -1,  10,  5,  2, -1,   0,  2,  5, -1,
		 6, 11,  3, -1,   6,  3,  5, -1,   2, 10,  3, -1,  10,  5,  3, -1,  -1, -1, -1, -1,
		 5,  8,  9, -1,   5,  2,  8, -1,   5,  6,  2, -1,   3,  8,  2, -1,  -1, -1, -1, -1,
		 9,  5,  6, -1,   9,  6,  0, -1,   0,  6,  2, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  5,  8, -1,   1,  8,  0, -1,   5,  6,  8, -1,   3,  8,  2, -1,   6,  2,  8, -1,
		 1,  5,  6, -1,   2,  1,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  3,  6, -1,   1,  6, 10, -1,   3,  8,  6, -1,   5,  6,  9, -1,   8,  9,  6, -1,
		10,  1,  0, -1,  10,  0,  6, -1,   9,  5,  0, -1,   5,  6,  0, -1,  -1, -1, -1, -1,
		 0,  3,  8, -1,   5,  6, 10, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		10,  5,  6, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		11,  5, 10, -1,   7,  5, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		11,  5, 10, -1,  11,  7,  5, -1,   8,  3,  0, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 5, 11,  7, -1,   5, 10, 11, -1,   1,  9,  0, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		10,  7,  5, -1,  10, 11,  7, -1,   9,  8,  1, -1,   8,  3,  1, -1,  -1, -1, -1, -1,
		11,  1,  2, -1,  11,  7,  1, -1,   7,  5,  1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  8,  3, -1,   1,  2,  7, -1,   1,  7,  5, -1,   7,  2, 11, -1,  -1, -1, -1, -1,
		 9,  7,  5, -1,   9,  2,  7, -1,   9,  0,  2, -1,   2, 11,  7, -1,  -1, -1, -1, -1,
		 7,  5,  2, -1,   7,  2, 11, -1,   5,  9,  2, -1,   3,  2,  8, -1,   9,  8,  2, -1,
		 2,  5, 10, -1,   2,  3,  5, -1,   3,  7,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 8,  2,  0, -1,   8,  5,  2, -1,   8,  7,  5, -1,  10,  2,  5, -1,  -1, -1, -1, -1,
		 9,  0,  1, -1,   5, 10,  3, -1,   5,  3,  7, -1,   3, 10,  2, -1,  -1, -1, -1, -1,
		 9,  8,  2, -1,   9,  2,  1, -1,   8,  7,  2, -1,  10,  2,  5, -1,   7,  5,  2, -1,
		 1,  3,  5, -1,   3,  7,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  8,  7, -1,   0,  7,  1, -1,   1,  7,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  0,  3, -1,   9,  3,  5, -1,   5,  3,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9,  8,  7, -1,   5,  9,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 5,  8,  4, -1,   5, 10,  8, -1,  10, 11,  8, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 5,  0,  4, -1,   5, 11,  0, -1,   5, 10, 11, -1,  11,  3,  0, -1,  -1, -1, -1, -1,
		 0,  1,  9, -1,   8,  4, 10, -1,   8, 10, 11, -1,  10,  4,  5, -1,  -1, -1, -1, -1,
		10, 11,  4, -1,  10,  4,  5, -1,  11,  3,  4, -1,   9,  4,  1, -1,   3,  1,  4, -1,
		 2,  5,  1, -1,   2,  8,  5, -1,   2, 11,  8, -1,   4,  5,  8, -1,  -1, -1, -1, -1,
		 0,  4, 11, -1,   0, 11,  3, -1,   4,  5, 11, -1,   2, 11,  1, -1,   5,  1, 11, -1,
		 0,  2,  5, -1,   0,  5,  9, -1,   2, 11,  5, -1,   4,  5,  8, -1,  11,  8,  5, -1,
		 9,  4,  5, -1,   2, 11,  3, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 2,  5, 10, -1,   3,  5,  2, -1,   3,  4,  5, -1,   3,  8,  4, -1,  -1, -1, -1, -1,
		 5, 10,  2, -1,   5,  2,  4, -1,   4,  2,  0, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3, 10,  2, -1,   3,  5, 10, -1,   3,  8,  5, -1,   4,  5,  8, -1,   0,  1,  9, -1,
		 5, 10,  2, -1,   5,  2,  4, -1,   1,  9,  2, -1,   9,  4,  2, -1,  -1, -1, -1, -1,
		 8,  4,  5, -1,   8,  5,  3, -1,   3,  5,  1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  4,  5, -1,   1,  0,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 8,  4,  5, -1,   8,  5,  3, -1,   9,  0,  5, -1,   0,  3,  5, -1,  -1, -1, -1, -1,
		 9,  4,  5, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4, 11,  7, -1,   4,  9, 11, -1,   9, 10, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  8,  3, -1,   4,  9,  7, -1,   9, 11,  7, -1,   9, 10, 11, -1,  -1, -1, -1, -1,
		 1, 10, 11, -1,   1, 11,  4, -1,   1,  4,  0, -1,   7,  4, 11, -1,  -1, -1, -1, -1,
		 3,  1,  4, -1,   3,  4,  8, -1,   1, 10,  4, -1,   7,  4, 11, -1,  10, 11,  4, -1,
		 4, 11,  7, -1,   9, 11,  4, -1,   9,  2, 11, -1,   9,  1,  2, -1,  -1, -1, -1, -1,
		 9,  7,  4, -1,   9, 11,  7, -1,   9,  1, 11, -1,   2, 11,  1, -1,   0,  8,  3, -1,
		11,  7,  4, -1,  11,  4,  2, -1,   2,  4,  0, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		11,  7,  4, -1,  11,  4,  2, -1,   8,  3,  4, -1,   3,  2,  4, -1,  -1, -1, -1, -1,
		 2,  9, 10, -1,   2,  7,  9, -1,   2,  3,  7, -1,   7,  4,  9, -1,  -1, -1, -1, -1,
		 9, 10,  7, -1,   9,  7,  4, -1,  10,  2,  7, -1,   8,  7,  0, -1,   2,  0,  7, -1,
		 3,  7, 10, -1,   3, 10,  2, -1,   7,  4, 10, -1,   1, 10,  0, -1,   4,  0, 10, -1,
		 1, 10,  2, -1,   8,  7,  4, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4,  9,  1, -1,   4,  1,  7, -1,   7,  1,  3, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4,  9,  1, -1,   4,  1,  7, -1,   0,  8,  1, -1,   8,  7,  1, -1,  -1, -1, -1, -1,
		 4,  0,  3, -1,   7,  4,  3, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 4,  8,  7, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9, 10,  8, -1,  10, 11,  8, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3,  0,  9, -1,   3,  9, 11, -1,  11,  9, 10, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  1, 10, -1,   0, 10,  8, -1,   8, 10, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3,  1, 10, -1,  11,  3, 10, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  2, 11, -1,   1, 11,  9, -1,   9, 11,  8, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3,  0,  9, -1,   3,  9, 11, -1,   1,  2,  9, -1,   2, 11,  9, -1,  -1, -1, -1, -1,
		 0,  2, 11, -1,   8,  0, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 3,  2, 11, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 2,  3,  8, -1,   2,  8, 10, -1,  10,  8,  9, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 9, 10,  2, -1,   0,  9,  2, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 2,  3,  8, -1,   2,  8, 10, -1,   0,  1,  8, -1,   1, 10,  8, -1,  -1, -1, -1, -1,
		 1, 10,  2, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 1,  3,  8, -1,   9,  1,  8, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  9,  1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		 0,  3,  8, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
		-1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,  -1, -1, -1, -1,
	};
}}
// clang-format on
