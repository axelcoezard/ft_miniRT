/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.f>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 13:42:38 by acoezard          #+#    #+#             */
/*   Updated: 2021/12/22 12:53:25 by mberger-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	check_args(int argc, char **argv)
{
	(void) argv;
	if (argc != 2)
		err("usage: ./minirt [minirtfile]");
}

int	open_file(char *filename)
{
	int	fd;

	fd = open(filename, O_DIRECTORY);
	if (fd != -1)
		err("file is a directory");
	close(fd);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		err("file can't be open");
	return (fd);
}

t_window	window_open(char *name, int width, int height)
{
	t_window	win;
	int			null;

	win.width = width;
	win.height = height;
	win.mlx = mlx_init();
	win.win = mlx_new_window(win.mlx, width, height, name);
	win.img = mlx_new_image(win.mlx, width, height);
	win.buf = (int *)mlx_get_data_addr(win.img, &null, &null, &null);
	return (win);
}

// HARD CODDED DATA TO REMOVE
//const t_bump_map	bump_maps[] = {
//	load_bump_map(scene.window, "assets/test.xpm")
//};
const t_sphere		g_spheres[] = {
{(t_vec){0, 0, 0}, 2, 4},
{(t_vec){0, 5, 0}, 1, 1},
{(t_vec){4, -4, 3.5}, .5, .25}
};
//{(t_vec){0, 10, 0}, (t_vec){0, 1, 0}, bump_maps + 0}
const t_plane		g_planes[] = {
{(t_vec){0, 0, -4}, (t_vec){0, 0, 1}, NULL},
};
const t_cylinder	g_cylinders[] = {
{(t_vec){5, 5, 0}, (t_vec){0, 0, 1}, 2, 4, 1}
};
const t_cone		g_cones[] = {
{(t_vec){1, 1, 0}, (t_vec){0, 0, 1}}
};
t_obj				g_objects[] = {
{ray_sphere, RED, (void *)(g_spheres + 0)},
{ray_sphere, GREEN, (void *)(g_spheres + 1)},
{ray_sphere, BLUE, (void *)(g_spheres + 2)},
{ray_plane, GREEN, (void *)(g_planes + 0)},
{ray_cylinder, BLUE, (void *)(g_cylinders + 0)},
{ray_cone, RED, (void *)(g_cones + 0)},
{NULL, BLACK, NULL}
};
t_light				g_lights[] = {
{{5, -5, 5}, RED, 2}
};
//end of hard codded data

void	parse_line(char *type, char **arg, t_scene *scene)
{
	int	i;

	(void)scene;
	if (type[0] == 'A' && type[1] == '\0')
		;
	else if (type[0] == 'C' && type[1] == '\0')
		;
	else if (type[0] == 'L' && type[1] == '\0')
		;
	else if (type[0] == 's' && type[1] == 'p' && type[2] == '\0')
		;
	else if (type[0] == 'p' && type[1] == 'l' && type[2] == '\0')
		;
	else if (type[0] == 'c' && type[1] == 'y' && type[2] == '\0')
		;
	else if (type[0] == 'c' && type[1] == 'o' && type[2] == '\0')
		;
	else
		return ((void)warn("unrecognized type", type));
	printf("%2s", type);
	i = -1;
	while (arg[++i])
		printf("  [%-15s]", arg[i]);
	printf("\n");
}

t_scene	parse(int ac, char **av)
{
	t_scene	scene;//TODO initialize every thing at first
	int		fd;
	char	*s;
	char	**splits;

	check_args(ac, av);
	fd = open_file(av[1]);

	while (1)
	{
		s = gnl(fd);
		if (s == NULL)
			break ;
		splits = split(s, ' ');
		if (splits && *splits)
			parse_line(*splits, splits + 1, &scene);
		free_splits(splits, -1);
	}
	close(fd);

	//TODO remove this and replace by check number of camera ...
	scene.win = window_open("miniRT", WIDTH, HEIGHT);
	scene.cam.pos = (t_vec){8, -4, 5.5};
	scene.cam.rot_euler = (t_vec){-PI / 4, 0, PI / 4};
	scene.cam.width = WIDTH;
	scene.cam.height = HEIGHT;
	scene.cam.fov_pixel = M_PI_2 / WIDTH;
	scene.ambient_color = 0xebebeb;
	scene.button = 0;
	scene.obj = g_objects;
	scene.lights = g_lights;
	return (scene);
}
