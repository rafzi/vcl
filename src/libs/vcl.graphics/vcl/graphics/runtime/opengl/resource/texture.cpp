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
#include <vcl/graphics/runtime/opengl/resource/texture.h>

// VCL
#include <vcl/core/contract.h>

namespace Vcl { namespace Graphics { namespace Runtime { namespace OpenGL {
	TextureBindPoint::TextureBindPoint(GLenum target, GLuint id)
	: _target(target)
	, _id(id)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(_target, _id);
	}
	TextureBindPoint::~TextureBindPoint()
	{
		glBindTexture(_target, GL_NONE);
	}

	GLenum Texture::toSurfaceFormat(SurfaceFormat type)
	{
		GLenum gl_format = GL_NONE;

		// clang-format off
		switch (type) {
		case SurfaceFormat::R32G32B32A32_FLOAT  : gl_format = GL_RGBA32F;            break;
		case SurfaceFormat::R32G32B32A32_UINT   : gl_format = GL_RGBA32UI;           break;
		case SurfaceFormat::R32G32B32A32_SINT   : gl_format = GL_RGBA32I;            break;
		case SurfaceFormat::R16G16B16A16_FLOAT  : gl_format = GL_RGBA16F;            break;
		case SurfaceFormat::R16G16B16A16_UNORM  : gl_format = GL_RGBA16;             break;
		case SurfaceFormat::R16G16B16A16_UINT   : gl_format = GL_RGBA16UI;           break;
		case SurfaceFormat::R16G16B16A16_SNORM  : gl_format = GL_RGBA16_SNORM;       break;
		case SurfaceFormat::R16G16B16A16_SINT   : gl_format = GL_RGBA16I;            break;
		case SurfaceFormat::R32G32B32_FLOAT     : gl_format = GL_RGB32F;             break;
		case SurfaceFormat::R32G32B32_UINT      : gl_format = GL_RGB32UI;            break;
		case SurfaceFormat::R32G32B32_SINT      : gl_format = GL_RGB32I;             break;
		case SurfaceFormat::R32G32_FLOAT        : gl_format = GL_RG32F;              break;
		case SurfaceFormat::R32G32_UINT         : gl_format = GL_RG32UI;             break;
		case SurfaceFormat::R32G32_SINT         : gl_format = GL_RG32I;              break;
		case SurfaceFormat::D32_FLOAT_S8X24_UINT: gl_format = GL_DEPTH32F_STENCIL8;  break;
		case SurfaceFormat::R10G10B10A2_UNORM   : gl_format = GL_RGB10_A2;           break;
		case SurfaceFormat::R10G10B10A2_UINT    : gl_format = GL_RGB10_A2UI;         break;
		case SurfaceFormat::R11G11B10_FLOAT     : gl_format = GL_R11F_G11F_B10F;     break;
		case SurfaceFormat::R8G8B8A8_UNORM      : gl_format = GL_RGBA8;              break;
		case SurfaceFormat::R8G8B8A8_UNORM_SRGB : gl_format = GL_SRGB8_ALPHA8;       break;
		case SurfaceFormat::R8G8B8A8_UINT       : gl_format = GL_RGBA8UI;            break;
		case SurfaceFormat::R8G8B8A8_SNORM      : gl_format = GL_RGBA8_SNORM;        break;
		case SurfaceFormat::R8G8B8A8_SINT       : gl_format = GL_RGBA8I;             break;
		case SurfaceFormat::R8G8B8_UNORM        : gl_format = GL_RGB8;               break;
		case SurfaceFormat::R16G16_FLOAT        : gl_format = GL_RG16F;              break;
		case SurfaceFormat::R16G16_UNORM        : gl_format = GL_RG16;               break;
		case SurfaceFormat::R16G16_UINT         : gl_format = GL_RG16UI;             break;
		case SurfaceFormat::R16G16_SNORM        : gl_format = GL_RG16_SNORM;         break;
		case SurfaceFormat::R16G16_SINT         : gl_format = GL_RG16I;              break;
		case SurfaceFormat::D32_FLOAT           : gl_format = GL_DEPTH_COMPONENT32F; break;
		case SurfaceFormat::R32_FLOAT           : gl_format = GL_R32F;               break;
		case SurfaceFormat::R32_UINT            : gl_format = GL_R32UI;              break;
		case SurfaceFormat::R32_SINT            : gl_format = GL_R32I;               break;
		case SurfaceFormat::D24_UNORM_S8_UINT   : gl_format = GL_DEPTH24_STENCIL8;   break;
		case SurfaceFormat::R8G8_UNORM          : gl_format = GL_RG8;                break;
		case SurfaceFormat::R8G8_UINT           : gl_format = GL_RG8UI;              break;
		case SurfaceFormat::R8G8_SNORM          : gl_format = GL_RG8_SNORM;          break;
		case SurfaceFormat::R8G8_SINT           : gl_format = GL_RG8I;               break;
		case SurfaceFormat::R16_FLOAT           : gl_format = GL_R16F;               break;
		case SurfaceFormat::D16_UNORM           : gl_format = GL_DEPTH_COMPONENT16;  break;
		case SurfaceFormat::R16_UNORM           : gl_format = GL_R16;                break;
		case SurfaceFormat::R16_UINT            : gl_format = GL_R16UI;              break;
		case SurfaceFormat::R16_SNORM           : gl_format = GL_R16_SNORM;          break;
		case SurfaceFormat::R16_SINT            : gl_format = GL_R16I;               break;
		case SurfaceFormat::R8_UNORM            : gl_format = GL_R8;                 break;
		case SurfaceFormat::R8_UINT             : gl_format = GL_R8UI;               break;
		case SurfaceFormat::R8_SNORM            : gl_format = GL_R8_SNORM;           break;
		case SurfaceFormat::R8_SINT             : gl_format = GL_R8I;                break;
		default: VclDebugError("Unsupported colour format.");
		};
		// clang-format on

		return gl_format;
	}

