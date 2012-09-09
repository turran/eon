#include "Escen.h"
#include "Ender.h"
#include "Eon.h"

typedef struct _Escen_Viewer
{
	/* command line options */
	int width;
	int height;
	char *file; /* file to open */

	/* backend related */
	Eon_Window *win;
	Eon_Backend *backend;

	Escen *escen; /* escen loaded */
	Ender_Element *container; /* the main container */
	Ender_Element *enders_stack;
	Ender_Element *states_stack;
	/* for the area where we should render the escen */
	Ender_Element *scene_area; /* where the escen loaded should be drawn */
	Ender_Element *scene_area_scrollview;
	Enesim_Surface *s;
	Escen_Ender *current_escen_ender; /* current escen ender */
	Escen_Instance *current_instance; /* current instance displaying */
	Ender_Element *current_ender;
	Enesim_Renderer *current_renderer;

	/* our own scene background */
	Enesim_Renderer *checker;
	Enesim_Renderer *compound;
} Escen_Viewer;

typedef struct _Escen_Viewer_Damages
{
	Escen_Viewer *thiz;
	Eina_List *areas;
} Escen_Viewer_Damages;

static void help(void)
{
	printf("Usage escen_viewer [ENGINE] WIDTH HEIGHT ESCEN_FILE\n");
}

static Eina_Bool _damages_cb(Enesim_Renderer *r, const Eina_Rectangle *area, Eina_Bool past, void *data)
{
	Escen_Viewer_Damages *damages = data;
	Eina_Rectangle *a;

	a = malloc(sizeof(Eina_Rectangle));
	*a = *area;

	damages->areas = eina_list_append(damages->areas, a);
	eon_surface_damage_add(damages->thiz->scene_area, a);

	return EINA_TRUE;
}

static Eina_Bool idler(void *data)
{
	Escen_Viewer *thiz = data;
	Escen_Viewer_Damages damages;
	Eina_Rectangle *a;

	damages.thiz = thiz;
	damages.areas = NULL;
	/* get the damages */
	enesim_renderer_damages_get(thiz->compound, _damages_cb, &damages);
	if (!damages.areas) goto done;

	/* draw it */
	enesim_renderer_draw_list(thiz->compound, thiz->s, damages.areas, 0, 0, NULL);
	EINA_LIST_FREE (damages.areas, a)
		free(a);
done:
	return EINA_TRUE;
}

static void state_clicked(Ender_Element *e, const char *event_name, void *event_data,
		void *data)
{
	Escen_Viewer *thiz = data;
	Escen_State *escen_state;
	Ender_Element *label;
	const char *string;

	eon_bin_child_get(e, &label);
	eon_label_text_get(label, &string);
	escen_state = escen_ender_state_get(thiz->current_escen_ender, string);
	escen_instance_state_set(thiz->current_instance, escen_state);
}

static void populate_states(Escen_Viewer *thiz, Ender_Element *container, Escen_Ender *ee)
{
	int num_escen_state;
	int i;

	eon_container_child_clear(thiz->states_stack);
	/* iterate over the escen_ender and find every state */
	num_escen_state = escen_ender_state_count(ee);
	for (i = 0; i < num_escen_state; i++)
	{
		Ender_Element *radio;
		Ender_Element *label;
		Escen_State *escen_state;
		const char *name;

		escen_state = escen_ender_state_nth_get(ee, i);
		name = escen_state_name_get(escen_state);

		label = eon_label_new();
		eon_label_text_set(label, name);

		radio = eon_radio_new();
		ender_event_listener_add(radio, "MouseClick", state_clicked, thiz);
		eon_radio_group_name_set(radio, "states");
		eon_bin_child_set(radio, label);
		eon_element_horizontal_alignment_set(label, EON_HORIZONTAL_ALIGNMENT_LEFT);

		eon_element_vertical_alignment_set(radio, EON_VERTICAL_ALIGNMENT_TOP);
		eon_container_child_add(container, radio);
	}
}

