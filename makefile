# OpenGL
libs-ogl=-lglfw3
frameworks-ogl= -framework Cocoa -framework OpenGL -framework IOKit

# Windows - OpenGL
libs-ogl-win=-lglfw3 -lopengl32 -lgdi32 -luser32

# Metal
# libs-mtl=
frameworks-mtl= -framework Foundation -framework QuartzCore -framework Metal -framework MetalKit -framework CoreGraphics

# Common
bin= brickd

build-ogl: compile-ogl compile-common link-ogl clean

build-owin: compile-ogl compile-common link-owin clean

build-mtl: compile-mtl compile-common link-mtl clean
	
compile-ogl:
	gcc -c src/ogl/gl.c -o obj/gl.o -I include/ogl
	g++ -std=c++17 -c -DOPENGL src/common/framework.cpp -o obj/framework.o -I include/ogl
	g++ -std=c++17 -c src/ogl/base.cpp -o obj/base.o -I include/ogl

compile-mtl:
	g++ -std=c++17 -c -DMETAL src/common/framework.cpp -o obj/framework.o -I include/mtl
	g++ -std=c++17 -c src/mtl/base.cpp -o obj/base.o -I include/mtl

compile-common:
	g++ -std=c++17 -c src/common/physics.cpp -o obj/physics.o
	g++ -std=c++17 -c src/main.cpp -o obj/main.o
	g++ -std=c++17 -c src/game.cpp -o obj/game.o
	g++ -std=c++17 -c src/core/gameobject.cpp -o obj/gameobject.o
	g++ -std=c++17 -c src/core/ball.cpp -o obj/ball.o
	g++ -std=c++17 -c src/core/paddle.cpp -o obj/paddle.o
	g++ -std=c++17 -c src/core/brick.cpp -o obj/brick.o

link-ogl:
	g++ obj/* -L lib $(libs-ogl) $(frameworks-ogl) -o bin/opengl/$(bin)

link-owin:
	g++ obj/* -L lib $(libs-ogl-win) -o bin/opengl/$(bin).exe

link-mtl:
	g++ obj/* $(frameworks-mtl) -o bin/metal/$(bin)

clean:
	rm obj/*

run-opengl:
	bin/opengl/$(bin)

package-opengl:
	cp bin/opengl/$(bin) Brickd.app/Contents/MacOS
	cp bundle/Info.plist Brickd.app/Contents
	./bundle/icons/make_icon.sh
	cp bundle/icons/AppIcon.icns Brickd.app/Contents/Resources/AppIcon.icns
	cp -R src/assets Brickd.app/Contents/Resources/src

run-metal:
	bin/metal/$(bin)