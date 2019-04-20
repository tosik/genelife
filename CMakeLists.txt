cmake_minimum_required(VERSION 3.10)

set(PROJECTNAME genelife)

project(${PROJECTNAME})

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

add_subdirectory(src)

if (APPLE)
  set(PACKAGE_DIR ${CMAKE_BINARY_DIR}/package/${PROJECTNAME}.app)
  set(EXECUTABLE ${PACKAGE_DIR}/Contents/MacOS/${PROJECTNAME})
  add_custom_target(build-package
    COMMAND rm -rf ${PACKAGE_DIR}
    COMMAND mkdir -p ${PACKAGE_DIR}/Contents
  	COMMAND mkdir -p ${PACKAGE_DIR}/Contents/MacOS
  	COMMAND mkdir -p ${PACKAGE_DIR}/Contents/Resources
  	COMMAND mkdir -p ${PACKAGE_DIR}/Contents/Frameworks
  	COMMAND cp ${CMAKE_BINARY_DIR}/src/${PROJECTNAME} ${EXECUTABLE}
    COMMAND cp /usr/local/lib/libSDL2pp.8.dylib ${PACKAGE_DIR}/Contents/Frameworks/
  	COMMAND cp /usr/local/lib/libSDL2-2.0.0.dylib       ${PACKAGE_DIR}/Contents/Frameworks/
  	COMMAND cp /usr/local/lib/libSDL2_image-2.0.0.dylib ${PACKAGE_DIR}/Contents/Frameworks/
  	COMMAND cp /usr/local/lib/libSDL2_ttf-2.0.0.dylib   ${PACKAGE_DIR}/Contents/Frameworks/
  	COMMAND cp /usr/local/lib/libSDL2_mixer-2.0.0.dylib ${PACKAGE_DIR}/Contents/Frameworks/
  	COMMAND cp /usr/local/lib/libintl.8.dylib           ${PACKAGE_DIR}/Contents/Frameworks/
  	COMMAND cp /usr/lib/libSystem.B.dylib  	          	${PACKAGE_DIR}/Contents/Frameworks/
  	COMMAND cp /usr/lib/libc++.1.dylib            			${PACKAGE_DIR}/Contents/Frameworks/
  	COMMAND install_name_tool -change libSDL2pp.8.dylib        									@executable_path/../Frameworks/libSDL2pp.8.dylib ${EXECUTABLE}
  	COMMAND install_name_tool -change /usr/local/lib/libSDL2-2.0.0.dylib        @executable_path/../Frameworks/libSDL2-2.0.0.dylib ${EXECUTABLE}
  	COMMAND install_name_tool -change /usr/local/lib/libSDL2_image-2.0.0.dylib  @executable_path/../Frameworks/libSDL2_image-2.0.0.dylib ${EXECUTABLE}
  	COMMAND install_name_tool -change /usr/local/lib/libSDL2_ttf-2.0.0.dylib    @executable_path/../Frameworks/libSDL2_ttf-2.0.0.dylib ${EXECUTABLE}
  	COMMAND install_name_tool -change /usr/local/lib/libSDL2_mixer-2.0.0.dylib  @executable_path/../Frameworks/libSDL2_mixer-2.0.0.dylib ${EXECUTABLE}
  	COMMAND install_name_tool -change /usr/local/lib/libintl.8.dylib            @executable_path/../Frameworks/libintl.8.dylib ${EXECUTABLE}
  	COMMAND install_name_tool -change /usr/lib/libSystem.B.dylib            		@executable_path/../Frameworks/libSystem.B.dylib ${EXECUTABLE}
  	COMMAND install_name_tool -change /usr/lib/libc++.1.dylib            				@executable_path/../Frameworks/libc++.1.dylib ${EXECUTABLE}
  	COMMAND chmod +x ${EXECUTABLE}
    COMMENT Create binary package
    )
elseif(MSYS)
  set(PACKAGE_DIR ${CMAKE_BINARY_DIR}/package/${PROJECTNAME})
  set(EXECUTABLE ${PACKAGE_DIR}/${PROJECTNAME})
  add_custom_target(build-package
    COMMAND rm -rf ${PACKAGE_DIR}
    COMMAND mkdir -p ${PACKAGE_DIR}
  	COMMAND cp ${CMAKE_BINARY_DIR}/src/${PROJECTNAME} ${EXECUTABLE}
    COMMAND cp ~/sdl-package/bin/SDL2.dll ${PACKAGE_DIR}
    COMMAND cp ~/sdl-package/bin/SDL2_image.dll ${PACKAGE_DIR}
    COMMAND cp /mingw64/bin/libgcc_s_seh-1.dll ${PACKAGE_DIR}
    COMMAND cp /mingw64/bin/libwinpthread-1.dll ${PACKAGE_DIR}
	  COMMAND cp /mingw64/bin/libstdc++-6.dll ${PACKAGE_DIR}
    COMMENT Create binary package
    )
endif()

add_dependencies(build-package ${PROJECTNAME})
