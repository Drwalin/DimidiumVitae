
include Makefile.win

CXX = g++
CC = gcc

CFLAGS = -m64 -ggdb3 -ggdb -g3 -g -Og -pg
CXXFLAGS = $(CFLAGS) -std=c++17

GAMEOBJDIR = game$(S)bin$(S)
MAINOBJECTS = $(GAMEOBJDIR)Character.o $(GAMEOBJDIR)Event.o $(GAMEOBJDIR)GetVersion.o $(GAMEOBJDIR)MeshLoader.o $(GAMEOBJDIR)ModulesLoader.o $(GAMEOBJDIR)Init.o $(GAMEOBJDIR)Player.o $(GAMEOBJDIR)ShapesLoader.o $(GAMEOBJDIR)SoundsLoader.o

GAMECOREMODULESOBJECTS = 
GAMECOREMODULES = 
RELEASEGAMECOREMODULES = $(subst game,release,$(GAMECOREMODULES))

ENGOBJDIR = engine$(S)bin$(S)
ENGINECOREOBJECTS = $(ENGOBJDIR)Camera.o $(ENGOBJDIR)ClassFactoryBase.o $(ENGOBJDIR)CollisionObjectManager.o $(ENGOBJDIR)CollisionShapeConstructor.o $(ENGOBJDIR)CollisionShapeManager.o $(ENGOBJDIR)DllImporter.o $(ENGOBJDIR)DynamicEntity.o $(ENGOBJDIR)Engine.o $(ENGOBJDIR)EngineRayTraceData.o $(ENGOBJDIR)Entity.o $(ENGOBJDIR)EventReceiverIrrlicht.o $(ENGOBJDIR)EventResponser.o $(ENGOBJDIR)GUI.o $(ENGOBJDIR)Model.o $(ENGOBJDIR)ModulesFactory.o $(ENGOBJDIR)MotionController.o $(ENGOBJDIR)MotionControllerTrigger.o $(ENGOBJDIR)Ogg.o $(ENGOBJDIR)SceneNode.o $(ENGOBJDIR)SoundEngine.o $(ENGOBJDIR)StaticEntity.o $(ENGOBJDIR)StlStreamExtension.o $(ENGOBJDIR)StringToEnter.o $(ENGOBJDIR)TimeCounter.o $(ENGOBJDIR)Trigger.o $(ENGOBJDIR)Wav.o $(ENGOBJDIR)World.o $(ENGOBJDIR)Window.o $(ENGOBJDIR)Resource.o $(ENGOBJDIR)ResourceManager.o

SHAREDLIBS = -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath -lIrrlicht -lm -lpthread
RELEASEDEPENDENCIES = $(addprefix release$(S),$(DEPENDENCIES))
LIBS = $(DIRLIBS) $(SHAREDLIBS) $(PLATFORMSPECIFICLIBS)

RELEASEDOCS = release$(S)ChangeLog.txt release$(S)Dependencies.txt release$(S)Readme.txt
RELEASEDEPENDENCYLICENSES = release$(S)dependency-licenses$(S)irrlicht release$(S)dependency-licenses$(S)jpeg release$(S)dependency-licenses$(S)libogg release$(S)dependency-licenses$(S)libpng release$(S)dependency-licenses$(S)libvorbis release$(S)dependency-licenses$(S)irrlicht$(S)readme.txt release$(S)dependency-licenses$(S)jpeg$(S)README release$(S)dependency-licenses$(S)libogg$(S)COPYING release$(S)dependency-licenses$(S)libpng$(S)ANNOUNCE release$(S)dependency-licenses$(S)libpng$(S)AUTHORS release$(S)dependency-licenses$(S)libpng$(S)LICENSE release$(S)dependency-licenses$(S)libpng$(S)README release$(S)dependency-licenses$(S)libpng$(S)TRADEMARK release$(S)dependency-licenses$(S)libvorbis$(S)COPYING



install: release
release: release$(S)game.exe $(RELEASEDOCS) $(RELEASEDEPENDENCYLICENSES)
game: game$(S)game.exe game$(S)game-core.dll game$(S)engine.dll $(GAMECOREMODULES)
engine: engine$(S)engine.dll

run: release$(S)game.exe
	@cd release && game.exe



release$(S)%.txt: .$(S)%.txt
	@$(CP) "$<" "$@"
release$(S)dependency-licenses$(S)%: dependency-licenses$(S)%
	@$(CP) "$<" "$@"



release$(S)game.exe: game$(S)game.exe release$(S)engine.dll release$(S)game-core.dll $(RELEASEDEPENDENCIES) $(RELEASEGAMECOREMODULES) release$(S)modules.list
	@$(CP) game$(S)game.exe release$(S)game.exe
game$(S)game.exe: game$(S)engine.dll game$(S)game-core.dll game$(S)bin$(S)Main.o $(GAMECOREMODULES)
	$(CXX) -o $@ $(CXXFLAGS) game$(S)engine.dll game$(S)bin$(S)Main.o $(LIBS)


engine$(S)engine.dll: $(ENGINECOREOBJECTS) engine$(S)libvorbisogg.dll engine$(S)libzlib.dll
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)
engine$(S)libvorbisogg.dll: dep$(S)libvorbisogg.dll
	@$(CP) "$<" "$@"
engine$(S)libzlib.dll: dep$(S)libzlib.dll
	@$(CP) "$<" "$@"

game$(S)engine.dll: engine$(S)engine.dll
	@$(CP) engine$(S)engine.dll game$(S)engine.dll
game$(S)game-core.dll: engine$(S)engine.dll $(MAINOBJECTS)
	$(CXX) -o $@ -shared -fPIC $(CXXFLAGS) $^ $(LIBS)

game$(S)dlls$(S)Trigger.dll: game$(S)engine.dll game$(S)bin$(S)Trigger.o
	$(CXX) -o $@ -shared -fPIC $(CXXFLAGS) $^ $(LIBS)



release$(S)modules.list: game$(S)modules.list
	@$(CP) "$<" "$@"
release$(S)%.dll: dep$(S)%.dll
	@$(CP) "$<" "$@"
release$(S)engine.dll: engine$(S)engine.dll
	@$(CP) "$<" "$@"
release$(S)dlls$(S)%.dll: game$(S)dlls$(S)%.dll
	@$(CP) "$<" "$@"
release$(S)game-core.dll: game$(S)game-core.dll
	@$(CP) "$<" "$@"



$(ENGOBJDIR)%.o: engine$(S)src$(S)mtd$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
$(ENGOBJDIR)%.o: engine$(S)src$(S)lib$(S)dll$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
$(ENGOBJDIR)%.o: engine$(S)src$(S)lib$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<
$(GAMEOBJDIR)%.o: game$(S)src$(S)mtd$(S)%.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $(DIRINCLUDE) $<



.PHONY: clean
clean:
	@$(RM) release$(S)game.exe
	@$(RM) release$(S)engine.dll
	@$(RM) release$(S)game-core.dll
	@$(RM) release$(S)modules.list
	@$(RM) release$(S)dlls$(S)*.dll
	@$(RM) release$(S)*.dll
	
	@$(RM) $(GAMEOBJDIR)*.o
	@$(RM) game$(S)game.exe
	@$(RM) game$(S)engine.dll
	@$(RM) game$(S)game-core.dll
	@$(RM) game$(S)dlls$(S)*.dll
	@$(RM) game$(S)*.dll
	
	@$(RM) $(ENGOBJDIR)*.o
	@$(RM) engine$(S)engine.dll
