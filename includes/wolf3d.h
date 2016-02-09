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

void	*w3d_loader(char *path);
