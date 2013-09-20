#############################################################################
# Makefile for building: FlashContainerLinux
# Generated by qmake (2.01a) (Qt 4.7.4) on: ?? 9? 20 14:32:21 2013
# Project:  FlashContainerLinux.pro
# Template: app
# Command: /usr/local/Trolltech/Qt-4.7.4/bin/qmake -spec /usr/local/Trolltech/Qt-4.7.4/mkspecs/linux-g++ CONFIG+=debug -o Makefile FlashContainerLinux.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DHAVE_NETINET_IN_H -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/local/Trolltech/Qt-4.7.4/mkspecs/linux-g++ -I. -I/usr/local/Trolltech/Qt-4.7.4/include/QtCore -I/usr/local/Trolltech/Qt-4.7.4/include/QtNetwork -I/usr/local/Trolltech/Qt-4.7.4/include/QtGui -I/usr/local/Trolltech/Qt-4.7.4/include -Inpapiinclude -I.
LINK          = g++
LFLAGS        = -Wl,-rpath,/usr/local/Trolltech/Qt-4.7.4/lib
LIBS          = $(SUBLIBS)  -L/usr/local/Trolltech/Qt-4.7.4/lib -lQtGui -L/usr/local/Trolltech/Qt-4.7.4/lib -L/usr/X11R6/lib -lQtNetwork -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/local/Trolltech/Qt-4.7.4/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		FlashPlugin.cpp \
		Container.cpp \
		npn.cpp \
		FlashContentLoader.cpp \
		faststrdup.cpp \
		Identifier.cpp \
		LocationNPClass.cpp \
		WindowNPClass.cpp moc_Container.cpp \
		moc_FlashContentLoader.cpp
OBJECTS       = main.o \
		FlashPlugin.o \
		Container.o \
		npn.o \
		FlashContentLoader.o \
		faststrdup.o \
		Identifier.o \
		LocationNPClass.o \
		WindowNPClass.o \
		moc_Container.o \
		moc_FlashContentLoader.o
DIST          = /usr/local/Trolltech/Qt-4.7.4/mkspecs/common/g++.conf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/common/unix.conf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/common/linux.conf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/qconfig.pri \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/qt_functions.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/qt_config.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/exclusive_builds.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/default_pre.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/debug.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/default_post.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/warn_on.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/qt.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/unix/thread.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/moc.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/resources.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/uic.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/yacc.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/lex.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/include_source_dir.prf \
		FlashContainerLinux.pro
QMAKE_TARGET  = FlashContainerLinux
DESTDIR       = 
TARGET        = FlashContainerLinux

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: FlashContainerLinux.pro  /usr/local/Trolltech/Qt-4.7.4/mkspecs/linux-g++/qmake.conf /usr/local/Trolltech/Qt-4.7.4/mkspecs/common/g++.conf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/common/unix.conf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/common/linux.conf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/qconfig.pri \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/qt_functions.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/qt_config.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/exclusive_builds.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/default_pre.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/debug.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/default_post.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/warn_on.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/qt.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/unix/thread.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/moc.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/resources.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/uic.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/yacc.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/lex.prf \
		/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/include_source_dir.prf \
		/usr/local/Trolltech/Qt-4.7.4/lib/libQtGui.prl \
		/usr/local/Trolltech/Qt-4.7.4/lib/libQtCore.prl \
		/usr/local/Trolltech/Qt-4.7.4/lib/libQtNetwork.prl
	$(QMAKE) -spec /usr/local/Trolltech/Qt-4.7.4/mkspecs/linux-g++ CONFIG+=debug -o Makefile FlashContainerLinux.pro
