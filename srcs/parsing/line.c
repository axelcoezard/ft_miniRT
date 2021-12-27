/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matubu <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 21:05:43 by matubu            #+#    #+#             */
/*   Updated: 2021/12/26 22:21:13 by matubu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

//parse a number from a string
static float	num(char *s)
{
	char	*end;
	float	dec;
	float	point;
	int		neg;

	if (s == NULL)
		err("missing float");
	dec = 0;
	point = 0;
	neg = *s == '-' && s++;
	end = s;
	while (*end)
		if (s == end++ && *s >= '0' && *s <= '9')
			dec = dec * 10.0 - *s++ + '0';
		else if (*s != '.')
			err("invalid number");
	while (--end > s)
		if (*end >= '0' && *end <= '9')
			point = (point - *end + '0') / 10;
		else
			err("invalid number");
	if (!neg)
		return (-dec - point);
	return (dec + point);
}

//parse a 3d vector from a string
static t_vec	vec(char *s)
{
	char	**splits;
	t_vec	vec;

	if (s == NULL)
		err("missing vector");
	splits = split(s, ',');
	if (!splits || !splits[0] || !splits[1] || !splits[2] || splits[3])
		err("invalid vector");
	vec = (t_vec){num(splits[0]), num(splits[1]), num(splits[2])};
	free_splits(splits, -1);
	return (vec);

}

//parse a color from a string
static int	col(char *s)
{
	char	**splits;
	int		color;

	if (s == NULL)
		err("missing color");
	splits = split(s, ',');
	if (!splits || !splits[0] || !splits[1] || !splits[2] || splits[3])
		err("invalid color");
	color = ((int)num(splits[0]) & 255) << 16
		| ((int)num(splits[1]) & 255) << 8
		| ((int)num(splits[2]) & 255) << 0;
	free_splits(splits, -1);
	return (color);
}

//push a new object in scene
static void	obj(t_scene *scene, t_obj obj)
{
	if (scene->obj_count == 1023)
		err("too many objects");
	scene->obj[scene->obj_count++] = obj;
}

void	parse_line(char *type, char **arg, t_scene *scene)
{
	if (type[0] == 'A' && type[1] == '\0')
		scene->ambient = (t_ambient){num(*arg++), col(*arg++)};
	else if (type[0] == 'C' && type[1] == '\0')
		scene->cam = (t_camera){vec(*arg++), vec(*arg++),
			WIDTH, HEIGHT, num(*arg++) / WIDTH};
	else if (type[0] == 'L' && type[1] == '\0')
		scene->lights[0] = (t_light){vec(*arg++), num(*arg++), col(*arg++)};
	else if (type[0] == 's' && type[1] == 'p' && type[2] == '\0')
		obj(scene, (t_obj){.func = ray_sphere, .sphere
				= (t_sphere){vec(*arg++), num(*arg++)}, .color = col(*arg++)});
	else if (type[0] == 'p' && type[1] == 'l' && type[2] == '\0')
		obj(scene, (t_obj){.func = ray_plane,
			.plane = (t_plane){vec(*arg++), vec(*arg++)},
			.color = col(*arg++)});
	else if (type[0] == 'c' && type[1] == 'y' && type[2] == '\0')
		obj(scene, (t_obj){.func = ray_cylinder,
			.cylinder = (t_cylinder){vec(*arg++), vec(*arg++), num(*arg++),
			num(*arg++)}, .color = col(*arg++)});
	else if (type[0] == 'c' && type[1] == 'o' && type[2] == '\0')
		obj(scene, (t_obj){.func = ray_cone,
			.cone = (t_cone){vec(*arg++), vec(*arg++)}, .color = col(*arg++)});
	else
		err("invalid object type");
	if (*arg)
		err("too many field");
}