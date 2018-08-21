TARGET = bin/exrviz
HEADERS +=  src/mainwin.hpp     \
            src/oglWidget.hpp
SOURCES +=  src/main.cpp        \
            src/mainwin.cpp     \
            src/oglWidget.cpp
QT += widgets
INCLUDEPATH += /usr/include/OpenEXR
LIBS += -lIlmImf -lHalf