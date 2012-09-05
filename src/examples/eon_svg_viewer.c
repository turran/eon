#include "Eon.h"
#include "Eon_Svg.h"

typedef struct _Svg_Viewer
{
	/* command line options */
	char *file; /* file to open */
} Svg_Viewer;

static void help(void)
{
	printf("Usage svg_viewer FILE\n");
}

int main(int argc, char **argv)
{
	Svg_Viewer *viewer;
	Eon_Window *ee;
	Ender_Element *win;
	Ender_Element *svg;

	if (argc < 2)
	{
		help();
		return 0;
	}

	eon_init();
	ecore_init();
	escen_init();

	svg = eon_svg_new();
	
	/* do the setup */
	viewer = calloc(1, sizeof(Svg_Viewer));
	/* create the main window */
	win = eon_stack_new();
	eon_stack_orientation_set(win, EON_ORIENTATION_HORIZONTAL);
	ee = eon_backend_window_new(eon_ecore_sdl_new(), win, 640, 480);
	if (!ee)
	{
		printf("Can not create the Eon_Window window\n");
		return 1;
	}
	eon_svg_file_set(svg, argv[1]);
	eon_container_child_add(win, svg);

	ecore_main_loop_begin();

	escen_shutdown();
	eon_shutdown();
	ecore_shutdown();

	return 0;
}
