#ifndef _EON_TEST_MAIN_H
#define _EON_TEST_MAIN_H

#define EXAMPLE(name)								\
	Ender_Element * eon_test_##name(int argc, char **argv);			\
	int main(int argc, char **argv)						\
	{									\
		Ender_Element *container;					\
		Eon_Backend *backend;						\
		Eon_Window *win;						\
		eon_init();							\
										\
		/* TODO make this using the args */				\
		if (argc > 2)							\
		{								\
			if (!strcmp(argv[1], "-e"))				\
			{							\
				if (!strcmp(argv[2], "remote"))			\
					backend = eon_ecore_remote_new();	\
				else if (!strcmp(argv[2], "sdl"))		\
					backend = eon_ecore_sdl_new();		\
			}							\
		}								\
		if (!backend)							\
			backend = eon_ecore_sdl_new();				\
		container = eon_test_##name(argc, argv);			\
		if (!container) return -1;					\
		win = eon_backend_window_new(backend,				\
				container, 320, 240);				\
		eon_backend_run(backend);					\
		eon_backend_delete(backend);					\
		eon_shutdown();							\
										\
		return 0;							\
	}

#endif
