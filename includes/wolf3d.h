#include "r3d.h"
#include "rlists.h"
#include "get_next_line.h"

typedef struct s_binded_object	t_binded_object;
typedef struct s_w3d_scene	t_w3d_scene;
typedef struct s_alias		t_alias;
typedef struct s_block		t_block;

struct	s_binded_object
{
	float		height;
	float		floor_height;
	t_bool		collision;
	t_vector3f	rotation;
	t_vector3f	scale;
	t_object	*item;
};

struct	s_alias
{
	char *alias;
	char *real;
};

struct	s_block
{
	t_bool	collider;
	float	floor_height;
};

struct	s_w3d_scene
{
	t_block	***map;
	int	width;
	int	height;
};

void		*w3d_loader(char *path);
void		player_movement_init(t_object *obj, void **data);
void		player_movement_update(t_object *obj, t_w3d_scene *data);
t_object	*create_fps_player(t_window *win);
void		mouse_look_x_update(t_object *obj, void *data);
void		mouse_look_y_update(t_object *obj, void *data);
