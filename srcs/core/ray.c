/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acoezard <acoezard@student.42nice.f>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/21 14:04:25 by acoezard          #+#    #+#             */
/*   Updated: 2021/12/21 14:09:11 by acoezard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	ray_sphere(t_vec *orig, t_vec *ray, t_sphere *sphere, t_hit *hit)
{
	t_vec	oc = sub(orig, &sphere->pos);
	float	a = dot(ray, ray);
	float	b = dot(&oc, ray);
	float	c = dot(&oc, &oc) - sphere->srad;
	float	d = b * b - a * c;

	if (d <= EPSILON)
		return ((void)(hit->dist = -1));
	hit->dist = (-b - sqrt(b * b - a * c)) / a;
	hit->pos = mult(ray, hit->dist);
	hit->pos = add(orig, &hit->pos);
	hit->normal = normalize(sub(&hit->pos, &sphere->pos));
}

void	ray_plane(t_vec *orig, t_vec *ray, t_plane *plane, t_hit *hit)
{
	float	d = dot(&plane->normal, ray);

	if (d >= EPSILON && d <= EPSILON)
		return ((void)(hit->dist = -1));
	hit->pos = sub(&plane->pos, orig);
	hit->dist = dot(&hit->pos, &plane->normal) / d;
	hit->pos = mult(ray, hit->dist);
	hit->pos = add(orig, &hit->pos);
	hit->normal = plane->normal;
}

void	ray_cylinder(t_vec *orig, t_vec *ray, t_cylinder *cylinder, t_hit *hit)
{
	float	a = (ray->x * ray->x) + (ray->z * ray->z);
	float	b = 2 * (ray->x * (orig->x - cylinder->pos.x) + ray->z * (orig->z - cylinder->pos.z));
	float	c = (orig->x - cylinder->pos.x) * (orig->x - cylinder->pos.x) + (orig->z - cylinder->pos.z) * (orig->z - cylinder->pos.z) - cylinder->srad;
	float	delta = b * b - 4 * (a * c);
	if(fabs(delta) < 0.001 || delta < 0.0)
		return ((void)(hit->dist = -1));
	float	t = fmin((-b - sqrt(delta)) / (2 * a), (-b + sqrt(delta)) / (2 * a));
	float	r = orig->y + t * ray->y;
	if (!(r >= cylinder->pos.y) && (r <= cylinder->pos.y + cylinder->height))
		return ((void)(hit->dist = -1));
	hit->dist = t;
}

void	ray_cone(t_vec *orig, t_vec *ray, t_cone *cone, t_hit *hit)
{
	(void)orig;
	(void)ray;
	(void)cone;
	hit->dist = 0;
}

inline int	ray_scene(t_vec *orig, t_vec *ray, t_scene *scene, t_hit *closest)
{
	t_hit			hit;
	register t_obj	*obj = scene->obj;

	closest->dist = -1;
	while (obj->func)
	{
		hit.dist = -1;
		obj->func(orig, ray, obj->data, &hit);
		if (hit.dist > CAMERA_CLIP_START
			&& (closest->dist == -1 || hit.dist < closest->dist))
		{
			closest->dist = hit.dist;
			closest->pos = hit.pos;
			closest->normal = hit.normal;
			closest->obj = obj;
		}
		obj++;
	}
	return (closest->dist != -1);
}