	ImageFormat Texture::toImageFormat(SurfaceFormat type)
	{
		ImageFormat image_format = { GL_NONE, GL_NONE };

		// clang-format off
		switch (type) {
		case SurfaceFormat::R32G32B32A32_FLOAT:   image_format.Format = GL_RGBA;            image_format.Type = GL_FLOAT;                          break;
		case SurfaceFormat::R32G32B32A32_UINT:    image_format.Format = GL_RGBA_INTEGER;    image_format.Type = GL_UNSIGNED_INT;                   break;
		case SurfaceFormat::R32G32B32A32_SINT:    image_format.Format = GL_RGBA_INTEGER;    image_format.Type = GL_INT;                            break;
		case SurfaceFormat::R16G16B16A16_FLOAT:   image_format.Format = GL_RGBA;            image_format.Type = GL_HALF_FLOAT;                     break;
		case SurfaceFormat::R16G16B16A16_UNORM:   image_format.Format = GL_RGBA;            image_format.Type = GL_UNSIGNED_SHORT;                 break;
		case SurfaceFormat::R16G16B16A16_UINT:    image_format.Format = GL_RGBA_INTEGER;    image_format.Type = GL_UNSIGNED_SHORT;                 break;
		case SurfaceFormat::R16G16B16A16_SNORM:   image_format.Format = GL_RGBA;            image_format.Type = GL_SHORT;                          break;
		case SurfaceFormat::R16G16B16A16_SINT:    image_format.Format = GL_RGBA_INTEGER;    image_format.Type = GL_SHORT;                          break;
		case SurfaceFormat::R32G32B32_FLOAT:      image_format.Format = GL_RGB;             image_format.Type = GL_FLOAT;                          break;
		case SurfaceFormat::R32G32B32_UINT:       image_format.Format = GL_RGB_INTEGER;     image_format.Type = GL_UNSIGNED_INT;                   break;
		case SurfaceFormat::R32G32B32_SINT:       image_format.Format = GL_RGB_INTEGER;     image_format.Type = GL_INT;                            break;
		case SurfaceFormat::R32G32_FLOAT:         image_format.Format = GL_RG;              image_format.Type = GL_FLOAT;                          break;
		case SurfaceFormat::R32G32_UINT:          image_format.Format = GL_RG_INTEGER;      image_format.Type = GL_UNSIGNED_INT;                   break;
		case SurfaceFormat::R32G32_SINT:          image_format.Format = GL_RG_INTEGER;      image_format.Type = GL_INT;                            break;
		case SurfaceFormat::D32_FLOAT_S8X24_UINT: image_format.Format = GL_DEPTH_STENCIL;   image_format.Type = GL_FLOAT_32_UNSIGNED_INT_24_8_REV; break;
		case SurfaceFormat::R10G10B10A2_UNORM:    image_format.Format = GL_RGBA;            image_format.Type = GL_FLOAT;                          break;
		case SurfaceFormat::R10G10B10A2_UINT:     image_format.Format = GL_RGBA;            image_format.Type = GL_UNSIGNED_INT;                   break;
		case SurfaceFormat::R11G11B10_FLOAT:      image_format.Format = GL_RGBA;            image_format.Type = GL_FLOAT;                          break;
		case SurfaceFormat::R8G8B8A8_UNORM:       image_format.Format = GL_RGBA;            image_format.Type = GL_UNSIGNED_BYTE;                  break;
		case SurfaceFormat::R8G8B8A8_UNORM_SRGB:  image_format.Format = GL_RGBA;            image_format.Type = GL_UNSIGNED_BYTE;                  break;
		case SurfaceFormat::R8G8B8A8_UINT:        image_format.Format = GL_RGBA_INTEGER;    image_format.Type = GL_UNSIGNED_BYTE;                  break;
		case SurfaceFormat::R8G8B8A8_SNORM:       image_format.Format = GL_RGBA;            image_format.Type = GL_BYTE;                           break;
		case SurfaceFormat::R8G8B8A8_SINT:        image_format.Format = GL_RGBA_INTEGER;    image_format.Type = GL_BYTE;                           break;
		case SurfaceFormat::R8G8B8_UNORM:         image_format.Format = GL_RGB;             image_format.Type = GL_UNSIGNED_BYTE;                  break;
		case SurfaceFormat::R16G16_FLOAT:         image_format.Format = GL_RG;              image_format.Type = GL_HALF_FLOAT;                     break;
		case SurfaceFormat::R16G16_UNORM:         image_format.Format = GL_RG;              image_format.Type = GL_UNSIGNED_SHORT;                 break;
		case SurfaceFormat::R16G16_UINT:          image_format.Format = GL_RG_INTEGER;      image_format.Type = GL_UNSIGNED_SHORT;                 break;
		case SurfaceFormat::R16G16_SNORM:         image_format.Format = GL_RG;              image_format.Type = GL_SHORT;                          break;
		case SurfaceFormat::R16G16_SINT:          image_format.Format = GL_RG_INTEGER;      image_format.Type = GL_SHORT;                          break;
		case SurfaceFormat::D32_FLOAT:            image_format.Format = GL_DEPTH_COMPONENT; image_format.Type = GL_UNSIGNED_INT;                   break;
		case SurfaceFormat::R32_FLOAT:            image_format.Format = GL_RED;             image_format.Type = GL_FLOAT;                          break;
		case SurfaceFormat::R32_UINT:             image_format.Format = GL_RED_INTEGER;     image_format.Type = GL_UNSIGNED_INT;                   break;
		case SurfaceFormat::R32_SINT:             image_format.Format = GL_RED_INTEGER;     image_format.Type = GL_INT;                            break;
		case SurfaceFormat::D24_UNORM_S8_UINT:    image_format.Format = GL_DEPTH_STENCIL;   image_format.Type = GL_UNSIGNED_INT_24_8;              break;
		case SurfaceFormat::R8G8_UNORM:           image_format.Format = GL_RG;              image_format.Type = GL_UNSIGNED_BYTE;                  break;
		case SurfaceFormat::R8G8_UINT:            image_format.Format = GL_RG_INTEGER;      image_format.Type = GL_UNSIGNED_BYTE;                  break;
		case SurfaceFormat::R8G8_SNORM:           image_format.Format = GL_RG;              image_format.Type = GL_BYTE;                           break;
		case SurfaceFormat::R8G8_SINT:            image_format.Format = GL_RG_INTEGER;      image_format.Type = GL_BYTE;                           break;
		case SurfaceFormat::R16_FLOAT:            image_format.Format = GL_RED;             image_format.Type = GL_HALF_FLOAT;                     break;
		case SurfaceFormat::D16_UNORM:            image_format.Format = GL_DEPTH_COMPONENT; image_format.Type = GL_UNSIGNED_SHORT;                 break;
		case SurfaceFormat::R16_UNORM:            image_format.Format = GL_RED;             image_format.Type = GL_UNSIGNED_SHORT;                 break;
		case SurfaceFormat::R16_UINT:             image_format.Format = GL_RED_INTEGER;     image_format.Type = GL_UNSIGNED_SHORT;                 break;
		case SurfaceFormat::R16_SNORM:            image_format.Format = GL_RED;             image_format.Type = GL_SHORT;                          break;
		case SurfaceFormat::R16_SINT:             image_format.Format = GL_RED_INTEGER;     image_format.Type = GL_SHORT;                          break;
		case SurfaceFormat::R8_UNORM:             image_format.Format = GL_RED;             image_format.Type = GL_UNSIGNED_BYTE;                  break;
		case SurfaceFormat::R8_UINT:              image_format.Format = GL_RED_INTEGER;     image_format.Type = GL_UNSIGNED_BYTE;                  break;
		case SurfaceFormat::R8_SNORM:             image_format.Format = GL_RED;             image_format.Type = GL_BYTE;                           break;
		case SurfaceFormat::R8_SINT:              image_format.Format = GL_RED_INTEGER;     image_format.Type = GL_BYTE;                           break;
		default: VclDebugError("Unsupported colour format.");
		}
		// clang-format on

		return image_format;
	}

