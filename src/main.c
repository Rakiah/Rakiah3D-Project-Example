#include <r3d.h>

void	process_input()
{
	t_vector3f	motion;
	t_core		*core;
	t_transform	*camera;

	core = get_core();
	camera = core->window->camera->transform;
	if (get_key_down(A))
		motion.x -= core->delta_time;
	if (get_key_down(D))
		motion.x += core->delta_time;
	if (get_key_down(W))
		motion.z += core->delta_time;
	if (get_key_down(S))
		motion.z -= core->delta_time;
	motion = trs_transform_direction(camera, &motion);
	trs_translate(camera, &motion);
}

void	mouse_look()
{
	t_vector3f	motion;
	t_vector2f	mouse_pos;
	t_vector2f	screen_halfs;
	t_transform	*camera;
	t_core		*core;
	
	core = get_core();
	camera = core->window->camera->transform;
	screen_halfs.x = core->window->width / 2;
	screen_halfs.y = core->window->height / 2;
	mouse_pos = get_mouse_pos();
	v3f_set(&motion,
		(mouse_pos.x - screen_halfs.x) * core->delta_time,
		(mouse_pos.y - screen_halfs.y) * core->delta_time, 0);
	trs_rotate(camera, &motion);
}

void	*w3ds_loader(char *path)
{
	(void)path;
	return (NULL);
}

void	update()
{
	static int	frames = 0;
	static float	timer = 0.0f;

	frames++;
	timer += get_core()->delta_time;
	if (timer >= 1.0f)
	{
		ft_putstr("FPS : [");
		ft_putnbr(frames);
		ft_putstr("]\n");
		frames = 0;
		timer = 0.0f;
	}
}

int	main(void)
{
	core_init(update, NULL, NULL, 60);
	camera_new_init(window_new(800, 600, "wolf3D"));
	core_add_loader(w3ds_loader, ft_strdup("w3ds"));
	core_lock_cursor(TRUE);
	load("level_1.w3ds");
	core_start();
	return (0);
}
