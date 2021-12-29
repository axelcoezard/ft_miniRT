/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.f>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 13:12:22 by acoezard          #+#    #+#             */
/*   Updated: 2021/12/29 13:30:25 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "maths.h"
# include "ray.h"
# include "vector.h"
# include "struct.h"

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <mlx.h>

# include <time.h>
# include <stdio.h>

char	*gnl(int fd);
int		free_splits(char **splits, int n);
char	**split(const char *s, char c);

void	push_obj(t_scene *scene, t_obj obj);
void	push_light(t_scene *scene, t_light light);
void	push_camera(t_scene *scene, t_camera cam);
void	push_ambient(t_scene *scene, t_ambient ambient);

t_scene	parse(int argc, char **argv);
void	parse_line(char *type, char **arg, t_scene *scene);

void	err(char *s);

int		hook_close(t_scene *scene);
int		hook_key_down(int key, t_scene *scene);
int		hook_key_up(int key, t_scene *scene);
int		hook_mouse_move(int x, int y, t_scene *scene);
int		hook_button_down(int button, int x, int y, t_scene *scene);
int		hook_button_up(int button, int x, int y, t_scene *scene);

void	controls_init(t_scene *scene);
int		controls_listen(t_scene *scene);
void	make_straight_move(t_scene *scene, int direction);
void	make_side_move(t_scene *scene, int direction);
void	make_rotation(t_scene *scene, int direction);

void	render(const t_scene *scene, const t_window *win,
			const t_camera *cam, int *buf);

static inline t_vec	radian_to_vector(const t_vec *rot)
{
	float	sin_z;
	float	cos_z;
	float	sin_y;
	float	sin_x;
	float	cos_x;

	sin_z = sinf(rot->z);
	cos_z = cosf(rot->z);
	sin_y = sinf(rot->y);
	sin_x = sinf(rot->x);
	cos_x = cosf(rot->x);
	return ((t_vec){
		-cos_z * sin_y * sin_x - sin_z * cos_x,
		-sin_z * sin_y * sin_x + cos_z * cos_x,
		cosf(rot->y) * sin_x
	});
}

static inline t_vec	reflect(const t_vec *ray, const t_vec *normal)
{
	t_vec	tmp;

	tmp = mult(normal, 2 * dot(ray, normal));
	return (sub(ray, &tmp));
}

#endif
