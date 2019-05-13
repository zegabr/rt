//#include<bits/stdc++.h> //inclui tudo mas compila mais lento
#include<iostream>
#include<fstream>
#include "sphere.h"
#include "hitablelist.h"
#include "float.h"
#include "camera.h"
using namespace std;

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    } while (p.squared_size() >= 1.0);
    return p;
}

vec3 color(const ray& r, hitable *world){
    hit_record rec;
    if(world->hit(r,0.0,FLT_MAX,rec)){
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5*color(ray(rec.p, target-rec.p), world);
    }
    else{
        vec3 unit_direction = unit_vector(r.direction());
        float t= 0.5*(unit_direction.y+1.0);
        return (1.0-t)*vec3(1.0,1.0,1.0)+t*vec3(0.5,0.7,1.0);
    }
}


int main(){
    const int W = 500; // tamanho horizontal da tela
    const int H = 500; // tamanho vertical da tela
    int ns = 100; // precisão do antialiasing
    ofstream out("teste.ppm");//arquivo resultado
    out << "P3" << '\n' << W << '\n' << H << '\n' << "255" << '\n'; 
    hitable *list[2]; // array de objetos na imagem
    list[0] = new sphere(vec3(0,0,-1),0.7); // esfera do centro
    list[1] = new sphere(vec3(0,-100.5,-1),100); // esfera do "chão"
    hitable *world = new hitable_list(list,2); // objeto que tem todas as imagens 
    camera cam; // camera
    for(int j = H-1; j >= 0; j--){ // começa a preencher a imagem de cima para baixo
        for(int i = 0; i < W; i++){ // e da esquerda para a direita
            vec3 col(0,0,0); 
            for(int s = 0;s < ns;s++) { // for do anti-aliasing: recomendo ler sobre para entender
                float u = float(i + drand48()) / float(W);
                float v = float(j + drand48()) / float(H);
                ray r = cam.get_ray(u,v);
                vec3 p = r.point_at_parameter(1.0); // não entendi o que é, ta no código do cara
                col += color(r,world);
            }
            col /= float(ns);
            int ir = int(255.99*col.x);
            int ig = int(255.99*col.y);
            int ib = int(255.99*col.z);
            out<<ir<<" "<<ig<<" "<<ib<<"\n"; 
        }
    }

}        