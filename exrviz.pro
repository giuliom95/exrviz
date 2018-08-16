TARGET = bin/exrviz
HEADERS +=  src/oglWidget.hpp
SOURCES +=  src/main.cpp        \
            src/oglWidget.cpp
QT += widgets
INCLUDEPATH += /usr/include/OpenEXR
LIBS += -lIlmImf -lHalf