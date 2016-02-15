#include <r3d.h>
#include <wolf3d.h>

void		player_movement_init(t_object *obj, void **data)
{
	t_vector3f position;

	position = (t_vector3f) {6, 1, 6 };
	obj->transform->position = position;
	*data = load("resources/scenes/level_1.w3d");
}

static void	jump_handle(t_object *obj, t_vector3f *motion)
{
	static t_bool	jumping = FALSE;
	static float	speed = 0.0f;

	if (get_key_down(R) && !jumping)
	{
		speed = 22;
		jumping = TRUE;
	}
	if (jumping)
		speed -= 1;
	if (obj->transform->position.y < 1.0f)
	{
		obj->transform->position.y = 1.0f;
		speed = 0.0f;
		jumping = FALSE;
	}
	motion->y = speed;
}

void		player_movement_update(t_object *obj, t_w3d_scene *data)
{
	t_vector3f	motion;
	t_core		*core;
	int		x;
	int		y;

	core = get_core();
	v3f_set(&motion, 0, 0, 0);
	if (get_key_down(A))
		motion.x -= 10;
	if (get_key_down(D))
		motion.x += 10;
	if (get_key_down(W))
		motion.z += 10;
	if (get_key_down(S))
		motion.z -= 10;
	jump_handle(obj, &motion);
	motion.x *= core->delta_time;
	motion.y *= core->delta_time;
	motion.z *= core->delta_time;
	motion = trs_transform_direction(obj->transform, &motion);
	x = lroundf((obj->transform->position.x + motion.x + 0.3f) / 4);
	y = lroundf((obj->transform->position.z + motion.z + 0.3f) / 4);
	if (data->map[y][x]->collider)
	/*{}*/
		return ;
	x = lroundf((obj->transform->position.x + motion.x - 0.3f) / 4);
	y = lroundf((obj->transform->position.z + motion.z - 0.3f) / 4);
	if (data->map[y][x]->collider)
	/*{}*/
		return ;
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
	camera_holder->transform->position.y = 2.0f;
	trs_set_child(player->transform, camera->transform);
	obj_attach_behaviour(player, behaviour_new(NULL, NULL,
						mouse_look_x_update));
	obj_attach_behaviour(player, behaviour_new(player_movement_init, NULL,
						(void (*)(t_object *, void *))
						player_movement_update));
	obj_attach_behaviour(camera_holder, behaviour_new(NULL, NULL,
							mouse_look_y_update));
	return (player);
}
