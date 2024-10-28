CC = clang
src = lib/glad/src/glad.c src/main.c src/graphics/bo.c src/graphics/vao.c src/util.c src/graphics/shader.c src/graphics/shader_program.c src/graphics/renderer.c src/graphics/window.c src/graphics/camera.c src/math/vector3.c src/graphics/stb_image.c src/graphics/texture.c src/world/block.c src/tilemap.c src/math/rectangle.c
lib = glfw3 GL m cglm
lib_include = lib/glfw/lib lib/cglm/lib
include = lib/glfw/include lib/glad/include lib/cglm/include lib/stb/include src
additional_flags = -Wl,-rpath=./lib/cglm/lib

all:
	@$(CC) $(addprefix -I,$(include)) $(addprefix -L,$(lib_include)) $(src) $(addprefix -l,$(lib)) $(additional_flags) -o build/minecraft
