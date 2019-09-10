
LIBS = -LC:\mingw-w64\lib\bullet -LC:\mingw-w64\lib -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath -lIrrlicht -lm -lpthread -lfreetype
INCLUDEDIRECTORIES = -IC:\mingw-w64\include\bullet -IC:\mingw-w64\include -Iengine\src\css -Iengine\src\lib

CP = copy
RM = del
CXX = g++

RM_FLAGS = /f
CXXFLAGS = -w -m64 -s -std=c++17 -Ofast

GAMECOREOBJECTS = game/bin/Converter.o game/bin/MeshLoader.o game/bin/LoadModules.o game/bin/Main.o game/bin/Init.o
GAMECOREMODULES = game/dlls/Player.dll game/dlls/Event.dll game/dlls/Trigger.dll game/dlls/StaticEntity.dll game/dlls/CharacterWalkTrigger.dll game/dlls/DynamicEntity.dll game/dlls/Character.dll
GAMECOREMODULESOBJECTS = game/bin/Player.o game/bin/Event.o game/bin/Trigger.o game/bin/StaticEntity.o game/bin/CharacterWalkTrigger.o game/bin/DynamicEntity.o game/bin/Character.o
ENGINECOREOBJECTS = engine/bin/BasicWindow.o engine/bin/Camera.o engine/bin/CollisionShapeManager.o engine/bin/CustomCollisionData.o engine/bin/Engine.o engine/bin/EngineRayTraceData.o engine/bin/EventReceiverIrrlicht.o engine/bin/EventResponser.o engine/bin/Model.o engine/bin/Entity.o engine/bin/StringToEnter.o engine/bin/TextPrinter.o engine/bin/TimeCounter.o engine/bin/Window.o engine/bin/World.o engine/bin/CollisionObjectManager.o engine/bin/DllImporter.o engine/bin/ClassFactoryBase.o engine/bin/ModulesFactory.o



install: game/game.exe game/game-core.dll game/engine.dll $(GAMECOREMODULES)
	@$(CP) game\game.exe release\game.exe
	@$(CP) game\game-core.dll release\game-core.dll
	@$(CP) game\engine.dll release\engine.dll
	@$(CP) game\dlls\*.dll release\dlls\*.dll
	@$(CP) game\modules.list release\modules.list


run: game/game.exe game/game-core.dll game/engine.dll $(GAMECOREMODULES)
	make install
	cd release && game.exe

game: game/game.exe game/game-core.dll game/engine.dll $(GAMECOREMODULES)
engine: engine/engine.dll


release/game.exe: game/game.exe
	@$(CP) $< $@

release/game-core.dll: game/game-core.dll
	@$(CP) $< $@

release/engine.dll: engine/engine.dll
	@$(CP) $< $@


game/game.exe: game/engine.dll game/game-core.dll game/bin/Main.o $(GAMECOREMODULES)
	$(CXX) -o $@ $(CXXFLAGS) game/engine.dll game/bin/Main.o $(LIBS)



game/engine.dll: engine/engine.dll
	$(CP) engine\engine.dll game\engine.dll

game/game-core.dll: engine/engine.dll game/dlls/Character.dll $(GAMECOREOBJECTS)
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)

engine/engine.dll: $(ENGINECOREOBJECTS)
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)

game/dlls/StaticEntity.dll: game/engine.dll game/bin/StaticEntity.o
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)

game/dlls/DynamicEntity.dll: game/engine.dll game/bin/DynamicEntity.o
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)

game/dlls/Character.dll: game/engine.dll game/dlls/DynamicEntity.dll game/bin/Character.o
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)

game/dlls/Player.dll: game/engine.dll game/dlls/DynamicEntity.dll game/dlls/Character.dll game/bin/Player.o
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)

game/dlls/Trigger.dll: game/engine.dll game/bin/Trigger.o
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)

game/dlls/CharacterWalkTrigger.dll: game/engine.dll game/dlls/Trigger.dll game/bin/CharacterWalkTrigger.o
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)

game/dlls/Event.dll: game/engine.dll game/dlls/Character.dll game/bin/Event.o
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)



engine/bin/%.o: engine/src/mtd/%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(INCLUDEDIRECTORIES) $<

game/bin/%.o: game/src/mtd/%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(INCLUDEDIRECTORIES) $<

engine/bin/%.o: engine/src/lib/dll/%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(INCLUDEDIRECTORIES) $<


.PHONY: clean
clean:
	@$(RM) $(RM_FLAGS) release\game.exe
	@$(RM) $(RM_FLAGS) release\engine.dll
	@$(RM) $(RM_FLAGS) release\game-core.dll
	@$(RM) $(RM_FLAGS) release\dlls\*.dll
	@$(RM) $(RM_FLAGS) game\bin\*.o
	@$(RM) $(RM_FLAGS) game\game.exe
	@$(RM) $(RM_FLAGS) game\engine.dll
	@$(RM) $(RM_FLAGS) game\game-core.dll
	@$(RM) $(RM_FLAGS) game\dlls\*.dll
	@$(RM) $(RM_FLAGS) engine\bin\*.o
	@$(RM) $(RM_FLAGS) engine\engine.dll
	@$(RM) $(RM_FLAGS) release\modules.list
