CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -I./

directory=bin
BIN0=bin/a3-model-0
SRCS0=src/earth.cpp src/earth_gl_framework.cpp src/shader_util.cpp src/earth_texture.cpp
INCLUDES0=src/earth_gl_framework.hpp src/shader_util.hpp src/earth.hpp src/earth_texture.hpp
BIN1=bin/a3-model-1
SRCS1=src/launch_site.cpp src/launch_site_gl_framework.cpp src/shader_util.cpp src/launch_site_texture.cpp
INCLUDES1=src/launch_site_gl_framework.hpp src/shader_util.hpp src/launch_site.hpp src/launch_site_texture.hpp
BIN2=bin/a3-model-2
SRCS2=src/rover.cpp src/rover_gl_framework.cpp src/shader_util.cpp src/rover_texture.cpp
INCLUDES2=src/rover_gl_framework.hpp src/shader_util.hpp src/rover.hpp src/rover_texture.hpp
BIN3=bin/a3-scene
SRCS3=src/a3-scene.cpp src/a3-scene_gl_framework.cpp src/shader_util.cpp src/a3-scene_texture.cpp
INCLUDES3=src/a3-scene_gl_framework.hpp src/shader_util.hpp src/a3-scene.hpp src/a3-scene_texture.hpp
all: | $(directory)
	@echo ""
$(directory):
	mkdir -p $@
all: $(BIN0) $(BIN1) $(BIN2) $(BIN3)

$(BIN0): $(SRCS0) $(INCLUDES0)
	g++ $(CPPFLAGS) $(SRCS0) -o $(BIN0) $(LDFLAGS) $(LIBS)
$(BIN1): $(SRCS1) $(INCLUDES1)
	g++ $(CPPFLAGS) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)
$(BIN2): $(SRCS2) $(INCLUDES2)
	g++ $(CPPFLAGS) $(SRCS2) -o $(BIN2) $(LDFLAGS) $(LIBS)
$(BIN3): $(SRCS3) $(INCLUDES3)
	g++ $(CPPFLAGS) $(SRCS3) -o $(BIN3) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN0)
	rm -f *~ *.o $(BIN1)
	rm -f *~ *.o $(BIN2)
	rm -f *~ *.o $(BIN3)
