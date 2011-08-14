#include "Eon.h"

static char *first_file;
static char *second_file;

static void _help(void)
{
	printf("eon_test_button_image FILE.png [FILE.png]\n");
}

static void _button_clicked(Ender_Element *e, const char *event_name, void *event_data, void *data)
{
	Ender_Element *image = data;
	const char *file;

	printf("button clicked\n");
	eon_image_file_get(image, &file);
	printf("file = %s %s %s\n", file, first_file, second_file);
	if (!strcmp(file, first_file))
		eon_image_file_set(image, second_file);
	else
		eon_image_file_set(image, first_file);
	printf("ok\n");
}

static void _add_image(Ender_Element *button, const char *file)
{
	Ender_Element *image;
	double w, h;

	image = eon_image_new();
	eon_image_file_set(image, file);
	eon_container_content_set(button, image);
	eon_element_preferred_width_get(image, &w);
	eon_element_preferred_width_get(image, &h);
	eon_element_min_width_set(image, w);
	eon_element_min_height_set(image, 20);
	ender_event_listener_add(button, "MouseClick", _button_clicked, image);
}


int main(int argc, char **argv)
{
	Eon_Window *win;
	Ender_Element *layout;
	Eon_Backend *backend;
	Eina_Bool with_image = EINA_FALSE;
	int i;

	eon_init();
	ecore_init();

	if (argc < 2)
	{
		_help();
		return 1;
	}
	first_file = argv[1];

	if (argc > 2)
		second_file = argv[2];
	else
		second_file = first_file;

	backend = eon_ecore_sdl_new();

	layout = eon_stack_new();
	eon_element_width_set(layout, 320.0);
	eon_element_height_set(layout, 240.0);

	eon_stack_direction_set(layout, EON_STACK_DIRECTION_VERTICAL);
	win = eon_window_new(backend, layout, 320, 240);

	for (i = 0; i < 5; i++)
	{
		Ender_Element *button;

		button = eon_button_new();
		_add_image(button, argv[1]);
		eon_layout_child_add(layout, button);
	}

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();

	return 0;
}

