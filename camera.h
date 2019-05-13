#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera {
    public:
        camera() {
            lower_left_corner = vec3(-5.0, -5.0, -1.0);
            horizontal = vec3(10.0,0.0,0.0);
            vertical = vec3(0.0,10.0,0.0);
            origin = vec3(0.0,0.0,0.0);
        }

        ray get_ray (float u, float v) { return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin); }

        vec3 origin;
        vec3 lower_left_corner;
        vec3 vertical;
        vec3 horizontal;
};

#endif