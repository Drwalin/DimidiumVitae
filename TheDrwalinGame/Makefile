
include ../config.mk

GAMECOREFILES = bin/Character.o bin/CharacterEvent.o bin/CharacterWalkTrigger.o bin/Player.o bin/Converter.o bin/Event.o bin/MeshLoader.o
GAMEEXE = game.exe


$(GAMEEXE): engine.dll game-core.dll bin/Main.o
	$(CXX) -o $(GAMEEXE) $^ $(LIBS)


install: engine.dll game-core.dll $(GAMEEXE)
	$(CP) game-core.dll ..\release\game-core.dll
	$(CP) $(GAMEEXE) ..\release\$(GAMEEXE)
	cd $(ENGINEDIR) && make install

game-core.dll: engine.dll $(GAMECOREFILES)
	$(CXX) -shared -fPIC -o $@ $(CXXFLAGS) $^ $(LIBS)


engine.dll:
	cd $(ENGINEDIR) && make
	$(CP) $(ENGINEDIR)\engine.dll engine.dll


bin/%.o: src/mtd/%.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(INCLUDEDIRECTORIES) $<


.PHONY : clean
clean:
	$(RM) $(RM_FLAGS) bin\*.o
	$(RM) $(RM_FLAGS) $(GAMEEXE)
	$(RM) $(RM_FLAGS) engine.dll
	$(RM) $(RM_FLAGS) game-core.dll
