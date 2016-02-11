#include <r3d.h>
#include <wolf3d.h>

void	mouse_look_x_update(t_object *obj, void *data)
{
	(void)data;
	t_vector3f	motion;
	t_vector2f	mouse_pos;
	t_vector2f	screen_halfs;
	t_core		*core;
	
	core = get_core();
	screen_halfs.x = core->window->width / 2;
	screen_halfs.y = core->window->height / 2;
	mouse_pos = get_mouse_pos();
	v3f_set(&motion, 0,
		(mouse_pos.x - screen_halfs.x) * core->delta_time * 10, 0);
	trs_rotate(obj->transform, &motion);
}

void	mouse_look_y_update(t_object *obj, void *data)
{
	(void)data;
	t_vector3f	motion;
	t_vector2f	mouse_pos;
	t_vector2f	screen_halfs;
	t_core		*core;
	
	core = get_core();
	screen_halfs.x = core->window->width / 2;
	screen_halfs.y = core->window->height / 2;
	mouse_pos = get_mouse_pos();
	v3f_set(&motion, (mouse_pos.y - screen_halfs.y) * core->delta_time * 10,
			0, 0);
	trs_rotate(obj->transform, &motion);
	if (obj->transform->rotation.x >= 80)
		obj->transform->rotation.x = 80;
	if (obj->transform->rotation.x <= -80)
		obj->transform->rotation.x = -80;
}