/usr/local/Trolltech/Qt-4.7.4/mkspecs/common/g++.conf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/common/unix.conf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/common/linux.conf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/qconfig.pri:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/modules/qt_webkit_version.pri:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/qt_functions.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/qt_config.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/exclusive_builds.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/default_pre.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/debug.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/default_post.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/warn_on.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/qt.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/unix/thread.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/moc.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/resources.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/uic.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/yacc.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/lex.prf:
/usr/local/Trolltech/Qt-4.7.4/mkspecs/features/include_source_dir.prf:
/usr/local/Trolltech/Qt-4.7.4/lib/libQtGui.prl:
/usr/local/Trolltech/Qt-4.7.4/lib/libQtCore.prl:
/usr/local/Trolltech/Qt-4.7.4/lib/libQtNetwork.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/local/Trolltech/Qt-4.7.4/mkspecs/linux-g++ CONFIG+=debug -o Makefile FlashContainerLinux.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/FlashContainerLinux1.0.0 || $(MKDIR) .tmp/FlashContainerLinux1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/FlashContainerLinux1.0.0/ && $(COPY_FILE) --parents FlashPlugin.h npapiinclude/npfunctions.h npapiinclude/npapi.h npn.h npapiinclude/nptypes.h npapiinclude/npruntime.h Container.h FlashContentLoader.h FRException.h faststrdup.h Identifier.h LocationNPClass.h WindowNPClass.h .tmp/FlashContainerLinux1.0.0/ && $(COPY_FILE) --parents main.cpp FlashPlugin.cpp Container.cpp npn.cpp FlashContentLoader.cpp faststrdup.cpp Identifier.cpp LocationNPClass.cpp WindowNPClass.cpp .tmp/FlashContainerLinux1.0.0/ && (cd `dirname .tmp/FlashContainerLinux1.0.0` && $(TAR) FlashContainerLinux1.0.0.tar FlashContainerLinux1.0.0 && $(COMPRESS) FlashContainerLinux1.0.0.tar) && $(MOVE) `dirname .tmp/FlashContainerLinux1.0.0`/FlashContainerLinux1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/FlashContainerLinux1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_Container.cpp moc_FlashContentLoader.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_Container.cpp moc_FlashContentLoader.cpp
moc_Container.cpp: FlashPlugin.h \
		Container.h
	/usr/local/Trolltech/Qt-4.7.4/bin/moc $(DEFINES) $(INCPATH) Container.h -o moc_Container.cpp

moc_FlashContentLoader.cpp: FlashContentLoader.h
	/usr/local/Trolltech/Qt-4.7.4/bin/moc $(DEFINES) $(INCPATH) FlashContentLoader.h -o moc_FlashContentLoader.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

main.o: main.cpp Container.h \
		FlashPlugin.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

FlashPlugin.o: FlashPlugin.cpp FlashPlugin.h \
		npn.h \
		faststrdup.h \
		FlashContentLoader.h \
		Identifier.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o FlashPlugin.o FlashPlugin.cpp

Container.o: Container.cpp Container.h \
		FlashPlugin.h \
		faststrdup.h \
		Identifier.h \
		npn.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Container.o Container.cpp

npn.o: npn.cpp FlashPlugin.h \
		npn.h \
		Identifier.h \
		faststrdup.h \
		FlashContentLoader.h \
		Container.h \
		WindowNPClass.h \
		LocationNPClass.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o npn.o npn.cpp

FlashContentLoader.o: FlashContentLoader.cpp FlashContentLoader.h \
		faststrdup.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o FlashContentLoader.o FlashContentLoader.cpp

faststrdup.o: faststrdup.cpp faststrdup.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o faststrdup.o faststrdup.cpp

Identifier.o: Identifier.cpp Identifier.h \
		faststrdup.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Identifier.o Identifier.cpp

LocationNPClass.o: LocationNPClass.cpp Container.h \
		FlashPlugin.h \
		LocationNPClass.h \
		Identifier.h \
		faststrdup.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o LocationNPClass.o LocationNPClass.cpp

WindowNPClass.o: WindowNPClass.cpp WindowNPClass.h \
		Identifier.h \
		faststrdup.h \
		npn.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o WindowNPClass.o WindowNPClass.cpp

moc_Container.o: moc_Container.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_Container.o moc_Container.cpp

moc_FlashContentLoader.o: moc_FlashContentLoader.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_FlashContentLoader.o moc_FlashContentLoader.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:
