#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera {
    public:
        camera(vec3 position, vec3 target, vec3 vup, float field_of_view, float aspect) { // field of view é vertical, não horizontal(depois pesquisar horizontal)
            vec3 u, v, w;
            float theta = field_of_view*M_PI/180;
            float half_height = tan(theta/2);
            float half_width = aspect * half_height;
            origin = position;
            w = unit_vector(position-target); // t
            u = unit_vector(cross(vup, w)); // b
            v = cross(w, u); // v 
            lower_left_corner = origin - half_width*u - half_height*v - w;
            horizontal = 2*half_width*u;
            vertical = 2*half_height*v;
        }

        ray get_ray (float u, float v) { return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin); std::cout << lower_left_corner; }

        vec3 origin;
        vec3 lower_left_corner;
        vec3 vertical;
        vec3 horizontal;
};

#endif
