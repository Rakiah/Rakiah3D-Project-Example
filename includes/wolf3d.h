#include "r3d.h"
#include "rlists.h"
#include "get_next_line.h"

typedef struct s_binded_object t_binded_object;

struct	s_binded_object
{
	int		height;
	t_vector3f	rotation;
	t_vector3f	scale;
	t_object	*item;
};

void		*w3d_loader(char *path);
void		player_movement_init(t_object *obj, void **data);
void		player_movement_update(t_object *obj, void *data);
t_object	*create_fps_player(t_window *win);
void		mouse_look_x_update(t_object *obj, void *data);
void		mouse_look_y_update(t_object *obj, void *data);
