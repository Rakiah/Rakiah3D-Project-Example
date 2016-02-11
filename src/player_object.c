#include <r3d.h>
#include <wolf3d.h>

void		player_movement_init(t_object *obj, void **data)
{
	t_vector3f position;

	(void)data;
	position = (t_vector3f) {24, 4, 2 };
	obj->transform->position = position;
}

void		player_movement_update(t_object *obj, void *data)
{
	t_vector3f	motion;
	t_core		*core;

	(void)data;
	core = get_core();
	v3f_set(&motion, 0, 0, 0);
	if (get_key_down(A))
		motion.x -= core->delta_time * 10;
	if (get_key_down(D))
		motion.x += core->delta_time * 10;
	if (get_key_down(W))
		motion.z += core->delta_time * 10;
	if (get_key_down(S))
		motion.z -= core->delta_time * 10;
	motion = trs_transform_direction(obj->transform, &motion);
	trs_translate(obj->transform, &motion);
}

t_object	*create_fps_player(t_window *win)
{
	t_object	*player;
	t_object	*camera_holder;
	t_camera	*camera;

	player = obj_new_init_mesh(win, NULL);
	camera = camera_new_init(win);
	camera_holder = obj_new(win, NULL, camera->transform);
	trs_set_child(player->transform, camera->transform);
	obj_attach_behaviour(player, behaviour_new(NULL, NULL,
						mouse_look_x_update));
	obj_attach_behaviour(player, behaviour_new(player_movement_init, NULL,
						player_movement_update));
	obj_attach_behaviour(camera_holder, behaviour_new(NULL, NULL,
							mouse_look_y_update));
	return (player);
}
