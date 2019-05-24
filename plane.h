//https://github.com/kbladin/Monte_Carlo_Ray_Tracer/blob/master/src/Object3D.cpp
#ifndef PLANEH
#define PLANEH
#include "hitable.h"
#include "material.h"
#include "phongmaterial.h"

class plane: public hitable {
    public:
    plane(){}
    plane(vec3 a, vec3 b, vec3 c) : 
        p0(a),
        p1(b),
        p2(c), 
        material(phongMaterial(vec3(1.5,1.5,1.5), 1,1,1,200)),
        normal(  unit_vector((cross(p0 - p1, p0 - p2)))),
        area(cross(p0 - p1, p0 - p2).size())
        {};

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    vec3 p0,p1,p2;//3 pontos onde o 1o será considerada a "origem" do plano
    phongMaterial material;
    vec3 normal;
    float area;
	
};
	bool plane::hit(const ray& r,float t_min, float t_max, hit_record& rec) const{
		// Möller–Trumbore intersection algorithm

		vec3 e1, e2;  //Edge1, Edge2
		vec3 P, Q, T;
		float det, inv_det, u, v;
		float t;

		// Find vectors for two edges sharing p0
		e1 = p1 - p0;
		e2 = p2 - p0;
		// Begin calculating determinant - also used to calculate u parameter
		P = cross(r.direction(), e2);
		// if determinant is near zero, ray lies in plane of triangle
		det = dot(e1, P);
		// NOT CULLING
		if(det > -0.00001 && det < 0.00001) return false;
			inv_det = 1.f / det;

		// calculate distance from p0 to ray origin
		T = r.origin() - p0;
		Q = cross(T, e1);

		// Calculate u parameter and test bound
		u = dot(T, P) * inv_det;
		v = dot(r.direction(), Q) * inv_det;

		// The intersection lies outside of the plane
		if(u < 0.f || u > 1.f || v < 0.f || v > 1.f) return false;

		t = dot(e2, Q) * inv_det;

		if(t > 0.00001) { //ray intersection
			rec.t = t;
			rec.normal = unit_vector(cross(e1, e2));
			rec.material = material;
			return true;
		}

		// No hit, no win
	return false;
}

#endif
