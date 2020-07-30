
This file is part of The Drwalin Game project - Dimidium Vitae
Copyright (C) 2018-2019 Marek Zalewski aka Drwalin aka DrwalinPCF

============================================================================
Dimidium Vitae v0.3.3
============================================================================

  Welcome to the Dimidium Vitae game Readme.txt.

  Content of this file:

  1. How To Run The Game
  2. Controls
  3. Requirements
  4. Directory Structure Overview
  5. How To Compile
  6. Release Notes

============================================================================
1. How To Run Game
============================================================================

  All you need is to install OpenAL:
	https://www.openal.org/downloads/

  On windows:
    Type in console:
      game 
    or double-click game.exe executable
  On llinux:
    Type in console:
      ./game

============================================================================
3. Controls
============================================================================

  Movement:       WASD
  Rotate camera:  move mouse, arrows
  Jump:           SPACE
  Crouch:         CTRL
  Stroll:         ALT
  Run:            SHIFT
  Throw box:      LMB - Left Mouse Button
  Throw ball:     RMB - Right Mouse Button
  Exit game:      ESC
  Unlock cursor:  P
  Destro Entity   DELETE
  Animation example:                   1,2,3,4 - afects bow
  Center view at coords (0;0;0):       MMB - Middle Mouse Button (press wheel)
  Print number of objects in console:  T

============================================================================
3. Requirements
============================================================================

  Currently compiled only on windows 7 64bit and arch linux
  Processor, Graphics card, memory and disk space requirements are not
  specified yet.

============================================================================
4. Directory Structure Overview
============================================================================

  /dep         Contain dependencies (list in: /Dependencies.txt).
  /dependency-licenses  Contain all copying/license/readme/... of all
               dependencies.
  /src         Source code
  /src/engine  Engine source code
  /src/game    Game source code
  /media       All the media for game (fonts,textures,models,maps,
               animations,materials,sounds,etc.)
  /            All executables and dlls

============================================================================
5. How To Compile
============================================================================

  On Windows with mingw:
    #1.:
	    Compile Irrlicht (http://irrlicht.sourceforge.net/), then copy
		Irrlicht.dll into /dep directory
	#2.:
	    Compile Bullet Physics (https://github.com/bulletphysics/bullet3)
		then copy libBulletBynamics.dll, libBulletCollision.dll and
		libLinearMath.dll into /dep directory
	#3.:
		Install OpenAL (https://www.openal.org/downloads/), then copy dll
		from installation directory into /dep directory
    #4.:
	    Compile zlib (https://zlib.net/) then copy libzlib.dll into /dep
		directory
	#5.:
		Cmpile OggVorbis with my Makefile
		(https://github.com/DrwalinPCF/libogg-libvorbis-compiler) then copy
		created libvorbisogg.dll into /dep directory
	#6.:
		Copy specified *.dll's (in /Dependencies.txt file) from your mingw
		installation directory or from C:/Windows/System32
    #7.:
	    Edit Makefile and change modify second line to:
		  include Makefile.win
	    Type in console:
	      make

  On Linux:
    #1.:
        Install all dependecies listed in /Dependencies.txt
    #2.:
	    Edit Makefile and change modify second line to:
		  include Makefile.lin
	    Type in console:
	      make

============================================================================
6. Release Notes
============================================================================
  
  Informations about changes in this new version (and all previous versions)
  can be found in /ChangeLog.txt.
  