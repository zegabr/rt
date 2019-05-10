//#include<bits/stdc++.h> //inclui tudo mas compila mais lento
#include<iostream>
#include<fstream>
#include "sphere.h"
#include "hitablelist.h"
#include "float.h"
using namespace std;
float hit_sphere(const vec3& center, float radius, const ray& r){ // t*t*dot(B,B) ​+ 2*t*dot( B,A - C ​ ) + dot( A-C,A -C ) - R*R = 0
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc,oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    if(discriminant < 0){
        return -1.0;
    }else{
        return (-b + sqrt(discriminant)) / (2.0*a);
    }
}
vec3 color(const ray& r, hitable *world){
    hit_record rec;
    if(world->hit(r,0.0,FLT_MAX,rec)){
        return 0.5*vec3(rec.normal.x+1,rec.normal.y+1,rec.normal.z+1);
    }
    else{
        vec3 unit_direction = unit_vector(r.direction());
        float t= 0.5*(unit_direction.y+1.0);
        return (1.0-t)*vec3(1.0,1.0,1.0)+t*vec3(0.5,0.7,1.0);
    }
}

int main(){
    const int W = 500;
    const int H = 500;
    ofstream out("ibagem.ppm");//arquivo resultado
    out << "P3" << '\n' << W << '\n' << H << '\n' << "255" << '\n';
    vec3 lower_left_corner(-5.0,-5.0,-1.0);
    vec3 horizontal(10.0,0.0,0.0);
    vec3 vertical(0.0,10.0,0.0);
    vec3 origin(0.0,0.0,0.0);
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1),0.5);
    list[1] = new sphere(vec3(0,-100.5,-1),100);
    hitable * world = new hitable_list(list,2);
    for(int j = H-1; j >= 0; j--){
        for(int i = 0; i < W; i++){
            float u = float(i)/float(W);
            float v = float(j)/float(H);
            ray r(origin, lower_left_corner + v*vertical + u*horizontal);

            vec3 p = r.point_at_parameter(2.0);
            vec3 col = color(r,world);
            int ir = int(255.99*col.x);
            int ig = int(255.99*col.y);
            int ib = int(255.99*col.z);

            out<<ir<<" "<<ig<<" "<<ib<<"\n";
        }
    }



}