#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera {
    public:
        camera( float field_of_view,float zplane, float aspect) { // field of view é vertical, não horizontal(depois pesquisar horizontal)
            vec3 u, v, w;
            float theta = field_of_view*M_PI/180;
            float half_height = tan(theta/2);
            float half_width = aspect * half_height;
            lower_left_corner = vec3(-half_width, -half_height, zplane);
            horizontal = vec3(2*half_width,0.0,0.0);
            vertical = vec3(0.0,2*half_height,0.0);
            origin = vec3(0.0,0.0,0.0);
        }

        ray get_ray (float u, float v) { return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin); }

        vec3 origin;
        vec3 lower_left_corner;
        vec3 vertical;
        vec3 horizontal;
};

#endif