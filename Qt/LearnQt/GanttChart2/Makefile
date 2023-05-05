#############################################################################
# Makefile for building QtGantt
# Cut'n Paste from a Makefile in the Qt 1.44 tutorial
#     Project: qtgantt
#    Template: app
#############################################################################

####### System-dependendant options

####### BEGIN

# Compiling
SYSCONF_CXX		= g++

# Compiling with support libraries
SYSCONF_CXXFLAGS_X11	= -I/usr/X11R6/include
SYSCONF_CXXFLAGS_QT	= -I$(QTDIR)/include
SYSCONF_CXXFLAGS_OPENGL	= -I/usr/X11R6/include

# Compiline YACC output
SYSCONF_CXXFLAGS_YACC     = -Wno-unused -Wno-parentheses

# Linking with support libraries
# X11
SYSCONF_LFLAGS_X11	= -L/usr/X11R6/lib
SYSCONF_LIBS_X11	= -lX11 -lXext
# Qt, Qt+OpenGL
SYSCONF_LFLAGS_QT	= -L$(QTDIR)/lib
SYSCONF_LIBS_QT		= -lqt
SYSCONF_LIBS_QT_OPENGL	= -lqgl
# OpenGL
SYSCONF_LFLAGS_OPENGL	= -L/usr/X11R6/lib
SYSCONF_LIBS_OPENGL	= -lMesaGL -lMesaGLU -lXmu -lXext -lm
# Yacc
SYSCONF_LIBS_YACC	= 

# Linking applications
SYSCONF_LINK		= g++
SYSCONF_LFLAGS		= 
SYSCONF_LIBS		= 

# Link flags shared objects
SYSCONF_LFLAGS_SHOBJ	= -shared

# Meta-object compiler
SYSCONF_MOC		= $(QTDIR)/bin/moc

# Linking shared libraries
#   - Build the $(TARGET) library, eg. lib$(TARGET).so.0.0
#   - Place target in $(DESTDIR) - which has a trailing /
#   - Usually needs to incorporate $(VER_MAJ) and $(VER_MIN)
#
SYSCONF_LINK_SHLIB	= g++
SYSCONF_LINK_TARGET_SHARED	= lib$(TARGET).so.$(VER_MAJ).$(VER_MIN)
SYSCONF_LINK_LIB_SHARED	=  $(SYSCONF_LINK_SHLIB) -shared -Wl,-soname,lib$(TARGET).so.$(VER_MAJ) \
				     $(LFLAGS) -o $(SYSCONF_LINK_TARGET_SHARED) \
				     $(OBJECTS) $(OBJMOC) $(LIBS); \
				 mv $(SYSCONF_LINK_TARGET_SHARED) $(DESTDIR); \
				 cd $(DESTDIR); \
				 rm -f lib$(TARGET).so lib$(TARGET).so.$(VER_MAJ); \
				 ln -s $(SYSCONF_LINK_TARGET_SHARED) lib$(TARGET).so; \
				 ln -s $(SYSCONF_LINK_TARGET_SHARED) lib$(TARGET).so.$(VER_MAJ)

# Linking static libraries
#   - Build the $(TARGET) library, eg. lib$(TARGET).a
#   - Place target in $(DESTDIR) - which has a trailing /
#
SYSCONF_AR		= ar cqs
SYSCONF_LINK_TARGET_STATIC = lib$(TARGET).a
SYSCONF_LINK_LIB_STATIC	= rm -f $(DESTDIR)$(SYSCONF_LINK_TARGET_STATIC) ; \
				 $(SYSCONF_AR) $(DESTDIR)$(SYSCONF_LINK_TARGET_STATIC) $(OBJECTS) $(OBJMOC) 
