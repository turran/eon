#include <Eon.h>

static Eon_Size size = { 0, 0 };
static Eina_Bool size_set = EINA_FALSE;
/*----------------------------------------------------------------------------*
 *                                 Helpers                                    *
 *----------------------------------------------------------------------------*/
static void help(void)
{
	printf("Usage: eon_loader FILE.eon FILE.png [width height]\n");
	printf("Where width and height defines the size of the destination image\n");
}

static void request_geometry_cb(Egueb_Dom_Event *ev, void *data)
{
	Egueb_Dom_Node *eon = data;
	Eon_Renderable_Size renderable_size;
	Eina_Rectangle geometry;

	printf("requesting geometry\n");
	eina_rectangle_coords_from(&geometry, 0, 0, 0, 0);
	if (!size_set)
	{
		int size_hints;

		size_hints = eon_renderable_size_hints_get(eon, &renderable_size);
		printf("size hints %08x\n", size_hints);
		if (size_hints & EON_RENDERABLE_HINT_MIN_MAX)
		{
			/* TODO set the min size for now */
			size.width = renderable_size.min_width;
			size.height = renderable_size.min_height;
		}
		size_set = EINA_TRUE;
	}
	eina_rectangle_coords_from(&geometry, 0, 0, size.width, size.height);
	eon_event_geometry_set(ev, &geometry);
}

int main(int argc, char *argv[])
{
	Egueb_Dom_Node *doc = NULL;
	Egueb_Dom_Node *eon = NULL;
	Enesim_Surface *s;
	Enesim_Buffer *b;
	Enesim_Log *err = NULL;
	Enesim_Stream *stream;

	if (argc < 3)
	{
		help();
		return -1;
	}

	if (argc > 3)
	{
		size.width = atoi(argv[3]);
 		if (argc < 5)
		{
			help();
			return -1;
		}
		size.height = atoi(argv[4]);
		size_set = EINA_TRUE;
	}

	if (!eon_init())
		return -1;

	stream = enesim_stream_file_new(argv[1], "r+");
	if (!stream)
	{
		printf("Fail to parse %s\n", argv[1]);
		goto shutdown_eon;
	}
	/* create the document */
	doc = eon_document_new();

	egueb_dom_parser_parse(stream, &doc);
	enesim_stream_unref(stream);

	eon = egueb_dom_document_element_get(doc);
	if (!eon)
	{
		printf("The parsed element is not a topmost EON element\n");
		goto shutdown_eon;
	}

	egueb_dom_node_event_listener_add(eon, EON_EVENT_GEOMETRY,
			request_geometry_cb, EINA_FALSE, eon);
	/* now process the document */
	egueb_dom_document_process(doc);

	if (!size_set || size.width <= 0 || size.height <= 0)
	{
		printf("Size is not set or is invalid, nothing to do\n");
		goto shutdown_eon;
	}
	/* finally draw */
	printf("Using a surface of size %d %d\n", size.width, size.height);
	s = enesim_surface_new(ENESIM_FORMAT_ARGB8888, size.width, size.height);
	eon_element_eon_draw(eon, s, ENESIM_ROP_FILL, NULL, 0, 0, &err);
	if (err)
	{
		enesim_log_dump(err);
		enesim_log_delete(err);
	}
	b = enesim_surface_buffer_get(s);
	enesim_image_file_save(argv[2], b, NULL);
	enesim_buffer_unref(b);
	enesim_surface_unref(s);

shutdown_eon:
	if (doc)
		egueb_dom_node_unref(doc);
	if (eon)
		egueb_dom_node_unref(eon);
	eon_shutdown();

	return 0;
}
