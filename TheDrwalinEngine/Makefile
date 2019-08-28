
include ../config.mk

CP = copy
RM = del
CXX = g++

RM_FLAGS = /F

ENGINECOREFILES = bin/BasicWindow.o bin/Camera.o bin/CollisionShapeManager.o bin/CustomCollisionData.o bin/Engine.o bin/EngineRayTraceData.o bin/EventReceiverIrrlicht.o bin/EventResponser.o bin/Model.o bin/Object.o bin/StringToEnter.o bin/TextPrinter.o bin/TimeCounter.o bin/Trigger.o bin/Window.o bin/World.o


install: engine.dll
	$(CP) engine.dll ..\release\engine.dll


engine.dll: $(ENGINECOREFILES)
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)


bin/%.o: src/mtd/%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(INCLUDEDIRECTORIES) $<


.PHONY : clean
clean:
	$(RM) $(RM_FLAGS) bin\*.o
	$(RM) $(RM_FLAGS) engine.o
