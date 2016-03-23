/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkabbas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/23 11:09:06 by bkabbas           #+#    #+#             */
/*   Updated: 2016/03/23 13:24:26 by bkabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <r3d.h>
#include <wolf3d.h>

void		player_movement_init(t_object *obj, void **data)
{
	t_player	*player;

	player = malloc(sizeof(t_player));
	player->player_speed = 15;
	player->sprint_modifier = 1.8f;
	player->bullet_speed = 15;
	player->jump_speed = 18;
	player->gravity = 1;
	player->fire_rate = 0.5f;
	player->bullet_scale = (t_vector3f) { 0.2f, 0.2f, 0.2f };
	player->bullet_mesh = load("resources/meshes/icosphere.obj");
	player->scene = load("resources/scenes/level_1.w3d");
	player->object = obj;
	obj->transform->position = (t_vector3f) { 6, 1, 6 };

	*data = (void *)player;
}

static void	jump_handle(t_object *obj, t_vector3f *motion, t_player *data)
{
	static t_bool	jumping = FALSE;
	static float	speed = 0.0f;

	if (get_key_down(R) && !jumping)
	{
		speed = data->jump_speed;
		jumping = TRUE;
	}
	if (jumping)
		speed -= data->gravity;
	if (obj->transform->position.y < 1.0f)
	{
		obj->transform->position.y = 1.0f;
		speed = 0.0f;
		jumping = FALSE;
	}
	motion->y = speed;
}

static void	player_shoot(t_object *obj, t_player *data)
{
	static float	timer = 0.5f;
	t_core			*core;

	core = get_core();
	if (timer > 0.0f)
		timer -= core->delta_time;
	if (get_mouse_button_down(1) && timer <= 0.0f)
	{
		ft_putstr("SHOOT\n");
		create_bullet(core->window, obj, data);
		timer = data->fire_rate;
	}
}

void		player_movement_update(t_object *obj, t_player *data)
{
	t_vector3f	motion;
	t_core		*core;
	int			x;
	int			y;

	core = get_core();
	v3f_set(&motion, 0, 0, 0);
	if (get_key_down(A))
		motion.x -= data->player_speed;
	if (get_key_down(D))
		motion.x += data->player_speed;
	if (get_key_down(W))
		motion.z += data->player_speed;
	if (get_key_down(S))
		motion.z -= data->player_speed;
	player_shoot(obj, data);
	jump_handle(obj, &motion, data);
	motion.x *= core->delta_time;
	motion.y *= core->delta_time;
	motion.z *= core->delta_time;
	motion = trs_transform_direction(obj->transform, &motion);
	x = lroundf((obj->transform->position.x + motion.x + 0.3f) / 4);
	y = lroundf((obj->transform->position.z + motion.z + 0.3f) / 4);
	if (data->scene->map[y][x]->collider)
	/*{}*/
		return ;
	x = lroundf((obj->transform->position.x + motion.x - 0.3f) / 4);
	y = lroundf((obj->transform->position.z + motion.z - 0.3f) / 4);
	if (data->scene->map[y][x]->collider)
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
