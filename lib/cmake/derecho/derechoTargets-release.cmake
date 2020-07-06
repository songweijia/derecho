#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "derecho" for configuration "Release"
set_property(TARGET derecho APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(derecho PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "rdmacm;ibverbs;rt;pthread;atomic;stdc++fs;/usr/local/lib/libfabric.so;-L/home/bmp53/derecho/lib -lmutils;-L/home/bmp53/derecho/lib -lmutils-tasks;/usr/lib/x86_64-linux-gnu/libssl.so;/usr/lib/x86_64-linux-gnu/libcrypto.so"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libderecho.so.0.9.2"
  IMPORTED_SONAME_RELEASE "libderecho.so.0.9"
  )

list(APPEND _IMPORT_CHECK_TARGETS derecho )
list(APPEND _IMPORT_CHECK_FILES_FOR_derecho "${_IMPORT_PREFIX}/lib/libderecho.so.0.9.2" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