static void ender_clicked(Ender_Element *e, const char *event_name, void *event_data,
		void *data)
{
	Escen_Viewer *thiz = data;
	Escen_Ender *new_escen_ender;
	Escen_Instance *new_instance;
	Ender_Element *new_ender;
	Ender_Element *label;
	Enesim_Renderer *r;
	const char *string = NULL;

	eon_bin_child_get(e, &label);
	if (!label) return;
	eon_label_text_get(label, &string);

	new_escen_ender = escen_ender_get(thiz->escen, string);
	if (!new_escen_ender) return;

	new_instance = escen_instance_new(new_escen_ender, EINA_TRUE);
	new_ender = escen_instance_ender_get(new_instance);
	/* TODO remove the old instance */
	//escen_instance_delete(thiz->current_instance);
	thiz->current_instance = new_instance;
	thiz->current_escen_ender = new_escen_ender;
	thiz->current_ender = new_ender;
	r = ender_element_object_get(thiz->current_ender);
	enesim_renderer_rop_set(r, ENESIM_BLEND);
	enesim_renderer_compound_layer_add(thiz->compound, r);

	/* FIXME we should not set this */
	enesim_renderer_compound_layer_remove(thiz->compound, thiz->current_renderer);
	thiz->current_renderer = r;

	/* just replace what we have as content of the scene area */
	populate_states(thiz, thiz->states_stack, thiz->current_escen_ender);
}


static void populate_enders(Escen_Viewer *thiz, Ender_Element *container, Escen *escen)
{
	int num_escen_ender;
	int i;

	/* iterate over the escen and find every ender_escen */
	num_escen_ender = escen_ender_count(escen);
	for (i = 0; i < num_escen_ender; i++)
	{
		Ender_Element *radio;
		Ender_Element *label;
		Escen_Ender *escen_ender;
		const char *name;

		escen_ender = escen_ender_nth_get(escen, i);
		name = escen_ender_name_get(escen_ender);

		printf("adding label %s\n", name); 
		label = eon_label_new();
		eon_label_text_set(label, name);

		radio = eon_radio_new();
		eon_radio_group_name_set(radio, "enders");
		ender_event_listener_add(radio, "MouseClick", ender_clicked, thiz);
		eon_bin_child_set(radio, label);
		eon_element_vertical_alignment_set(radio, EON_VERTICAL_ALIGNMENT_TOP);
		eon_element_horizontal_alignment_set(label, EON_HORIZONTAL_ALIGNMENT_LEFT);

		eon_container_child_add(container, radio);
	}
}