	GLenum Texture::toTextureType(TextureType type)
	{
		// clang-format off
		switch (type) {
		case TextureType::Texture1D:        return GL_TEXTURE_1D;
		case TextureType::Texture1DArray:   return GL_TEXTURE_1D_ARRAY;
		case TextureType::Texture2D:        return GL_TEXTURE_2D;
		case TextureType::Texture2DArray:   return GL_TEXTURE_2D_ARRAY;
		case TextureType::Texture2DMS:      return GL_TEXTURE_2D_MULTISAMPLE;
		case TextureType::Texture2DMSArray: return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
		case TextureType::Texture3D:        return GL_TEXTURE_3D;
		case TextureType::TextureCube:      return GL_TEXTURE_CUBE_MAP;
		case TextureType::TextureCubeArray: return GL_TEXTURE_CUBE_MAP_ARRAY;
		default: return GL_NONE;
		}
		// clang-format on
	}

	Texture::Texture(Texture&& rhs)
	: Runtime::Texture(std::move(rhs))
	, Resource(std::move(rhs))
	{
	}

	Texture::Texture(const Texture& rhs)
	: Runtime::Texture(rhs)
	, Resource()
	{
	}

	Texture::~Texture()
	{
		// Delete the texture
		glDeleteTextures(1, &_glId);
	}

