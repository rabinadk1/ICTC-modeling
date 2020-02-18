# Declaring the variables
CC = g++
CFLAGS = -Wall -Weffc++
HEADERS = Shader.hpp Renderer.hpp Model.hpp

LINKALL = -lGLEW -lglfw -lGL -lX11 -ldl -lassimp
LINKTARGET = main
OBJS =  IndexBuffer.o VertexBuffer.o \
VertexArray.o Shader.o \
Renderer.o Texture.o \
Camera.o Mesh.o Model.o \
stb_image.o main.o

all: $(LINKTARGET)

# The target on first compiles on default
$(LINKTARGET): $(OBJS)
	$(CC) -Og -ggdb -o $@ $^ $(LINKALL)

# % accepts all filename
%.o: %.cpp
	$(CC) -Og -ggdb $(CFLAGS) -c $<

# Make header files of classes as their dependencies
VertexArray.o: VertexArray.hpp VertexBuffer.hpp VertexBufferLayout.hpp
IndexBuffer.o: IndexBuffer.hpp
VertexBuffer.o: VertexBuffer.hpp
Shader.o: Shader.hpp
Renderer.o: Renderer.hpp
Texture.o: Texture.hpp
Camera.o: Camera.hpp
Mesh.o: Mesh.hpp
Model.o: Model.hpp
stb_image.o: stb_image.h

main.o: $(HEADERS)

clean:
	echo "Cleaning .o files and executable"
	rm -v $(OBJS) $(LINKTARGET)
