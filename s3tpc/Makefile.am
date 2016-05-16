# build libs3tp and the s3tp tool

# c11 plus all warnings
AM_CXXFLAGS = -std=c++14 -Wall -Wextra -pedantic -pthread

# s3tp library
lib_LTLIBRARIES = libs3tp.la
libs3tp_la_LDFLAGS = -no-undefined -pthread
libs3tp_la_SOURCES = \
                     s3tpc.cpp\
                     s3tpc_c.cpp\
                     close_connection_event.cpp\
                     connect_event.cpp\
                     connection.cpp\
                     dispatcher.cpp\
                     event.cpp\
                     network_event.cpp\
                     new_connection_event.cpp\
                     protocol_exception.cpp\
                     protocol_handler.cpp\
                     ringbuffer.cpp\
                     stop_event.cpp

s3tpincludedir = $(includedir)/s3tp
s3tpinclude_HEADERS = \
                      s3tp.h

# s3tp executable
bin_PROGRAMS = s3tp rbtest
s3tp_SOURCES = s3tp_tool.c
s3tp_LDADD = libs3tp.la

rbtest_SOURCES = rb_test.cpp
rbtest_LDADD = ringbuffer.o
