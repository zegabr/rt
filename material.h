#ifndef MATERIALH
#define MATERIALH
#include <random>

struct hit_record;

float random_digit() { // windows não possui drand48(), então criei essa função,  mas a preferência é drand48().
    return (float(rand()) / float((RAND_MAX + 1.0))); // retorna float entre 0.0 e 1.0 (menos precisão que drand48())
}

#include "ray.h"
#include "hitable.h"
vec3 reflect(const vec3& v, const vec3& n){
    return v-2*dot(v,n)*n;
}
vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        // p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
        p = 2.0*vec3(random_digit(), random_digit(), random_digit()); // utilizar em windows
    } while (p.squared_size() >= 1.0);
    return p;
}
class material{
    public:
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};
class lambertian : public material{
    public:
        lambertian(const vec3& a) : albedo(a){}
        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const{
            vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = ray(rec.p, target-rec.p);
            attenuation = albedo;
            return true;
        }
        vec3 albedo;
};
class metal : public material {
    public:
        metal(const vec3& a): albedo(a){}
        virtual bool scatter(const ray& r_in,const hit_record& rec, vec3& attenuation, ray& scattered) const{
            vec3 reflected = reflect(unit_vector(r_in.direction()),rec.normal);
            scattered = ray(rec.p,reflected);
            attenuation = albedo;
            return (dot(scattered.direction(),rec.normal) > 0);
        }
        vec3 albedo;
};
#endif