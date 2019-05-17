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

vec3 phong(const hit_record &hitou, const camera &cam, phongLight light){
    vec3 n,l,r,v;
    

    l = (light.position - hitou.p); // direção da luz
    n = hitou.normal; // normal no ponto que hitou
    v = (cam.origin - hitou.p); // view direction

    vec3 b = unit_vector(l);
    l.normalize();
    v.normalize();
    n.normalize();

    r = 2*dot(l,n)*n - l; // pega o raio refletido pela luz

    float vr = dot(v,r), cosine =  max(dot(n,l), 0.0f); // pega o cosseno entre n e l

    vec3 Ka = hitou.material.color*hitou.material.Ka;
    vec3 Kd = hitou.material.color*hitou.material.Kd;
    vec3 Ks = hitou.material.color*hitou.material.Ks;
    float exponent = 128.0;

    vec3 ambient = light.color*Ka;
    vec3 diffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);
    
    if(cosine > 0.0) {
       diffuse = Kd * light.color * cosine;
       specular = Ks*light.color*pow(max(0.0f, vr),exponent);
    }
    return  hitou.material.alpha*(ambient + diffuse + specular);

}

vec3 color(const ray& r, hitable *world, const camera &cam, const phongLight &light){
    hit_record rec;
    if(world->hit(r,0.0000001,FLT_MAX,rec)){ // se acertar algum objeto da imagem, entra nesse if
        hit_record h;
        if(world->hit(ray(rec.p + 1.00001*unit_vector(light.position), unit_vector(light.position)), 0.0000001,FLT_MAX, h)) {
                return vec3(0.0,0.0,0.0);
        }
        return phong(rec,cam,light);
    }
    else{
        return vec3(0.0,0.0,0.0);
    }
}

//
int main(){
    const int W = 500; // tamanho horizontal da tela
    const int H = 500; // tamanho vertical da tela
    int ns = 50; // precisão do antialiasing
    ofstream out("teste.ppm");//arquivo resultado
    out << "P3" << '\n' << W << '\n' << H << '\n' << "255" << '\n'; 
    hitable *list[4]; // array de objetos na imagem
    list[0] = new sphere(vec3(0.0,0.5,-1.0),0.7, phongMaterial(vec3(0.0,0.0,1.0), 0.2, 0.5, 0.6, 1.0)); // esfera do centro
    list[1] = new sphere(vec3(0.0,-1000.5,-1.0),1000.0, phongMaterial(vec3(0.0,1.0,0.0), 0.2, 0.5, 0.6, 0.8)); // esfera do "chão"
    list[2] = new sphere(vec3(-1.0,0.5,-1.0), 0.01, phongMaterial(vec3(1.0,1.0,1.0),1.0,1.0,1.0,1.0));
    list[3] = new sphere(vec3(1.0,0.5,-1.0),0.7, phongMaterial(vec3(1.0,0.0,1.0), 0.2, 0.5, 0.6, 1.0)); // esfera do centro
    hitable *world = new hitable_list(list,4); // objeto que tem todas as imagens
    phongLight light(vec3(1.0,1.0,1.0), vec3(-1.0,0.5,-1.0)); // 1 parametro é a cor, segundo é a posição
    camera cam(vec3(-3.0,3.0,-3.0), vec3(0.0,0.0,0.0), vec3(0.0,1.0,0.0), 90, float(W)/float(H));
    // camera: 1 parametro é a posição da camera, segundo é o alvo, terceiro é o vetor up, quarto é o fov (vertical), quinto é o aspect/ratio
    for(int j = H-1; j >= 0; j--){ // começa a preencher a imagem de cima para baixo
        for(int i = 0; i < W; i++){ // e da esquerda para a direita
            vec3 col(0,0,0); 
            for(int s = 0;s < ns;s++) { // for do anti-aliasing: recomendo ler sobre para entender
                float u = float(i + drand48()/*random_digit()*/) / float(W);
                float v = float(j + drand48()/*random_digit()*/) / float(H);
                ray r = cam.get_ray(u,v);
                col += color(r,world,cam,light);
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