# Compiling application source
# MJG commented out options while debugging SYSCONF_CXXFLAGS	= -pipe -O2 -fno-strength-reduce -g
SYSCONF_CXXFLAGS	= -g
# Compiling library source
SYSCONF_CXXFLAGS_LIB	= -O2 -fno-strength-reduce -fPIC
# Compiling shared-object source
SYSCONF_CXXFLAGS_SHOBJ	= -O2 -fno-strength-reduce -fPIC
# Default link type (stati linking is still be used where required)
SYSCONF_LINK_LIB	= $(SYSCONF_LINK_LIB_SHARED)
SYSCONF_LINK_TARGET	= $(SYSCONF_LINK_TARGET_SHARED)

####### End of automatically generated section
####### END

####### Compiler, tools and options

CXX	=	$(SYSCONF_CXX)
CXXFLAGS=	$(SYSCONF_CXXFLAGS_QT) $(SYSCONF_CXXFLAGS)
LFLAGS	=	$(SYSCONF_LFLAGS_QT) $(SYSCONF_LFLAGS_X11) $(SYSCONF_LFLAGS)
LIBS	=	$(SYSCONF_LIBS_QT) $(SYSCONF_LIBS_X11) $(SYSCONF_LIBS)
MOC	=	$(SYSCONF_MOC)

####### Target

TARGET	= qtgantt
DESTDIR = ./
VER_MAJ = 1
VER_MIN = 0

####### Files

HEADERS	=	gantt.h project.h task.h ganttview.h ganttchart.h taskview.h \
		calendarview.h datehelper.h \
		taskinfodialog.h taskinfodialogimpl.h \
		projectinfodialog.h projectinfodialogimpl.h \
		optionsdialog.h optionsdialogimpl.h

SOURCES	=	gantt.cpp main.cpp mainwidget.cpp project.cpp task.cpp \
		taskview.cpp taskeditor.cpp ganttview.cpp ganttchart.cpp \
		calendarview.cpp datehelper.cpp \
		taskinfodialog.cpp taskinfodialogimpl.cpp \
		projectinfodialog.cpp projectinfodialogimpl.cpp \
		optionsdialog.cpp optionsdialogimpl.cpp

OBJECTS	=	gantt.o main.o mainwidget.o project.o task.o ganttview.o \
		ganttchart.o calendarview.o datehelper.o \
		taskview.o taskeditor.o \
		taskinfodialog.o taskinfodialogimpl.o \
		projectinfodialog.o projectinfodialogimpl.o \
		optionsdialog.o optionsdialogimpl.o

SRCMOC	=	moc_gantt.cpp moc_mainwidget.cpp moc_ganttview.cpp \
		moc_ganttchart.cpp moc_taskview.cpp moc_taskeditor.cpp \
		moc_calendarview.cpp \
		moc_taskinfodialog.cpp moc_taskinfodialogimpl.cpp \
		moc_projectinfodialog.cpp moc_projectinfodialogimpl.cpp \
		moc_optionsdialog.cpp moc_optionsdialogimpl.cpp

OBJMOC	=	moc_gantt.o moc_mainwidget.o moc_ganttview.o moc_ganttchart.o \
		moc_taskview.o moc_taskeditor.o moc_calendarview.o \
		moc_taskinfodialog.o moc_taskinfodialogimpl.o \
		moc_projectinfodialog.o moc_projectinfodialogimpl.o \
		moc_optionsdialog.o moc_optionsdialogimpl.o

WIDGETDIRS	=	calendar

WIDGETLIBS	=	-Lcalendar -lqtcal

####### Implicit rules

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: widgets $(DESTDIR)$(TARGET)

