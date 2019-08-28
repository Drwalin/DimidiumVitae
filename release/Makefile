
include ../config.mk

build:
	cd $(GAMEDIR) && make && make install

.PHONY: clean
clean:
	cd $(GAMEDIR) && make clean
	cd $(ENGINEDIR) && make clean
	$(RM) $(RM_FLAGS) *.exe
	$(RM) $(RM_FLAGS) dlls\*.dll
	$(RM) $(RM_FLAGS) engine.dll
	$(RM) $(RM_FLAGS) game-core.dll

