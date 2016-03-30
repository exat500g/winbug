#-------------------------------------------------
#
# Project created by QtCreator 2013-05-20T17:35:23
#
#-------------------------------------------------

QT       += widgets xml gui

CONFIG(debug, debug|release) {
TARGET = ObjectInspectord
} else {
TARGET = ObjectInspector
}

TEMPLATE = lib

DESTDIR = ../

INCLUDEPATH += \
    dialogs \
    model \
    resources \
    widgets \
    commands

INCLUDEPATH += ../

DEFINES += OBJECTINSPECTOR_LIBRARY

SOURCES +=\
    dialogs/textinputdialog.cpp \
    model/cflagsmodel.cpp \
    widgets/qpropertyeditwidget.cpp \
    widgets/qcolorwidget.cpp \
    model/treenode.cpp \
    model/propertytreenode.cpp \
    model/sizepropertynode.cpp \
    model/cobjectinspectortreemodel.cpp \
    model/enumproperty.cpp \
    model/cobjectinspectortreedelegate.cpp \
    model/flagpropertynode.cpp \
    model/floatsizepropertynode.cpp \
    widgets/qfontlabel.cpp \
    commands/cpropertycommand.cpp \
    commands/cobjectcommand.cpp \
    commands/cpropertyresetcommand.cpp \
    initres.cpp \
    model/transformpropertynode.cpp \
    widgets/qlongspinbox.cpp \
    widgets/qulongspinbox.cpp \
    model/BoundRectPropertyNode.cpp \
    model/PlannerPosPropertyNode.cpp \
    model/QVariantListPropertyNode.cpp

HEADERS +=\
        objectinspector_global.h \
    dialogs/textinputdialog.h \
    model/cflagsmodel.h \
    widgets/qpropertyeditwidget.h \
    widgets/qcolorwidget.h \
    model/treenode.h \
    model/propertytreenode.h \
    model/sizepropertynode.h \
    model/cobjectinspectortreemodel.h \
    model/enumproperty.h \
    model/cobjectinspectortreedelegate.h \
    model/flagpropertynode.h \
    model/floatsizepropertynode.h \
    widgets/qfontlabel.h \
    commands/cpropertycommand.h \
    commands/cobjectcommand.h \
    commands/cpropertyresetcommand.h \
    model/transformpropertynode.h \
    widgets/qlongspinbox.h \
    widgets/qulongspinbox.h \
    model/BoundRectPropertyNode.h \
    model/PlannerPosPropertyNode.h \
    model/QVariantListPropertyNode.h


FORMS += \
    dialogs/textinputdialog.ui

RESOURCES += objinspector.qrc
