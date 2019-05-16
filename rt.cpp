//#include<bits/stdc++.h> //inclui tudo mas compila mais lento
#include<iostream>
#include<fstream>
#include<cmath>
#include<algorithm>
#include "sphere.h"
#include "hitablelist.h"
#include "float.h"
#include "camera.h"
#include "material.h"
using namespace std;

vec3 PRETO = vec3(0.0,0.0,0.0), BRANCO = vec3(1.0,1.0,1.0);

vec3 phong(const hit_record &hitou, const camera &cam,  vec3 lightpos = vec3(1,2,1),  vec3 lightcolor = BRANCO){
    vec3 n,l,r,v;
    float vr, cosine;

    l = (lightpos - hitou.p); // direção da luz
    n = hitou.normal; // normal no ponto que hitou
    v = (cam.origin - hitou.p); // view direction


    l.normalize();
    v.normalize();
    n.normalize();

    r = 2*dot(l,n)*n - l; // pega o raio refletido pela luz

    vr = dot(v,r); // parte da especular
    cosine =  max(dot(n,l), 0.0f); // pega o cosseno entre n e l

    vec3 Ka = hitou.material.color*hitou.material.Ka;
    vec3 Kd = hitou.material.color*hitou.material.Kd;
    vec3 Ks = hitou.material.color*hitou.material.Ks;
    float exponent = 200.0;

    vec3 ambient = lightcolor*Ka;
    vec3 diffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);
    
    if(cosine > 0.0) {
        diffuse = Kd * lightcolor * cosine;
        specular = Ks*lightcolor*pow(max(0.0f, vr),exponent);
    }
    return  hitou.material.alpha*(ambient + diffuse + specular);

}

vec3 color(const ray& r, hitable *world, const camera &cam){
    hit_record rec;
    if(world->hit(r,0.0000001,FLT_MAX,rec)){ // se acertar algum objeto da imagem, entra nesse if
        return phong(rec,cam);
    }
    else{
        return vec3(0.0,0.0,0.0);
        vec3 unit_direction = unit_vector(r.direction());
        float t= 0.5*(unit_direction.y+1.0);
        return (1.0-t)*vec3(1.0,1.0,1.0)+t*vec3(0.5,0.7,1.0);
    }
}

//
int main(){
    const int W = 500; // tamanho horizontal da tela
    const int H = 500; // tamanho vertical da tela
    int ns = 100; // precisão do antialiasing
    ofstream out("teste.ppm");//arquivo resultado
    out << "P3" << '\n' << W << '\n' << H << '\n' << "255" << '\n'; 
    hitable *list[2]; // array de objetos na imagem
    list[0] = new sphere(vec3(0,0,-1),0.7, phongMaterial(vec3(0,0,1.0), 0.2, 0.5, 0.6, 1.0)); // esfera do centro
    list[1] = new sphere(vec3(0,-1000.5,-1),1000, phongMaterial(vec3(0,1.0,0), 0.2, 0.5, 0.6, 0.8)); // esfera do "chão"
    hitable *world = new hitable_list(list,2); // objeto que tem todas as imagens
    camera cam(vec3(-2,3,1), vec3(0,0,-1), vec3(0,1,0), 90, float(W)/float(H));
    // camera: 1 parametro é a posição da camera, segundo é o alvo, terceiro é o vetor up, quarto é o fov (vertical), quinto é o aspect/ratio
    for(int j = H-1; j >= 0; j--){ // começa a preencher a imagem de cima para baixo
        for(int i = 0; i < W; i++){ // e da esquerda para a direita
            vec3 col(0,0,0); 
            for(int s = 0;s < ns;s++) { // for do anti-aliasing: recomendo ler sobre para entender
                float u = float(i + drand48()/*random_digit()*/) / float(W);
                float v = float(j + drand48()/*random_digit()*/) / float(H);
                ray r = cam.get_ray(u,v);
                col += color(r,world,cam);
            }

            col /= float(ns);
            col = vec3(col.x,col.y,col.z); // serve pra ajustar a gamma de cores para visualizadores de imagem
            int ir = int(255.99*col.x);  // vermelho do pixel
            int ig = int(255.99*col.y); // verde do pixel
            int ib = int(255.99*col.z); // azul do pixel
            out << ir << " " << ig << " " << ib << "\n";
        }
    }

}        