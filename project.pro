# Nome do projeto
TEMPLATE = app
TARGET = main
CONFIG += console c++11
CONFIG -= app_bundle

# Diretórios de inclusão
INCLUDEPATH += /usr/local/include/opencv4
INCLUDEPATH += /usr/include/opencv4

# Diretórios de bibliotecas
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

# Arquivos de origem
SOURCES += main.cpp
SOURCES += ImageManager.cpp
SOURCES += ImageEditing.cpp
SOURCES += ConvKernels.cpp

# Arquivos de cabeçalho
HEADERS += ImageManager.h
HEADERS += ImageEditing.h
HEADERS += ConvKernels.h

# Definir o sistema operacional, se necessário
unix:!macx {
    LIBS += -lpthread
}

# Qt Módulos necessários
QT += core gui widgets
