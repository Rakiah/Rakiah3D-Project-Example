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

static int parse_index(char *index)
{
	if (*index >= 'A' && *index <= 'Z')
		return (10 + (*index - 'A'));
	else if (*index >= '0' && *index <= '9')
		return (*index - '0');
	return (-1);
}

static char *find_alias(char *to_find, t_list *alias)
{
	t_alias *iter;
	char	*result;

	result = NULL;
	list_set_start(alias);
	while ((iter = list_next(alias)) != NULL)
	{
		if (ft_strequ(to_find, iter->alias))
			result = iter->real;
	}
	return (result);
}

static void parse_map(char **splitted, t_list *items, t_list *alias, t_list *map)
{
	static int	y = 0;
	int		x;
	int		sub_x;
	char		**sub_split;
	char		*alias_real;
	t_binded_object *item;
	t_object	*tmp;
	t_core		*core;
	t_list		*line;
	t_block		*block;
	int		id;

	line = list_new(sizeof(t_block **));
	core = get_core();
	x = -1;
	while (splitted[++x] != NULL)
	{
		block = malloc(sizeof(t_block));
		block->collider = FALSE;
		block->floor_height = 0.0f;
		if ((id = parse_index(splitted[x]) - 1) < 0)
		{
			list_push_back(line, block);
			continue ;
		}
		if ((alias_real = find_alias(splitted[x], alias)) == NULL)
			alias_real = splitted[x];
		sub_x = -1;
		sub_split = ft_strsplit(alias_real, '|', NULL);
		while (sub_split[++sub_x] != NULL)
		{
			id = parse_index(sub_split[sub_x]) -1;
			item = list_get_data_at(items, id);
			tmp = obj_new_init_mesh(core->window, item->item->mesh);
			v3f_set(&tmp->transform->position, x * 4, item->height, y * 4);
			tmp->transform->rotation = item->rotation;
			tmp->transform->scale = item->scale;
			if (item->collision)
			{
				block->collider = TRUE;
				block->floor_height = item->floor_height;
			}
		}
		list_push_back(line, block);
		line_free(sub_split, NULL);
	}
	list_push_back(map, line);
	y++;
}

static void parse_alias(char **splitted, t_list *aliases)
{
	t_alias *alias;

	if ((alias = malloc(sizeof(t_alias))) == NULL)
		error_exit("alloc failed");
	alias->alias = ft_strdup(splitted[1]);
	alias->real = ft_strdup(splitted[2]);
	list_push_back(aliases, alias);
}

#include <stdio.h>
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
	item->collision = ft_strequ(splitted[10], "yes");
	item->floor_height = strtod(splitted[11], NULL);
	list_push_back(items, item);
}

static t_w3d_scene	*list_to_scene(t_list *map)
{
	t_w3d_scene	*scene;
	t_list		*iterator;
	t_block		***map_blocks;
	int		i;

	scene = malloc(sizeof(t_w3d_scene));
	map_blocks = malloc(sizeof(t_block **) * map->count);
	scene->height = map->count;
	i = 0;
	while ((iterator = list_next(map)) != NULL)
	{
		map_blocks[i] = (t_block **)list_to_array(iterator);
		scene->width = iterator->count;
		list_clear(iterator);
		i++;
	}
	list_clear_inner(map, list_default_remove_functor);
	free(map);
	scene->map = map_blocks;
	return (scene);
}

void		*w3d_loader(char *path)
{
	char	**split;
	t_list	*items;
	t_list	*aliases;
	t_list	*map;
	char	*line;
	int	fd;

	map = list_new(sizeof(t_block *));
	items = list_new(sizeof(t_binded_object *));
	aliases = list_new(sizeof(t_alias *));
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
		else if (ft_strequ(split[0], "alias"))
			parse_alias(split, aliases);
		else if (ft_strequ(split[0], "bind"))
			parse_bind(split, items);
		else
			parse_map(split, items, aliases, map);
		line_free(split, line);
	}
	list_clear_inner(items, item_object_clearer);
	return (list_to_scene(map));
}
