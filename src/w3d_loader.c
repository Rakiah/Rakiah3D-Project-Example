#include "wolf3d.h"

static void line_free(char **splitted, char *line)
{
	int i;

	i = 0;
	while (splitted[i] != NULL)
		free(splitted[i++]);
	free(line);
}

static void item_object_clearer(t_node *iterated)
{
	free(iterated->data);
}

static void parse_map(char **splitted, t_list *items, t_core *core)
{
	static int	y = 0;
	int		x;
	int		sub_x;
	char		**sub_split;
	t_binded_object *item;
	t_object	*tmp;
	int		id;

	x = -1;
	while (splitted[++x] != NULL)
	{
		if ((id = ft_atoi(splitted[x]) - 1) < 0)
			continue ;
		sub_x = -1;
		sub_split = ft_strsplit(splitted[x], '|', NULL);
		while (sub_split[++sub_x] != NULL)
		{
			id = ft_atoi(sub_split[sub_x]) -1;
			item = list_get_data_at(items, id);
			tmp = obj_new_init_mesh(core->window, item->item->mesh);
			v3f_set(&tmp->transform->position, x * 2, item->height, y * 2);
			tmp->transform->rotation = item->rotation;
			tmp->transform->scale = item->scale;
		}
		line_free(sub_split, NULL);
	}
	y++;
}

static void parse_bind(char **splitted, t_list *items)
{
	t_binded_object	*item;

	if ((item = malloc(sizeof(t_binded_object))) == NULL)
		error_exit("alloc failed");
	if (ft_strequ(splitted[1], "use"))
		item->item = ((t_binded_object *)list_get_data_at(items, ft_atoi(splitted[2]) - 1))->item;
	else if (ft_strequ(splitted[1], "new"))
		item->item = load(splitted[2]);
	trs_set_pos_x(item->item->transform, 1000);
	item->height = strtod(splitted[3], NULL);
	v3f_set(&item->rotation, strtod(splitted[4], NULL),
				strtod(splitted[5], NULL),
				strtod(splitted[6], NULL));
	v3f_set(&item->scale, strtod(splitted[7], NULL),
				strtod(splitted[8], NULL),
				strtod(splitted[9], NULL));
	list_push_back(items, item);
}

void		*w3d_loader(char *path)
{
	char	**split;
	t_list	*items;
	t_core	*core;
	char	*line;
	int	fd;

	core = get_core();
	items = list_new(sizeof(t_binded_object *));
	if ((fd = open(path, O_RDONLY)) < 0)
		error_exit("no file found");
	while (get_next_line(fd, &line))
	{
		split = ft_strsplit(line, ' ', NULL);
		if (ft_strequ(split[0], "#"))
		{
			line_free(split, line);
			continue ;
		}
		else if (ft_strequ(split[0], "bind"))
			parse_bind(split, items);
		else
			parse_map(split, items, core);
		line_free(split, line);
	}
	list_clear_inner(items, item_object_clearer);
	return (NULL);
}
