//#include<bits/stdc++.h> //inclui tudo mas compila mais lento
#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include"vec3.h"
#include"ray.h"
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
vec3 color(const ray& r){
    float t = hit_sphere (vec3(0,0,-1),0.5,r);//Em vc3, está a posição da horizontal, a altura, e a profundidade respectivamente.
    if (t > 0.0){
        vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0,0,-1));
        return 0.5*vec3(N.x+1,N.y+1,N.z+1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5*(unit_direction.y+1.0);
    return (1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5,0.7,1.0);
}





int main(){
    const int W = 1000;
    const int H = 500;
    ofstream out("out.ppm");//arquivo resultado
    out << "P3" << '\n' << W << '\n' << H << '\n' << "255" << '\n';
    vec3 lower_left_corner(-2.0,-1.0,-1.0);
    vec3 horizontal(4.0,0.0,0.0);
    vec3 vertical(0.0,2.0,0.0);
    vec3 origin(0.0,0.0,0.0);
    for(int j = H-1; j >= 0; j--){
        for(int i = 0; i < W; i++){
            float u = float(i)/float(W);
            float v = float(j)/float(H);
            ray r(origin, lower_left_corner + v*vertical + u*horizontal);
            vec3 col = color(r);
            int ir = int(255.99*col.x);
            int ig = int(255.99*col.y);
            int ib = int(255.99*col.z);

            out<<ir<<" "<<ig<<" "<<ib<<"\n";
        }
    }



}