static void draw_ui(Escen_Viewer *thiz)
{
	Ender_Element *e;
	Enesim_Surface *s;
	Enesim_Renderer *r;

	/* add a stack with the enders, make it scrollable */
	e = eon_stack_new();
	/* TODO make it scrollable */
	eon_stack_orientation_set(e, EON_ORIENTATION_VERTICAL);
	eon_container_child_add(thiz->container, e);
	thiz->enders_stack = e;

	/* add another stack with the states of each escen */
	/* TODO make it scrollable */
	e = eon_stack_new();
	eon_stack_orientation_set(e, EON_ORIENTATION_VERTICAL);
	eon_container_child_add(thiz->container, e);
	thiz->states_stack = e;

	/* the scene area */
	e = eon_scrollview_new();
	thiz->scene_area_scrollview = e;
	eon_container_child_add(thiz->container, e);
	eon_stack_child_weight_set(thiz->container, e, 1);

	e = eon_surface_new();
	thiz->scene_area = e;

	/* TODO use the backend to create the surface */
	s = enesim_surface_new(ENESIM_FORMAT_ARGB8888, 640, 480);
	thiz->s = s;
	eon_surface_source_set(e, s);
	eon_element_vertical_alignment_set(e, EON_VERTICAL_ALIGNMENT_TOP);
	eon_element_horizontal_alignment_set(e, EON_HORIZONTAL_ALIGNMENT_LEFT);
	eon_container_child_add(thiz->scene_area_scrollview, e);

	r = enesim_renderer_checker_new();
	enesim_renderer_checker_even_color_set(r, 0xffcccccc);
	enesim_renderer_checker_odd_color_set(r, 0xffffffff);
	enesim_renderer_checker_width_set(r, 10);
	enesim_renderer_checker_height_set(r, 10);
	enesim_renderer_rop_set(r, ENESIM_FILL);
	thiz->checker = r;

	r = enesim_renderer_compound_new();
	enesim_renderer_compound_layer_add(r, thiz->checker);
	enesim_renderer_rop_set(r, ENESIM_FILL);
	thiz->compound = r;

	/* set the default state */
	thiz->current_escen_ender = escen_ender_nth_get(thiz->escen, 0);
	thiz->current_instance = escen_instance_new(thiz->current_escen_ender, EINA_TRUE);
	escen_instance_state_set(thiz->current_instance, escen_ender_state_get(thiz->current_escen_ender, "default"));
	thiz->current_ender = escen_instance_ender_get(thiz->current_instance);
	thiz->current_renderer = ender_element_object_get(thiz->current_ender);

	/* FIXME we should not set this */
	enesim_renderer_rop_set(thiz->current_renderer, ENESIM_BLEND);
	enesim_renderer_compound_layer_add(thiz->compound, thiz->current_renderer);

	/* populate the radio buttons */
	populate_enders(thiz, thiz->enders_stack, thiz->escen);
	populate_states(thiz, thiz->states_stack, thiz->current_escen_ender);
}

Escen_Viewer * escen_viewer_new(const char *engine, int width, int height, const char *file)
{
	Escen_Viewer *thiz;
	Escen *escen;
	Ender_Element *e;
	Eon_Backend *backend = NULL;
	Eon_Window *win;

	/* load the escen file */
	escen = escen_parser_load(file);
	if (!escen)
	{
		printf("Can not parse the file %s\n", file);
		return NULL;
	}
	/* do the setup */
	thiz = calloc(1, sizeof(Escen_Viewer));
	thiz->width = width;
	thiz->height = height;
	thiz->file = strdup(file);
	thiz->escen = escen;

	/* create the main container */
	e = eon_stack_new();
	eon_stack_orientation_set(e, EON_ORIENTATION_HORIZONTAL);
	thiz->container = e;

	if (!strcmp(engine, "remote"))
		backend = eon_ecore_remote_new();
	else if (!strcmp(engine, "sdl"))
		backend = eon_ecore_sdl_new();

	if (!backend)
		backend = eon_ecore_sdl_new();
	thiz->backend = backend;

	/* FIXME the win must be created on the _new function, but we are having issues with the propagation */
	win = eon_backend_window_new(thiz->backend, thiz->container, thiz->width, thiz->height);
	if (!win)
	{
		printf("Can not create the Eon_Window container\n");
		return NULL;
	}
	thiz->win = win;

	eon_backend_idler_add(thiz->backend, idler, thiz);

	return thiz;
}

void escen_viewer_free(Escen_Viewer *thiz)
{
	eon_backend_delete(thiz->backend);
	free(thiz);
}

int main(int argc, char **argv)
{
	Escen_Viewer *thiz;
	int width;
	int height;
	char *file;
	const char *engine;

	if (argc < 4)
	{
		help();
		return 0;
	}
	if (argc > 4)
	{
		engine = argv[1];
		width = strtol(argv[2], NULL, 10);
		height = strtol(argv[3], NULL, 10);
		file = argv[4];
		
	}
	else
	{
		width = strtol(argv[1], NULL, 10);
		height = strtol(argv[2], NULL, 10);
		file = argv[3];
		engine = "sdl";
	}

	eon_init();
	escen_init();


	thiz = escen_viewer_new(engine, width, height, file);
	if (!thiz) return -1;

	draw_ui(thiz);

	eon_backend_run(thiz->backend);
	escen_viewer_free(thiz);

	escen_shutdown();
	eon_shutdown();

	return 0;
}