	void Texture::clear(SurfaceFormat fmt, const void* data)
	{
		ImageFormat gl_fmt = toImageFormat(fmt);

		glClearTexSubImage(_glId, 0, 0, 0, 0, width(), height(), depth(), gl_fmt.Format, gl_fmt.Type, data);
	}

	void Texture::copyTo(Buffer& target, size_t dstOffset) const
	{
		auto binder = target.bind(GL_PIXEL_PACK_BUFFER);
		read(sizeInBytes(), (void*)dstOffset);
	}

	void Texture::initialise(const TextureResource* init_data /* = nullptr */)
	{
		GLenum tex_type = toTextureType(type());
		GLenum colour_fmt = toSurfaceFormat(format());

#if defined(VCL_GL_ARB_direct_state_access)
		glCreateTextures(tex_type, 1, &_glId);
		allocImpl(colour_fmt);

		// Configure texture
		glTextureParameteri(_glId, GL_TEXTURE_BASE_LEVEL, firstMipMapLevel());
		glTextureParameteri(_glId, GL_TEXTURE_MAX_LEVEL, firstMipMapLevel() + mipMapLevels() - 1);
#elif defined(VCL_GL_EXT_direct_state_access)
		glGenTextures(1, &_glId);
		allocImpl(colour_fmt);

		// Configure texture
		glTextureParameteriEXT(_glId, tex_type, GL_TEXTURE_BASE_LEVEL, firstMipMapLevel());
		glTextureParameteriEXT(_glId, tex_type, GL_TEXTURE_MAX_LEVEL, firstMipMapLevel() + mipMapLevels() - 1);
#else
		glCreateTextures(tex_type, 1, &_glId);
		TextureBindPoint bp(tex_type, _glId);
		allocImpl(colour_fmt);

		// Configure texture
		glTexParameteri(tex_type, GL_TEXTURE_BASE_LEVEL, firstMipMapLevel());
		glTexParameteri(tex_type, GL_TEXTURE_MAX_LEVEL, firstMipMapLevel() + mipMapLevels() - 1);
#endif

		if (init_data)
			updateImpl(*init_data);
	}

