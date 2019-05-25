#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0*vec3(drand48(),drand48(),0) - vec3(1,1,0);
	} while (dot(p,p) >= 1.0);
	return p;
}

class camera {
	public:
		camera() {}
		camera(vec3 position, vec3 target, vec3 vup, float fov, float aspect,float aperture, float distance) { // field of view é vertical, não horizontal(depois pesquisar horizontal)
			lens_radius = aperture/2.0;
			float theta = fov*M_PI/180;
			float half_height = tan(theta/2);
			float half_width = aspect * half_height;
			origin = position;
			w = unit_vector(position-target); // t
			u = unit_vector(cross(vup, w)); // b
			v = cross(w, u); // v 
			lower_left_corner = origin - w*distance - half_width*u*distance - distance*half_height*v;
			horizontal = 2*half_width*u*distance;
			vertical = 2*half_height*v*distance;
		}

		ray get_ray (float s, float t) { 
			vec3 rd = lens_radius*random_in_unit_disk();
			vec3 offset = u*rd.x + v*rd.y;
			return ray(origin+offset, lower_left_corner + s*horizontal + t*vertical - origin-offset); std::cout << lower_left_corner; }

		vec3 origin;
		vec3 lower_left_corner;
		vec3 horizontal;
		vec3 vertical;
		vec3 u, v, w;
		float lens_radius;
		
};

#endif
