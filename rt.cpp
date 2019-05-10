//#include<bits/stdc++.h> //inclui tudo mas compila mais lento
#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include"vec3.h"
#include"ray.h"
using namespace std;
vec3 color(const ray& r){
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y+1.0);
    return (1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5,0.7,1.0);
}





int main(){
    const int W = 500;
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