	void Texture::update(const TextureResource& data)
	{
#if !defined(VCL_GL_ARB_direct_state_access) && !defined(VCL_GL_EXT_direct_state_access)
		TextureBindPoint bp(toTextureType(type()), _glId);
#endif
		updateImpl(data);
	}

	void Texture::read(size_t size, void* data) const
	{
		SurfaceFormat fmt = this->format();
		ImageFormat gl_fmt = toImageFormat(fmt);

		// Read to 'data', not to a device buffer
		// glBindBuffer(GL_PIXEL_PACK_BUFFER, GL_NONE);

#if defined(VCL_GL_ARB_direct_state_access)
		glGetTextureImage(_glId, 0, gl_fmt.Format, gl_fmt.Type, (GLsizei)size, data);
#else
		GLenum tex_type = toTextureType(type());
		TextureBindPoint bp(tex_type, _glId);
		if (tex_type != GL_TEXTURE_CUBE_MAP)
		{
#	if defined(VCL_GL_EXT_direct_state_access)
			glGetTextureImageEXT(_glId, tex_type, 0, gl_fmt.Format, gl_fmt.Type, data);
#	else
			glGetTexImage(tex_type, 0, gl_fmt.Format, gl_fmt.Type, data);
#	endif
		} else
		{
			const GLenum faces[] = {
				GL_TEXTURE_CUBE_MAP_POSITIVE_X,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
				GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
			};
			int pixel_size = Vcl::Graphics::sizeInBytes(fmt);
			GLsizei w = (GLsizei)width();
			GLsizei h = (GLsizei)height();

			unsigned char* data_ptr = reinterpret_cast<unsigned char*>(data);
			for (const auto face : faces)
			{
#	if defined(VCL_GL_EXT_direct_state_access)
				glGetTextureImageEXT(_glId, face, 0, gl_fmt.Format, gl_fmt.Type, data_ptr);
#	else
				glGetTexImage(face, 0, gl_fmt.Format, gl_fmt.Type, data_ptr);
#	endif
				data_ptr += w * h * pixel_size;
			}
		}
#endif
	}
}}}}
