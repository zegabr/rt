#ifndef PLANEH
#define PLANEH
#include "hitable.h"
#include "phongmaterial.h"


class plane : public hitable
{
public:
    plane(vec3 p0, vec3 p1, vec3 p2, phongMaterial mat) : 
	material(mat),
	P0(p0),
	P1(p1),
	P2(p2),
	normal(unit_vector(cross(p0 - p1, p0 - p2))),
	area(size(cross(p0 - p1, p0 - p2)))
{}
	~plane(){};

	bool 		intersect(IntersectionData* id, Ray r) const;
	vec3 	getPointOnSurface(float u, float v) const;
	float 		getArea() const;
	vec3 	getNormal() const;
	vec3 	getFirstTangent() const;

    vec3 P0, P1, P2, V1, V2, normal;
    float area;
    phongMaterial material;
};


bool plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	// Möller–Trumbore intersection algorithm

	vec3 e1, e2;  //Edge1, Edge2
	vec3 P, Q, T;
	float det, inv_det, u, v;
	float t;

	// Find vectors for two edges sharing P0
	e1 = P1 - P0;
	e2 = P2 - P0;
	// Begin calculating determinant - also used to calculate u parameter
	P = cross(r.direction, e2);
	// if determinant is near zero, ray lies in plane of triangle
	det = dot(e1, P);
	// NOT CULLING
	if(det > -0.00001 && det < 0.00001) return false;
		inv_det = 1.f / det;

	// calculate distance from P0 to ray origin
	T = r.origin - P0;
	Q = cross(T, e1);

	// Calculate u parameter and test bound
	u = dot(T, P) * inv_det;
	v = dot(r.direction, Q) * inv_det;

	// The intersection lies outside of the plane
	if(u < 0.f || u > 1.f || v < 0.f || v > 1.f) return false;

	t = dot(e2, Q) * inv_det;

	if(t > 0.00001) { //ray intersection
        rec.t = t;
        rec.normal = normalize(cross(e1, e2));
        rec.material = material();
        return true;
	}

	// No hit, no win
	return false;
}