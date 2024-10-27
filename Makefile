CC = clang
src = lib/glad/src/glad.c src/main.c src/bo.c src/vao.c src/util.c src/shader.c src/shader_program.c src/renderer.c src/window.c src/camera.c src/vector3.c src/stb_image.c src/texture.c
lib = glfw3 GL m cglm
lib_include = lib/glfw/lib lib/cglm/lib
include = lib/glfw/include lib/glad/include lib/cglm/include lib/stb/include
additional_flags = -Wl,-rpath=./lib/cglm/lib

all:
	$(CC) $(addprefix -I,$(include)) $(addprefix -L,$(lib_include)) $(src) $(addprefix -l,$(lib)) $(additional_flags) -o build/minecraft
