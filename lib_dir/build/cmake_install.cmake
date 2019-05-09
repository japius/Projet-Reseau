# Install script for directory: /home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/libwebsockets.pc")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/libwebsockets_static.pc")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev_headers")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/include/libwebsockets")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libraries")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/lib/libwebsockets.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/include/libwebsockets.h"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/lws_config.h"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/plugins/ssh-base/include/lws-plugin-ssh.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libraries")
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwebsockets.so.14"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwebsockets.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/lib/libwebsockets.so.14"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/lib/libwebsockets.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwebsockets.so.14"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libwebsockets.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/include/libwebsockets.h"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/lws_config.h"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/plugins/ssh-base/include/lws-plugin-ssh.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "examples")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-server" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-server")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-server"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/bin/libwebsockets-test-server")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-server" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-server")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-server")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "examples")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-server-extpoll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-server-extpoll")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-server-extpoll"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/bin/libwebsockets-test-server-extpoll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-server-extpoll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-server-extpoll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-server-extpoll")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "examples")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-lejp" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-lejp")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-lejp"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/bin/libwebsockets-test-lejp")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-lejp" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-lejp")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-lejp")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "examples")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-client" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-client")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-client"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/bin/libwebsockets-test-client")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-client" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-client")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/libwebsockets-test-client")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "examples")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libwebsockets-test-server" TYPE FILE FILES
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/libwebsockets-test-server.key.pem"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/libwebsockets-test-server.pem"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/favicon.ico"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/leaf.jpg"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/candide.zip"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/libwebsockets.org-logo.svg"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/http2.png"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/wss-over-h2.png"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/lws-common.js"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/test.html"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/test.css"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/test.js"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "examples")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libwebsockets-test-server/private" TYPE FILE FILES "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/private/index.html")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "examples")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libwebsockets-test-server" TYPE FILE FILES
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/lws-ssh-test-keys"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/libwebsockets/test-apps/lws-ssh-test-keys.pub"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libwebsockets" TYPE FILE FILES
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/CMakeFiles/LibwebsocketsConfig.cmake"
    "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/LibwebsocketsConfigVersion.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libwebsockets/LibwebsocketsTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libwebsockets/LibwebsocketsTargets.cmake"
         "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/CMakeFiles/Export/lib/cmake/libwebsockets/LibwebsocketsTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libwebsockets/LibwebsocketsTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libwebsockets/LibwebsocketsTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libwebsockets" TYPE FILE FILES "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/CMakeFiles/Export/lib/cmake/libwebsockets/LibwebsocketsTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libwebsockets" TYPE FILE FILES "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/CMakeFiles/Export/lib/cmake/libwebsockets/LibwebsocketsTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/faridah/real/Desktop/Trd_Year/PR6/marron_et_vert_c_marron_qui_gagne/lib_dir/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
