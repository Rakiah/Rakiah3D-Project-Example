/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bullet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkabbas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/23 11:36:16 by bkabbas           #+#    #+#             */
/*   Updated: 2016/03/23 13:06:00 by bkabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <r3d.h>
#include <wolf3d.h>

void		bullet_update(t_object *obj, void *data)
{
	(void)data;
	t_vector3f	motion;
	t_core		*core;

	core = get_core();
	v3f_set(&motion, 0, 0, 0);

	motion.z += 100  * core->delta_time;
	motion = trs_transform_direction(obj->transform, &motion);
	trs_translate(obj->transform, &motion);
}

void		create_bullet(t_window *win, t_object *obj, t_player *data)
{
	t_object *bullet;

	bullet = obj_new_init_mesh(win, data->bullet_mesh);
	bullet->transform->position = trs_get_child(obj->transform, 0)->position;
	bullet->transform->rotation = obj->transform->rotation;
	bullet->transform->scale = data->bullet_scale;
	obj_attach_behaviour(bullet, behaviour_new(NULL, NULL, bullet_update));
}
