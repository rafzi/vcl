/*
 * This file is part of the Visual Computing Library (VCL) release under the
 * MIT license.
 *
 * Copyright (c) 2018 Basil Fierz
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
#pragma once

// VCL configuration
#include <vcl/config/global.h>
#include <vcl/config/eigen.h>
#include <vcl/config/cuda.h>

// C++ standard library
#include <array>

// VCL
#include <vcl/compute/cuda/buffer.h>
#include <vcl/compute/cuda/context.h>
#include <vcl/compute/cuda/module.h>
#include <vcl/math/solver/jacobi.h>
#include <vcl/math/solver/poisson.h>

namespace Vcl { namespace Mathematics { namespace Solver { namespace Cuda {
	class Poisson3DJacobiCtx : public JacobiContext
	{
		using vector_t = Eigen::Matrix<float, Eigen::Dynamic, 1>;
		using map_t = Eigen::Map<vector_t>;
		using const_map_t = Eigen::Map<const vector_t>;

	public:
		Poisson3DJacobiCtx(
			ref_ptr<Compute::Context> ctx,
			ref_ptr<Compute::CommandQueue> queue,
			const Eigen::Vector3ui& dim);
		~Poisson3DJacobiCtx();

		void setData(map_t unknowns, const_map_t rhs);
		void setData(ref_ptr<Compute::Cuda::Buffer> unknowns, ref_ptr<Compute::Cuda::Buffer> rhs);

		//! \brief Create the poisson stencil
		//! \param h Grid spacing
		//! \param k Scaling constant
		//! \param o Diagonal offset
		//! \param skip Blocked cells
		void updatePoissonStencil(float h, float k, float o, Eigen::Map<const Eigen::Matrix<unsigned char, Eigen::Dynamic, 1>> skip);
		void updatePoissonStencil(float h, float k, float o, const Compute::Cuda::Buffer& skip);

		//! \name Access internal solver state
		//! \{
		std::array<ref_ptr<Compute::Cuda::Buffer>, 7> matrix() const { return _laplacian; }
		//! \}

		//! \name Interface implementation
		//! \{
		int size() const override;
		void precompute() override;
		void updateSolution() override;
		double computeError() override;
		void finish(double* residual) override;
		//! \}

	private:
		//! Dimensions of the grid
		Eigen::Vector3ui _dim;

		//! Current error
		float _error{ 0 };

		// Device context
		ref_ptr<Compute::Context> _ownerCtx;

		//! Commandqueue the execution uses
		ref_ptr<Compute::CommandQueue> _queue;

		//! Laplacian matrix (center, x(l/r), y(l/r), z(l/r))
		std::array<ref_ptr<Compute::Cuda::Buffer>, 7> _laplacian;

		//! Left-hand side
		std::tuple<ref_ptr<Compute::Cuda::Buffer>, map_t> _unknowns;

		//! Right-hand side
		std::tuple<ref_ptr<Compute::Cuda::Buffer>, map_t> _rhs;

		//! Temporary buffer for the updated solution
		ref_ptr<Compute::Cuda::Buffer> _next;

		//! Buffer to accumulate the solver error
		ref_ptr<Compute::Cuda::Buffer> _dev_error;
	};
}}}}
