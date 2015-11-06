#-------------------------------------------------
#
# Project created by QtCreator 2013-06-08T22:34:13
#
#-------------------------------------------------

TARGET = my_logger 

CONFIG -= qt

#DEFINES += DEBUG_MODE
#QMAKE_CXXFLAGS_DEBUG -= -g 
#QMAKE_CXXFLAGS_DEBUG += -ggdb 
QMAKE_CXXFLAGS_DEBUG += -Wpedantic 


QMAKE_CXXFLAGS += -std=c++11


SOURCES += main.cpp \
           my_logger.cpp

HEADERS += my_logger.hpp

LIBS += -L/usr/lib -lpthread 
