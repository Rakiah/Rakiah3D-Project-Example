#include <r3d.h>
#include <wolf3d.h>

void	process_input()
{
	t_vector3f	motion;
	t_core		*core;
	t_transform	*camera;

	core = get_core();
	camera = core->window->camera->transform;
	v3f_set(&motion, 0, 0, 0);
	if (get_key_down(A))
		motion.x -= core->delta_time * 30;
	if (get_key_down(D))
		motion.x += core->delta_time * 30;
	if (get_key_down(W))
		motion.z += core->delta_time * 30;
	if (get_key_down(S))
		motion.z -= core->delta_time * 30;
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
		(mouse_pos.y - screen_halfs.y) * core->delta_time * 10,
		(mouse_pos.x - screen_halfs.x) * core->delta_time * 10, 0);
	trs_rotate(camera, &motion);
}

void	update()
{
	static int	frames = 0;
	static float	timer = 0.0f;
	static t_bool	skip_frame = TRUE;
	static t_bool	paused = FALSE;

	if (skip_frame)
	{
		skip_frame = FALSE;
		return ;
	}
	if (!paused)
	{
		/*mouse_look();*/
		/*process_input();*/
	}
	if (get_key_up(P))
	{
		paused = !paused;
		core_lock_cursor(!paused);
		((t_ui_element *)get_core()->ui_renderer->elements->start->data)->active = paused;
		((t_button *)get_core()->ui_renderer->buttons->start->data)->active = paused;
	}
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
void	mouse_click_down(t_button *button, void *data)
{
	(void)button;
	(void)data;
	/*ft_putendl("click down");*/
}

void	mouse_click_up(t_button *button, void *data)
{
	(void)button;
	(void)data;
	exit(0);
	/*ft_putendl("click up");*/

}

void	mouse_hover(t_button *button, void *data)
{
	(void)button;
	(void)data;
	/*ft_putendl("hover");*/
}

int	main(void)
{
	t_button	*button;
	t_rect		button_rect;

	button_rect = (t_rect) { 20, 20, 200, 30 };
	core_init(update, NULL, NULL, 60);
	create_fps_player(window_new(1000, 600, "wolf3d"));
	core_add_loader(w3d_loader, ft_strdup("w3d"));
	core_lock_cursor(TRUE);
	button = button_new_init(&button_rect, "Quit", mouse_click_up, NULL);
	button->active = FALSE;
	button->drawable->active = FALSE;
	button_bind_click_down(button, mouse_click_down, NULL);
	button_bind_mouse_hover(button, mouse_hover, NULL);
	load("resources/scenes/level_1.w3d");
	core_start();
	return (0);
}
