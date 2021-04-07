
include Makefile.win

CXX = g++
CC = gcc

CFLAGS = $(PLATFORMSPECIFICFLAGS) -m64 -ggdb3 -ggdb -g3 -g -Og
CXXFLAGS = $(CFLAGS) -std=c++17

GAMEOBJ_ = Character.o Event.o GetVersion.o Init.o MainMenu.o LoadingScreen.o Player.o
GAMEOBJ = $(addprefix .$(S)bin$(S),$(GAMEOBJ_))

ENGOBJ_ = Animation.o Camera.o CollisionObjectManager.o CollisionShape.o DllImporter.o DynamicEntity.o Engine.o Entity.o EventReceiverIrrlicht.o EventResponser.o FileSystem.o GUI.o JSON.o Material.o Menu.o Model.o ModulesFactory.o MotionController.o MotionControllerTrigger.o Ogg.o PrimitiveCollisionShape.o Resource.o ResourceManager.o SceneNode.o Singleton.o Sound.o SoundEngine.o SoundSource.o StaticEntity.o StlStreamExtension.o StringToEnter.o Texture.o TimeCounter.o Trigger.o Wav.o Window.o World.o
ENGOBJ = $(addprefix .$(S)bin$(S),$(ENGOBJ_))
	
SHAREDLIBS = -lBulletSoftBody -lLinearMath -lBulletCollision -lBulletDynamics -lIrrlicht -lm -lpthread
LIBS = $(DIRLIBS) $(SHAREDLIBS) $(PLATFORMSPECIFICLIBS) $(DEPENDENCIES)

DIRINCLUDE = $(DIRINCLUDEPC) -Isrc/engine/css -Isrc/engine/lib -Isrc/thirdparty

compile: .$(S)game$(EXTEXECUTABLE) .$(S)engine$(EXTSHARED) .$(S)game-core$(EXTSHARED)

run: compile
	.$(S)game$(EXTEXECUTABLE)

tools: .$(S)ObjToShapeConverter$(EXTEXECUTABLE)

.$(S)ObjToShapeConverter$(EXTEXECUTABLE): .$(S)src$(S)tools$(S)ObjToShapeConverter.cpp .$(S)bin$(S)JSON.o
	$(CXX) -o $@ $(CXXFLAGS) $(DIRINCLUDE) $(LIBS) $^



.$(S)game$(EXTEXECUTABLE): .$(S)engine$(EXTSHARED) .$(S)bin$(S)Main.o
	$(CXX) -o $@ $(CXXFLAGS) $(LIBS) .$(S)engine$(EXTSHARED) .$(S)bin$(S)Main.o
.$(S)engine$(EXTSHARED): $(ENGOBJ)
	$(CXX) -shared -o $@ $(CXXFLAGS) $^ $(LIBS)
.$(S)game-core$(EXTSHARED): .$(S)engine$(EXTSHARED) $(GAMEOBJ)
	$(CXX) -shared -o $@ $(CXXFLAGS) .$(S)engine$(EXTSHARED) $(GAMEOBJ) $(LIBS)



.$(S)bin$(S)%.o: .$(S)src$(S)engine$(S)mtd$(S)%.cpp .$(S)src$(S)engine$(S)css$(S)%.h
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
.$(S)bin$(S)%.o: .$(S)src$(S)engine$(S)lib$(S)dll$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
.$(S)bin$(S)%.o: .$(S)src$(S)engine$(S)lib$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
.$(S)bin$(S)%.o: .$(S)src$(S)game$(S)mtd$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<



.PHONY: clean
clean:
	$(RM) .$(S)game$(EXTEXECUTABLE)
	$(RM) .$(S)engine$(EXTSHARED) .$(ENGOBJ)
	$(RM) .$(S)game-core$(EXTSHARED) $(GAMEOBJ)
