######################################################################
# Automatically generated by qmake (3.0) ?? 3? 30 14:29:47 2022
######################################################################

######################################################################
# Automatically generated by qmake (2.01a) ?? ?? 11 22:53:03 2015
######################################################################

TEMPLATE = app
TARGET = CadCanvas
CONFIG(x64){
TARGET = $$TARGET"64"
}
include($$(SOUI4PATH)/demo_com.pri)

CONFIG(debug, debug|release) {
	DESTDIR = ../Debug
}
else {
	DESTDIR = ../Release
}
PRECOMPILED_HEADER = stdafx.h
RC_FILE += cadCanvas.rc

# Input
HEADERS += MainDlg.h \
           ProbeSelDlg.h \
           resource.h \
           SCadCanvas.h \
           dxflib3170/dl_attributes.h \
           dxflib3170/dl_codes.h \
           dxflib3170/dl_creationadapter.h \
           dxflib3170/dl_creationinterface.h \
           dxflib3170/dl_dxf.h \
           dxflib3170/dl_entities.h \
           dxflib3170/dl_exception.h \
           dxflib3170/dl_extrusion.h \
           dxflib3170/dl_global.h \
           dxflib3170/dl_writer.h \
           dxflib3170/dl_writer_ascii.h \
           Objects/ARC.h \
           Objects/CadObj.h \
           Objects/Circle.h \
           Objects/Composite.h \
           Objects/Line.h \
           Objects/Poly.h \
           Objects/Text.h \
           Objects/YJPoint.h \
           Objects/YJPublic.h \
           res/resource.h

SOURCES += cadCanvas.cpp \
           MainDlg.cpp \
           ProbeSelDlg.cpp \
           SCadCanvas.cpp \
           dxflib3170/dl_dxf.cpp \
           dxflib3170/dl_writer_ascii.cpp \
           Objects/ARC.cpp \
           Objects/CadObj.cpp \
           Objects/Circle.cpp \
           Objects/Composite.cpp \
           Objects/Line.cpp \
           Objects/Poly.cpp \
           Objects/Text.cpp
