#include "Eon.h"
#include "eon_test_main.h"

static void _help(void)
{
	printf("eon_test_image FILE.png\n");
}

Ender_Element * eon_test_image(int argc, char **argv)
{
	Ender_Element *container;
	Ender_Element *image;
	if (argc < 2)
	{
		_help();
		return NULL;
	}

	container = eon_stack_new();
	image = eon_image_new();
	eon_image_file_set(image, argv[1]);
	eon_container_child_add(container, image);

	return container;
}

EXAMPLE(image)

