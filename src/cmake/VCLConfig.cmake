# Determine the compiler vendor
MESSAGE(STATUS "Detecting compiler: ${CMAKE_CXX_COMPILER_ID}")
IF(MSVC)
	SET(VCL_COMPILER_MSVC ON)
ELSEIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
	SET(VCL_COMPILER_INTEL ON)
ELSEIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	SET(VCL_COMPILER_GNU ON)
ELSEIF("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	SET(VCL_COMPILER_CLANG ON)
ELSE()
	SET(VCL_COMPILER_UNKNOWN ON)
ENDIF()

# Determine platform architexture
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
	SET(VCL_ADDRESS_SIZE "64")
ELSE()
	SET(VCL_ADDRESS_SIZE "32")
ENDIF()
MESSAGE(STATUS "Compiling for ${VCL_ADDRESS_SIZE}bit machine")

# Determine the underlying OS
MESSAGE(STATUS "Running on ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")

# Enable Visual Studio solution folders
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Eigen
SET(VCL_EIGEN_DIR CACHE PATH "Directory of Eigen")
INCLUDE_DIRECTORIES(${VCL_EIGEN_DIR})
INCLUDE_DIRECTORIES(${VCL_EIGEN_DIR}/unsupported)

# Control OpenMP support
SET(VCL_OPENMP_SUPPORT CACHE BOOL "Enable OpenMP support")
FIND_PACKAGE(OpenMP)
IF(OPENMP_FOUND AND VCL_OPENMP_SUPPORT)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ENDIF()

# Control OpenCL support
SET(VCL_OPENCL_SUPPORT CACHE BOOL "Enable OpenCL support")
IF(VCL_OPENCL_SUPPORT)
	FIND_PACKAGE(OpenCL)
	IF(OPENCL_FOUND)
		ADD_DEFINITIONS(-DVCL_OPENCL_SUPPORT)
	ENDIF()
ENDIF()

# Control OpenGL support
SET(VCL_OPENGL_SUPPORT CACHE BOOL "Enable OpenGL support")
IF(VCL_OPENGL_SUPPORT)
	FIND_PACKAGE(GLEW)
	IF(GLEW_FOUND)
		ADD_DEFINITIONS(-DVCL_OPENGL_SUPPORT)
	ENDIF()
ENDIF()

# nvToolsExt
SET(VCL_NVTOOLSEXT_SUPPORT CACHE BOOL "Turn on the VCL nvToolsExt support")
IF(VCL_NVTOOLSEXT_SUPPORT)
	SET(VCL_NVTOOLSEXT_INCLUDE CACHE PATH "Include directory of nvToolsExt")
	SET(VCL_NVTOOLSEXT_LIBRARY CACHE FILEPATH "nvToolsExt library")
	ADD_DEFINITIONS(-DVCL_NVTOOLSEXT_SUPPORT)
	INCLUDE_DIRECTORIES(${VCL_NVTOOLSEXT_INCLUDE})
ENDIF(VCL_NVTOOLSEXT_SUPPORT)

# Define vectorisation
SET(VCL_VECTORIZE_SSE2 CACHE BOOL "Enable SSE 2 instruction set")
SET(VCL_VECTORIZE_SSE3 CACHE BOOL "Enable SSE 3 instruction set")
SET(VCL_VECTORIZE_SSSE3 CACHE BOOL "Enable SSEE 3 instruction set")
SET(VCL_VECTORIZE_SSE4_1 CACHE BOOL "Enable SSE 4.1 instruction set")
SET(VCL_VECTORIZE_SSE4_2 CACHE BOOL "Enable SSE 4.2 instruction set")
SET(VCL_VECTORIZE_AVX CACHE BOOL "Enable AVX instruction set")
SET(VCL_VECTORIZE_AVX2 CACHE BOOL "Enable AVX 2 instruction set")

# Set whether contracts should be used
SET(VCL_USE_CONTRACTS CACHE BOOL "Enable contracts")

# Configure MSVC compiler
IF(VCL_COMPILER_MSVC)
	# Configure release configuration
	SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /GS- /fp:fast")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GS- /fp:fast")
	
	# Configure all configuration
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	
	# Make AVX available
	IF(VCL_VECTORIZE_AVX2)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:AVX2")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
	ELSEIF(VCL_VECTORIZE_AVX)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:AVX")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX")
	ELSEIF(VCL_VECTORIZE_SSE2 AND VCL_ADDRESS_SIZE EQUAL "32")
		# All x64 bit machine come with SSE2, thus it's defined as default
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:SSE2")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
	ENDIF()

ENDIF(VCL_COMPILER_MSVC)

# Configure GCC and CLANG
IF(VCL_COMPILER_GNU OR VCL_COMPILER_CLANG)

	# Configure all configuration
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c++0x")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")

	IF(VCL_VECTORIZE_AVX2)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx2")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")
	ELSEIF(VCL_VECTORIZE_AVX)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
	ELSEIF(VCL_VECTORIZE_SSE4_2)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.2")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2")
	ELSEIF(VCL_VECTORIZE_SSE4_1)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.1")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
	ELSEIF(VCL_VECTORIZE_SSSE3)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msss3")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mssse3")
	ELSEIF(VCL_VECTORIZE_SSE3)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse3")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse3")
	ELSEIF(VCL_VECTORIZE_SSE2)
		SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse2")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
	ENDIF()
ENDIF(VCL_COMPILER_GNU OR VCL_COMPILER_CLANG)
