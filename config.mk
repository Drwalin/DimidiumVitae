
ENGINEDIR = ..\TheDrwalinEngine
GAMEDIR = ..\TheDrwalinGame
MEDIADIR = ..\TheDrwalinGameMedia
RELEASEDIR = ..\release

LIBS = -LC:\mingw-w64\lib\bullet -LC:\mingw-w64\lib -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath -lIrrlicht -lm -lpthread -lfreetype
INCLUDEDIRECTORIES = -IC:\mingw-w64\include\bullet -IC:\mingw-w64\include -I$(ENGINEDIR)\src\css -I$(ENGINEDIR)\src\lib

CP = copy
RM = del
CXX = g++

RM_FLAGS = /f

CXXFLAGS = -Ofast -s -w -m64 -std=c++17
