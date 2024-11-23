CC = clang

src = lib/glad/src/glad.c lib/noise/noise1234.c src/main.c src/graphics/bo.c src/graphics/vao.c src/util.c src/graphics/shader.c src/graphics/shader_program.c src/graphics/renderer.c src/graphics/window.c src/graphics/camera.c src/math/vector3/vector3f.c src/math/vector3/vector3d.c src/math/vector3/vector3i.c src/math/vector2/vector2f.c src/math/vector2/vector2d.c src/math/vector2/vector2i.c src/graphics/stb_image.c src/graphics/texture.c src/tilemap.c src/math/rectangle.c src/input/keyboard.c src/input/mouse.c src/world/chunk.c src/player.c src/math/math_util.c src/world/world.c src/world/block.c src/gui/gui.c src/gui/gui_image.c src/math/matrix4/matrix4d.c
lib = glfw3 GL m cglm pthread
lib_include = lib/glfw/lib lib/cglm/lib
include = lib/glfw/include lib/glad/include lib/cglm/include lib/stb/include lib/noise
additional_flags = -Wl,-rpath=./lib/cglm/lib

all:
	@$(CC) $(addprefix -I,$(include)) $(addprefix -L,$(lib_include)) $(src) $(addprefix -l,$(lib)) $(additional_flags) -o build/minecraft
