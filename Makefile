
LIBS = -LC:\mingw-w64\lib\bullet -LC:\mingw-w64\lib -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath -lIrrlicht -lm -lpthread -lfreetype
INCLUDEDIRECTORIES = -IC:\mingw-w64\include\bullet -IC:\mingw-w64\include -Iengine\src\css -Iengine\src\lib

CP = copy
RM = del
CXX = g++

RM_FLAGS = /f
CXXFLAGS = -w -m64 -std=c++17


GAMECOREMODULES = game/dlls/Player.dll
GAMECOREMODULESOBJECTS = game/bin/Player.o
GAMECOREOBJECTS = game/bin/Converter.o game/bin/Event.o game/bin/MeshLoader.o game/bin/Character.o
ENGINECOREOBJECTS = engine/bin/BasicWindow.o engine/bin/Camera.o engine/bin/CollisionShapeManager.o engine/bin/CustomCollisionData.o engine/bin/Engine.o engine/bin/EngineRayTraceData.o engine/bin/EventReceiverIrrlicht.o engine/bin/EventResponser.o engine/bin/Model.o engine/bin/Entity.o engine/bin/StringToEnter.o engine/bin/TextPrinter.o engine/bin/TimeCounter.o engine/bin/Window.o engine/bin/World.o engine/bin/CollisionObjectManager.o engine/bin/DllImporter.o engine/bin/ClassFactoryBase.o engine/bin/ModulesFactory.o



install: game/game.exe game/game-core.dll game/engine.dll $(GAMECOREMODULES)
	$(CP) game\game.exe release\game.exe
	$(CP) game\game-core.dll release\game-core.dll
	$(CP) game\engine.dll release\engine.dll
	$(CP) game\dlls\*.dll release\dlls\*.dll


run: game/game.exe game/game-core.dll game/engine.dll $(GAMECOREMODULES)
	make install
	cd release && game.exe

game: game/game.exe game/game-core.dll
engine: engine/engine.dll


release/game.exe: game/game.exe
	$(CP) game\game.exe release\game.exe

release/game-core.dll: game/game-core.dll
	$(CP) game\game-core.dll release\game-core.dll

release/engine.dll: engine/engine.dll
	$(CP) engine\engine.dll release\engine.dll


game/game.exe: engine/engine.dll game/game-core.dll game/bin/Main.o $(GAMECOREMODULES)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(LIBS)



game/engine.dll: engine/engine.dll
	$(CP) engine\engine.dll game\engine.dll

game/game-core.dll: engine/engine.dll $(GAMECOREOBJECTS)
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)

engine/engine.dll: $(ENGINECOREOBJECTS)
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)

game/dlls/Player.dll: game/engine.dll game/game-core.dll game/bin/Player.o
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)



engine/bin/%.o: engine/src/mtd/%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(INCLUDEDIRECTORIES) $<

game/bin/%.o: game/src/mtd/%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(INCLUDEDIRECTORIES) $<

engine/bin/%.o: engine/src/lib/dll/%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(INCLUDEDIRECTORIES) $<


.PHONY: clean
clean:
	$(RM) $(RM_FLAGS) release\game.exe
	$(RM) $(RM_FLAGS) release\engine.dll
	$(RM) $(RM_FLAGS) release\game-core.dll
	$(RM) $(RM_FLAGS) release\dlls\*.dll
	$(RM) $(RM_FLAGS) game\bin\*.o
	$(RM) $(RM_FLAGS) game\game-core.dll
	$(RM) $(RM_FLAGS) game\dlls\*.dll
	$(RM) $(RM_FLAGS) engine\bin\*.o
	$(RM) $(RM_FLAGS) engine\engine.dll
