#include "Eon.h"

#include "Eon.h"
#include "eon_test_main.h"

#if 0
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
#endif

static void _help(void)
{
	printf("eon_test_button_image FILE.png [FILE2.png FILE3.png ...]\n");
}

static void _add_image(Ender_Element *button, const char *file)
{
	Ender_Element *image;
	double w, h;

	image = eon_image_new();
	eon_image_file_set(image, file);
	eon_container_child_add(button, image);
#if 0
	eon_element_preferred_width_get(image, &w);
	eon_element_preferred_width_get(image, &h);
	eon_element_min_width_set(image, w);
	eon_element_min_height_set(image, 20);
	ender_event_listener_add(button, "MouseClick", _button_clicked, image);
#endif
}

Ender_Element * eon_test_button_image(int argc, char **argv)
{
	Ender_Element *container;
	int i = 1;

	if (argc < 2)
	{
		_help();
		return NULL;
	}

	container = eon_stack_new();
	eon_stack_orientation_set(container, EON_ORIENTATION_VERTICAL);
	eon_stack_homogeneous_set(container, EINA_TRUE);
	while (argc > 1)
	{
		Ender_Element *button;
		Ender_Element *image;
		
		button = eon_button_new();
		_add_image(button, argv[i]);
		eon_container_child_add(container, button);
		i++;
		argc--;
	}

	return container;
}

EXAMPLE(button_image)

