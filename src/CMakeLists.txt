
add_executable (doit doit.cpp datastore.cpp cmdline.cpp)

target_link_libraries (doit sqlite3 boost_program_options boost_system)

install (TARGETS doit DESTINATION /usr/bin)
