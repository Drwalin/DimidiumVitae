
CXX = g++
CXXFLAGS = -std=c++17 $(PLATFORMSPECIFICFLAGS) -m64 -ggdb3 -ggdb -g3 -g -Wall -pedantic
CXXFLAGS += -Isrc -Idep$(S)JSON$(S)include -Idep$(S)DllLoader$(S)include
CXXFLAGS += -Idep$(S)concurrentqueue
LIBS = -lBulletSoftBody -lLinearMath -lBulletCollision -lBulletDynamics
LIBS += -lIrrlicht -lm -lpthread
SHAREDFLAGS = -shared -Wl,-rpath,.

include MakefilePlatformSpecific
include MakefileFiles

compile: .$(S)game$(EXEC_EXT) .$(S)game-core$(SHARED_EXT) .$(S)engine$(SHARED_EXT) .$(S)scripts$(SHARED_EXT)

run: compile
	./game$(EXTEXECUTEXEC_EXT)

tools: ObjToShapeConverter$(EXEC_EXT)

ObjToShapeConverter$(EXEC_EXT): src$(S)tools$(S)ObjToShapeConverter.cpp bin$(S)JSON.o
	$(CXX) -o $@ $(CXXFLAGS) $(DIRINCLUDE) $(LIBS) $^



.$(S)game$(EXEC_EXT): bin$(S)Main.o .$(S)engine$(SHARED_EXT) .$(S)scripts$(SHARED_EXT)
	$(CXX) -o $@ $(CXXFLAGS) $(LIBS) bin$(S)Main.o .$(S)engine$(SHARED_EXT)
.$(S)engine$(SHARED_EXT): $(ENGOBJ)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(SHAREDFLAGS) $(LIBS)
.$(S)game-core$(SHARED_EXT): $(GAMEOBJ) .$(S)engine$(SHARED_EXT)
	$(CXX) -o $@ $(CXXFLAGS) $(GAMEOBJ) .$(S)engine$(SHARED_EXT) $(SHAREDFLAGS) $(LIBS)
.$(S)scripts$(SHARED_EXT): $(SCRIPTSOBJ) .$(S)engine$(SHARED_EXT)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(SHAREDFLAGS) $(LIBS)



bin$(S)%.o: src$(S)engine$(S)%.cpp src$(S)engine$(S)%.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
bin$(S)%.o: src$(S)entities$(S)%.cpp src$(S)entities$(S)%.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
bin$(S)%.o: src$(S)gui$(S)%.cpp src$(S)gui$(S)%.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
bin$(S)%.o: src$(S)resources$(S)%.cpp src$(S)resources$(S)%.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
bin$(S)%.o: src$(S)util$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
bin$(S)%.o: src$(S)game$(S)%.cpp src$(S)game$(S)%.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
bin$(S)%.o: src$(S)scripts$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
bin$(S)%.o: src$(S)game$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<



bin$(S)JSON.o: dep$(S)JSON$(S)src$(S)JSON.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<

bin$(S)%.o: dep$(S)DllLoader$(S)src$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<

.PHONY: clean
clean:
	$(RM) game$(EXEC_EXT) bin$(S)Main.o
	$(RM) engine$(SHARED_EXT) $(ENGOBJDEL)
	$(RM) game-core$(SHARED_EXT) $(GAMEOBJDEL)
	$(RM) scripts$(SHARED_EXT) $(SCRIPTSDEL)

