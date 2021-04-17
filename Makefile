
CXX = g++
CXXFLAGS = -std=c++17 $(PLATFORMSPECIFICFLAGS) -m64 -ggdb3 -ggdb -g3 -g
CXXFLAGS += -Isrc$(S)engine$(S)css -Isrc$(S)engine$(S)lib -Isrc$(S)thirdparty -Idep$(S)JSON/include
LIBS = -lBulletSoftBody -lLinearMath -lBulletCollision -lBulletDynamics
LIBS += -lIrrlicht -lm -lpthread
SHAREDFLAGS = -shared -Wl,-rpath,.

include MakefilePlatformSpecific
include MakefileFiles

compile: .$(S)game$(EXEC_EXT) .$(S)game-core$(SHARED_EXT) .$(S)engine$(SHARED_EXT)

run: compile
	./game$(EXTEXECUTEXEC_EXT)

tools: ObjToShapeConverter$(EXEC_EXT)

ObjToShapeConverter$(EXEC_EXT): src$(S)tools$(S)ObjToShapeConverter.cpp bin$(S)JSON.o
	$(CXX) -o $@ $(CXXFLAGS) $(DIRINCLUDE) $(LIBS) $^



.$(S)game$(EXEC_EXT): bin$(S)Main.o .$(S)engine$(SHARED_EXT)
	$(CXX) -o $@ $(CXXFLAGS) $(LIBS) bin$(S)Main.o .$(S)engine$(SHARED_EXT)
.$(S)engine$(SHARED_EXT): $(ENGOBJ)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(SHAREDFLAGS) $(LIBS)
.$(S)game-core$(SHARED_EXT): $(GAMEOBJ) .$(S)engine$(SHARED_EXT)
	$(CXX) -o $@ $(CXXFLAGS) $(GAMEOBJ) .$(S)engine$(SHARED_EXT) $(SHAREDFLAGS) $(LIBS)



bin$(S)%.o: src$(S)engine$(S)mtd$(S)%.cpp src$(S)engine$(S)css$(S)%.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
bin$(S)%.o: src$(S)engine$(S)lib$(S)dll$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
bin$(S)%.o: src$(S)engine$(S)lib$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
bin$(S)%.o: src$(S)game$(S)mtd$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<



.PHONY: clean
clean:
	$(RM) game$(EXEC_EXT) bin$(S)Main.o
	$(RM) engine$(SHARED_EXT) $(ENGOBJDEL)
	$(RM) game-core$(SHARED_EXT) $(GAMEOBJDEL)

