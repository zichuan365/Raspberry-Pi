TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    histSeg.cpp \
    coreAlgorithm.cpp \
    tranferProcess.cpp

HEADERS += \
    histSeg.h \
    coreAlgorithm.h \
    tranferProcess.h \
    unit-test.h


#fopenmp
QMAKE_CFLAGS+= -fopenmp
LIBS+= -fopenmp
#pthread
LIBS += -lpthread
#LIBS+= /usr/lib/arm-linux-gnueabihf/libpthread.so
#Raspicam
INCLUDEPATH += \
/usr/local/include/raspicam
LIBS +=/usr/local/lib/libraspicam.so
LIBS +=/usr/local/lib/libraspicam_cv.so
#Opencv
INCLUDEPATH += \
/usr/local/include \
/usr/local/include/opencv \
/usr/local/include/opencv2
LIBS +=/usr/local/lib/libopencv_*.so
#modbus
INCLUDEPATH+=/home/pi/Libs/modbus/install/include/modbus/
LIBS+=/home/pi/Libs/modbus/install/lib/libmodbus.so
HEADERS += \
    unit-test.h

