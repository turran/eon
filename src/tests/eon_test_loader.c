#include "Eon.h"
#include "Escen.h"

static Eon_Backend * _remote_new(void)
{
	Eon_Backend *backend;
	backend = eon_ecore_remote_new();
	return backend;
}

static Eon_Backend * _sdl_new(void)
{
	Eon_Backend *backend;
	backend = eon_ecore_sdl_new();
	return backend;
}

static void _help(void)
{
	printf("eon_loader FILE\n");
}

int main(int argc, char **argv)
{
	Escen *e;
	Escen_Ender *ee;
	Escen_Instance *eei;
	Ender_Element *ender;
	Ender_Element *layout;
	Eon_Window *win;
	Eon_Backend *backend;
	char *file;

	if (argc < 2)
	{
		_help();
		return 1;
	}

	file = argv[1];

	eon_init();
	ecore_init();
	escen_init();
	ender_loader_load_all();

	e = escen_parser_load(file);
	if (!e)
	{
		printf("Impossible to parse the file %s\n", file);
		return 1;
	}
	ee = escen_ender_nth_get(e, 0);
	if (!ee)
	{
		printf("No escen ender available\n");
		return 1;
	}
	eei = escen_instance_new(ee);
	ender = escen_instance_ender_get(eei);

	backend = _sdl_new();
	win = eon_window_new(backend, ender, 320, 240);

	ecore_main_loop_begin();
	ecore_shutdown();
	eon_shutdown();
	escen_shutdown();

	return 0;
}
