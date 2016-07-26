/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced_test_engine.                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkabbas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/22 12:53:43 by bkabbas           #+#    #+#             */
/*   Updated: 2016/07/26 19:32:37 by Rakiah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <r3d.h>

void	process_input()
{
	t_vector3f	motion;
	t_transform	*camera;
	t_core		*core;

	core = get_core();
	motion = V3F_ZERO;
	camera = core->camera->transform;
	if (get_key_down(A))
		motion.x -= 30;
	if (get_key_down(D))
		motion.x += 30;
	if (get_key_down(W))
		motion.z += 30;
	if (get_key_down(S))
		motion.z -= 30;
	if (get_key_down(SPACE))
		motion.y += 30;
	motion = v3f_mul_float(motion, core->delta_time);
	trs_translate(camera, trs_transform_direction(camera, motion));
}

void	mouse_look()
{
	t_vector2f	mouse_pos;
	t_vector2f	half;
	t_vector2f	diff;
	t_transform	*camera;
	t_core		*core;
	
	core = get_core();
	camera = core->camera->transform;
	mouse_pos = get_mouse_pos();
	half = V2F_INIT(core->width / 2, core->height / 2);

	diff = v2f_mul_float(v2f_sub(mouse_pos, half), core->delta_time * 10);
	trs_rotate(camera, V3F_INIT(diff.y, diff.x, 0));
}

void	print_frame_rate()
{
	static int		frames = 0;
	static float	timer = 0.0f;

	frames++;
	timer += get_core()->delta_time;
	if (timer >= 1.0f)
	{
		rstd_putstr("FPS : [");
		rstd_putnbr(frames);
		rstd_putstr("]\n");
		frames = 0;
		timer = 0.0f;
	}
}

void	update()
{
	static t_bool	skip_frame = TRUE;
	static t_bool	paused = FALSE;

	if (skip_frame)
	{
		skip_frame = FALSE;
		return ;
	}
	if (get_key_up(P))
	{
		paused = !paused;
		core_lock_cursor(!paused);
	}
	if (!paused)
	{
		process_input();
		mouse_look();
	}
	print_frame_rate();
}

int		main(void)
{
	t_object	*model;

	core_init(update, 1280, 800, 60);
	camera_new_init();
	model = obj_new_init_mesh(resources_find("ahri"));
	trs_set_scale(model->transform, V3F_INIT(0.1f, 0.1f, 0.1f));
	trs_set_pos(model->transform, V3F_INIT(0, -5, 20));
	trs_set_rot(model->transform, V3F_INIT(-90, 180, 0));
	model->mesh->material->texture = resources_find("ahri_tex");
	core_lock_cursor(TRUE);
	core_start();
	return (0);
}
