#include "Eon.h"
#include "eon_test_main.h"

static void _help(void)
{
	printf("eon_test_svg FILE.svgg\n");
}

Ender_Element * eon_test_svg(int argc, char **argv)
{
	Ender_Element *container;
	Ender_Element *svg;
	if (argc < 2)
	{
		_help();
		return NULL;
	}

	container = eon_stack_new();
	eon_stack_homogeneous_set(container, EINA_TRUE);
	svg = eon_svg_new();
	eon_svg_file_set(svg, argv[1]);
	eon_container_child_add(container, svg);

	return container;
}

EXAMPLE(svg)

