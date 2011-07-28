#include "Eon.h"

static void _help(void)
{
	printf("eon_test_image FILE.png\n");
}

int main(int argc, char **argv)
{
	Eon_Window *win;
	Eon_Backend *backend;
	Ender_Element *layout;
	Ender_Element *image;

	if (argc < 2)
	{
		_help();
		return 1;
	}

	eon_init();
	ecore_init();

	backend = eon_ecore_sdl_new();
	//backend = eon_ecore_remote_new();

	layout = eon_stack_new();
	eon_element_width_set(layout, 320.0);
	eon_element_height_set(layout, 240.0);

	win = eon_window_new(backend, layout, 320, 240);
	eon_stack_direction_set(layout, EON_STACK_DIRECTION_VERTICAL);
	image = eon_image_new();
	eon_image_file_set(image, argv[1]);
	eon_layout_child_add(layout, image);

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}

