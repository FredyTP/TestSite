include_directories("${PROJECT_SOURCE_DIR}/../bullet/include")

file(GLOB BG2E_SRC	*.cpp
					system/*.cpp
					system/windows/*.cpp
                    base/*.cpp)
file(GLOB_RECURSE BG2E_INC "${PROJECT_SOURCE_DIR}/include/*.hpp")

add_library(bg2e SHARED ${BG2E_SRC} ${BG2E_INC})

set(VWNET_ROOT "${PROJECT_SOURCE_DIR}/../vwnet/")  
set(VWNET_LIB "${VWNET_ROOT}build/bin/")
include_directories("${VWGL_ROOT}cplusplus/include")
include_directories("${VWNET_ROOT}cplusplus/include")

IF (APPLE)

# TODO: Fill in bullet, vwgl and vwnet dependencies

ELSEIF (WIN32)
	IF (${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013 Win64")
        message("Visual Studio 2013 64 bits")
        set(BULLET_LIB "${PROJECT_SOURCE_DIR}/../bullet/lib/win64/vs12/")
    ELSEIF (${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
        message("Visual Studio 2015 64 bits")
        set(BULLET_LIB "${PROJECT_SOURCE_DIR}/../bullet/lib/win64/vs14/")
    ENDIF (${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013 Win64")
	
    # Bullet physics
    target_link_libraries(bg2e debug "${BULLET_LIB}Bullet2FileLoader_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}Bullet3Collision_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}Bullet3Common_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}Bullet3Dynamics_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}Bullet3Geometry_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}Bullet3OpenCL_clew_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}BulletCollision_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}BulletDynamics_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}BulletFileLoader_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}BulletInverseDynamicsUtils_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}BulletInverseDynamics_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}BulletSoftBody_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}BulletWorldImporter_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}BulletXmlWorldImporter_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}ConvexDecomposition_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}GIMPACTUtils_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}gtest_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}gwen_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}HACD_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}LinearMath_Debug.lib")
    target_link_libraries(bg2e debug "${BULLET_LIB}OpenGLWindow_Debug.lib")

    target_link_libraries(bg2e optimized "${BULLET_LIB}Bullet2FileLoader.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}Bullet3Collision.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}Bullet3Common.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}Bullet3Dynamics.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}Bullet3Geometry.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}Bullet3OpenCL_clew.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}BulletCollision.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}BulletDynamics.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}BulletFileLoader.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}BulletInverseDynamicsUtils.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}BulletInverseDynamics.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}BulletSoftBody.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}BulletWorldImporter.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}BulletXmlWorldImporter.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}ConvexDecomposition.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}GIMPACTUtils.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}gtest.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}gwen.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}HACD.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}LinearMath.lib")
    target_link_libraries(bg2e optimized "${BULLET_LIB}OpenGLWindow.lib")

    # VitaminewNET
    target_link_libraries(bg2e debug "${VWNET_LIB}vwnetd.lib")
    target_link_libraries(bg2e optimized "${VWNET_LIB}vwnet.lib")
    
    #set_property(SOURCE system/windows/path-win.cpp PROPERTY COMPILE_FLAGS "/clr")
	string (REPLACE "/EHsc" "/EHa" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string (REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    string (REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set_target_properties(bg2e PROPERTIES DEBUG_POSTFIX "d")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
ELSEIF (UNIX)

ENDIF (APPLE)

install (TARGETS bg2e DESTINATION bin)

if (WIN32)
	install(FILES   "${VWNET_LIB}vwnet.dll"
                    "${VWNET_LIB}vwnetd.dll"
                    "${VWNET_LIB}libcurl.dll"
                    "${VWNET_LIB}libcurld.dll"
                    DESTINATION bin)
endif (WIN32)