$(DESTDIR)$(TARGET): $(OBJECTS) $(OBJMOC)
	$(SYSCONF_LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(WIDGETOBJS) $(OBJMOC) $(LIBS) \
	$(WIDGETLIBS)

widgets:
	@list='$(WIDGETDIRS)'; for subdir in $$list; do \
	  echo "Making $@ in $$subdir"; \
	  (cd $$subdir && $(MAKE) ) \
	done;
 


moc: $(SRCMOC)

tmake:
	tmake qtgantt.pro

clean:
	-rm -f $(OBJECTS) $(OBJMOC) $(SRCMOC)
	-rm -f *~ core *.ps
	@list='$(WIDGETDIRS)'; for subdir in $$list; do \
	  echo "Making $@ in $$subdir"; \
	  (cd $$subdir && $(MAKE) $@ ) \
	done;
 
realclean:
	-make clean
	-rm -f $(TARGET)

####### Create Qt Designer Derived Files

# Task Information Dialog

taskinfodialog.h: taskinfodialog.ui
	uic -o taskinfodialog.h taskinfodialog.ui

taskinfodialog.cpp: taskinfodialog.h taskinfodialog.ui
	uic -o taskinfodialog.cpp -impl taskinfodialog.h taskinfodialog.ui

# subclass header and implementation (taskinfodialogimpl.h/cpp) were created
# manually after the first run of the above, since we don't want them creating
# again every time we change things!

# So, only run these targets once manually, then edit their creations:
tidsubclass: taskinfodialog.h taskinfodialog.ui
	uic -o taskinfodialogimpl.h -subdecl TaskInfoDialogImpl \
		taskinfodialog.h taskinfodialog.ui
	uic -o taskinfodialogimpl.cpp -subimpl TaskInfoDialogImpl \
		taskinfodialogimpl.h taskinfodialog.ui


# Project Information Dialog

projectinfodialog.h: projectinfodialog.ui
	uic -o projectinfodialog.h projectinfodialog.ui

projectinfodialog.cpp: projectinfodialog.h projectinfodialog.ui
	uic -o projectinfodialog.cpp -impl projectinfodialog.h projectinfodialog.ui

# subclass header and implementation (projectinfodialogimpl.h/cpp) were created
# manually after the first run of the above, since we don't want them creating
# again every time we change things!

# So, only run these targets once manually, then edit their creations:
pidsubclass: projectinfodialog.h projectinfodialog.ui
	uic -o projectinfodialogimpl.h -subdecl ProjectInfoDialogImpl \
		projectinfodialog.h projectinfodialog.ui
	uic -o projectinfodialogimpl.cpp -subimpl ProjectInfoDialogImpl \
		projectinfodialogimpl.h projectinfodialog.ui

# Options Dialog

optionsdialog.h: optionsdialog.ui
	uic -o optionsdialog.h optionsdialog.ui

optionsdialog.cpp: optionsdialog.h optionsdialog.ui
	uic -o optionsdialog.cpp -impl optionsdialog.h optionsdialog.ui

# subclass header and implementation (optionsdialogimpl.h/cpp) were created
# manually after the first run of the above, since we don't want them creating
# again every time we change things!

# So, only run these targets once manually, then edit their creations:
odsubclass: optionsdialog.h optionsdialog.ui
	uic -o optionsdialogimpl.h -subdecl OptionsDialogImpl \
		optionsdialog.h optionsdialog.ui
	uic -o optionsdialogimpl.cpp -subimpl OptionsDialogImpl \
		optionsdialogimpl.h optionsdialog.ui



####### Compile

gantt.o: gantt.cpp \
		gantt.h

main.o: main.cpp \
		mainwidget.h gantt.h project.h task.h

mainwidget.o: mainwidget.cpp \
		mainwidget.h gantt.h project.h task.h ganttview.h \
		calendarview.h taskview.h \
		taskinfodialogimpl.h projectinfodialogimpl.h \
		optionsdialogimpl.h

project.o: project.cpp \
		project.h task.h datehelper.h

task.o: task.cpp \
		task.h project.h datehelper.h

ganttview.o : ganttview.cpp \
		task.h project.h ganttview.h ganttchart.h \
		taskinfodialogimpl.h

ganttchart.o : ganttchart.cpp \
		ganttchart.h

taskview.o : taskview.cpp \
		task.h project.h taskview.h

taskeditor.o : taskeditor.cpp \
		task.h project.h taskeditor.h

calendarview.o : calendarview.cpp \
		project.h calendarview.h

datehelper.o : datehelper.cpp datehelper.h

taskinfodialog.o : taskinfodialog.h calendar/calendar.h

taskinfodialogimpl.o : taskinfodialogimpl.cpp \
		taskinfodialog.h \
		taskinfodialogimpl.h task.h calendar/calendar.h

projectinfodialog.o : projectinfodialog.h calendar/calendar.h

projectinfodialogimpl.o : projectinfodialogimpl.cpp \
		projectinfodialogimpl.h project.h calendar/calendar.h

optionsdialog.o : optionsdialog.h

optionsdialogimpl.o : optionsdialogimpl.cpp \
		optionsdialogimpl.h


moc_gantt.o: moc_gantt.cpp \
		gantt.h

moc_gantt.cpp: gantt.h
	$(MOC) gantt.h -o moc_gantt.cpp


moc_mainwidget.o: moc_mainwidget.cpp \
		mainwidget.h

moc_mainwidget.cpp: mainwidget.h
	$(MOC) mainwidget.h -o moc_mainwidget.cpp


moc_ganttview.o: moc_ganttview.cpp \
		ganttview.h ganttchart.h

moc_ganttview.cpp: ganttview.h
	$(MOC) ganttview.h -o moc_ganttview.cpp


moc_ganttchart.o: moc_ganttchart.cpp \
		ganttchart.h

moc_ganttchart.cpp: ganttchart.h
	$(MOC) ganttchart.h -o moc_ganttchart.cpp


moc_taskview.o: moc_taskview.cpp \
		taskview.h task.h project.h 

moc_taskview.cpp: taskview.h
	$(MOC) taskview.h -o moc_taskview.cpp


moc_taskeditor.o: moc_taskeditor.cpp \
		taskeditor.h task.h project.h 

moc_taskeditor.cpp: taskeditor.h
	$(MOC) taskeditor.h -o moc_taskeditor.cpp


moc_calendarview.o: moc_calendarview.cpp \
		calendarview.h project.h 

moc_calendarview.cpp: calendarview.h
	$(MOC) calendarview.h -o moc_calendarview.cpp


moc_taskinfodialog.o: moc_taskinfodialog.cpp \
		taskinfodialog.h

moc_taskinfodialog.cpp: taskinfodialog.h
	$(MOC) taskinfodialog.h -o moc_taskinfodialog.cpp


moc_taskinfodialogimpl.o: moc_taskinfodialogimpl.cpp \
		taskinfodialogimpl.h

moc_taskinfodialogimpl.cpp: taskinfodialogimpl.h
	$(MOC) taskinfodialogimpl.h -o moc_taskinfodialogimpl.cpp



moc_projectinfodialog.o: moc_projectinfodialog.cpp \
		projectinfodialog.h

moc_projectinfodialog.cpp: projectinfodialog.h
	$(MOC) projectinfodialog.h -o moc_projectinfodialog.cpp


moc_projectinfodialogimpl.o: moc_projectinfodialogimpl.cpp \
		projectinfodialogimpl.h

moc_projectinfodialogimpl.cpp: projectinfodialogimpl.h
	$(MOC) projectinfodialogimpl.h -o moc_projectinfodialogimpl.cpp


moc_optionsdialog.o: moc_optionsdialog.cpp \
		optionsdialog.h

moc_optionsdialog.cpp: optionsdialog.h
	$(MOC) optionsdialog.h -o moc_optionsdialog.cpp


moc_optionsdialogimpl.o: moc_optionsdialogimpl.cpp \
		optionsdialogimpl.h

moc_optionsdialogimpl.cpp: optionsdialogimpl.h
	$(MOC) optionsdialogimpl.h -o moc_optionsdialogimpl.cpp



