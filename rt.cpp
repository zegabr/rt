#include<iostream>
#include<fstream>
#include<cmath>
#include<algorithm>
#include<vector>
#include<map>
#include "sphere.h"
#include "plane.h"
#include "hitablelist.h"
#include "float.h"
#include "camera.h"
using namespace std;
#define endl '\n'


vec3 PRETO = vec3(0.0,0.0,0.0), BRANCO = vec3(1.0,1.0,1.0);

vec3 phong(const hit_record &hitou, const camera &cam, const hitable_list *world, int soft){


	vec3 n,l,r,v;
	float attenuation,distance;
	float alpha = hitou.material.alpha*128; // isso aqui é o alpha do material, ele deve ir de 0.0 a 1.0 preferencialmente

	vec3 ambient = vec3(0.0,0.0,0.0);
	vec3 diffuse = vec3(0.0,0.0,0.0);
	vec3 specular = vec3(0.0,0.0,0.0);
	if(soft==0){//usa luzes pontuais inicializadas descritas na cena
		for(int i = 0;i<world->numLights;i++) {
			hit_record h;
			if(world->hit(ray(hitou.p,world->lights[i].position - hitou.p), 0.001, FLT_MAX, h)) { // entra nesse if se houver interseção entre o ponto e a luz i.
				diffuse += vec3(0,0,0);
				specular += vec3(0,0,0);
			} else {

				distance = vec3(world->lights[i].position - hitou.p).size();
				attenuation = 1/(distance);
				l = unit_vector(world->lights[i].position - hitou.p); // direção da luz
				n = unit_vector(hitou.normal); // normal no ponto que hitou
				v = unit_vector(cam.origin - hitou.p); // view direction

				r = 2*dot(l,n)*n - l; // pega o raio refletido pela luz
				float vr = dot(v,r), cosine =  max(dot(n,l), 0.0f); // pega o cosseno entre n e l

				if(cosine > 0.0) {
					diffuse = diffuse + hitou.material.Kd * world->lights[i].color * cosine * attenuation;
					specular = specular + hitou.material.Ks * world->lights[i].color*pow(max(0.0f, vr),alpha)*attenuation;

				}

			}

			ambient += hitou.material.Ka * world->lights[i].color/world->numLights;
		}
	}else{//usa plano luminoso
		// monte carlo aqui
		vec3 vy = world->planelight.p2 - world->planelight.p0;//vetor representativo de um dos lados
		vec3 vx = world->planelight.p1 - world->planelight.p0;//vetor representativo do outro lado
		int Y = (int)vy.size();//aproximacao inteira do tamanho dos lados
		int X = (int)vx.size();
		//cout<<X<<' '<<Y<<endl;
		vec3 diffuseaux = vec3(0,0,0);
		vec3 specularaux = vec3(0,0,0);
		for(int i=0;i<soft;i++){
			float uu = drand48();//valores entre 0 e 1
			float vv = drand48();//valores entre 0 e 1
			//cout << uu << ' ' << vv << endl;//soh pra testar se uu e vv tao entre zero e o tamanho dos lados do planoluz

			vec3 ligpos = world->planelight.p0 + uu*vy + vv*vx;//ponto no plano
			//attenuation = 1/((ligpos - hitou.p).size());

			hit_record h2;
			if(world->hit(ray(hitou.p,ligpos - hitou.p), 0.001, FLT_MAX, h2)){//acertoualgo =sombra
				diffuseaux += vec3(0,0,0);
				specularaux += vec3(0,0,0);
			}else{
				l = unit_vector(ligpos - hitou.p); // direção da luz
				n = unit_vector(hitou.normal); // normal no ponto que hitou
				v = unit_vector(cam.origin - hitou.p); // view direction

				r = 2*dot(l,n)*n - l; // pega o raio refletido pela luz
				float vr = dot(v,r), cosine =  max(dot(n,l), 0.0f); // pega o cosseno entre n e l

				if(cosine > 0.0) {//AQUI
					diffuseaux += hitou.material.Kd * world->planelight.material.color * cosine /**attenuation*/;
					specularaux += hitou.material.Ks * world->planelight.material.color*pow(max(0.0f, vr),alpha) /**attenuation*/;
				}
			}
		}

		diffuse += diffuseaux/soft;//media difusa
		specular += specularaux/soft;//media especular
		ambient += world->planelight.material.Ka *hitou.material.Ka/soft;

	}


	return  ambient + diffuse + specular;

}

vec3 color(const ray& r, const hitable_list *world, const camera &cam, int soft){
	hit_record rec;
	if(world->hit(r,0.00000001,FLT_MAX,rec)){ // se acertar algum objeto da imagem, entra nesse if
		return phong(rec,cam,world, soft);
	}
	else{
		return vec3(0.0,0.0,0.0);
	}
}


int main(){
	ios::sync_with_stdio(0); cin.tie(0);

	int W, H ,ns = 200; // precisão do antialiasing 
	int soft=30;//QUANTIDADE DE ITERACOES NO SOFT SHADOWS
	//=====soft eh agr passado como parametro em cor e phong, pra ficar mais facil inicializar aqui
	camera cam;

	fstream cena;
	string input = "cenaze.txt";
	string output = "zesoft.ppm";
	cena.open(input);//arquivo descricao
	ofstream out(output);//arquivo resultado

	string action;
	map<string,phongMaterial> material_dictionary;
	vector<sphere> objects;
	vector<phongLight> lights;

	while(cena >> action){
		if(action == "res"){
			cena >> H >> W;
		}else if(action == "camera"){
			float px,py,pz,tx,ty,tz,ux,uy,uz,fov,aperture,dist;
			cena >> px >> py >> pz >> tx >> ty >> tz >> ux >> uy >> uz >> fov >>aperture/* >> f*/;//ver esse f, provavel ser o depth of field
			// camera: 1 parametro é a posição da camera, segundo é o alvo, terceiro é o vetor up, quarto é o fov (vertical), quinto é o aspect/ratio
			vec3 direction(tx-px,ty-py,tz-pz);
			dist = direction.size();
			cam = camera(vec3(px,py,pz), vec3(tx,ty,tz), vec3(ux,uy,uz), fov, float(W)/float(H), aperture ,dist);
		}else if(action == "material"){
			float r, g, b, kd, ks, ka, alpha;
			string name; 
			cena>> name >> r >> g >> b >> ka >> kd >> ks >> alpha;
			material_dictionary[name] = phongMaterial(vec3(r,g,b),ka,kd,ks,alpha);
		}else if(action == "sphere"){
			float cx, cy, cz, radius;
			string materialname;
			cena >> cx >> cy >> cz >> radius >> materialname;
			objects.push_back(sphere(vec3(cx,cy,cz), radius, material_dictionary[materialname])); 
		}else if(action == "light"){

			float r, g, b,px,py,pz;
			cena >> r >> g >> b >> px >> py >> pz;
			lights.push_back(phongLight(vec3(r,g,b), vec3(px,py,pz)));

		}else if(action == "planelight"){

		}
	}

	int QUANTIDADE = objects.size();//tamanho do vector de esferas 
	int numLights = lights.size();

	cout<<QUANTIDADE<<" esferas"<<endl;
	cout<<material_dictionary.size()<<" materiais"<<endl;
	cout<<numLights<<" luzes"<<endl;

	hitable *list[QUANTIDADE]; // array de objetos na imagem (inicializar com as esferas)
	for(int i=0;i<QUANTIDADE;i++){
		sphere aux  = objects[i];
		list[i]=new sphere(aux.center, aux.radius, aux.material);
	}

	phongLight lightList[numLights];
	for(int i=0;i<numLights;i++){
		phongLight aux  = lights[i];
		lightList[i]= phongLight(aux.color, aux.position);
	}

	//=====================PLANO LUMINOSO AQUI==========================
	plane lightplane(vec3(0,3,0),vec3(5,3,0),vec3(0,2,5));
	hitable_list *world = new hitable_list(list,QUANTIDADE,lightList,numLights,lightplane); // objeto que tem todas as imagens



	out<<"P3"<<'\n'<<W<<'\n'<<H<<'\n'<<"255"<<'\n'; 
	for(int j = H-1; j >= 0; j--){ // começa a preencher a imagem de cima para baixo
		for(int i = 0; i < W; i++){ // e da esquerda para a direita
			vec3 col(0,0,0); 
			for(int s = 0;s < ns;s++) { // for do anti-aliasing: recomendo ler sobre para entender
				float u = float(i + drand48()/*random_digit()*/) / float(W);
				float v = float(j + drand48()/*random_digit()*/) / float(H);
				ray r = cam.get_ray(u,v);
				col += color(r,world,cam,soft);
			}
			col /= float(ns);
			col = vec3(col.x,col.y,col.z); // serve pra ajustar a gamma de cores para visualizadores de imagem
			int ir = int(255.99*col.x);  // vermelho do pixel
			int ig = int(255.99*col.y); // verde do pixel
			int ib = int(255.99*col.z); // azul do pixel
			out<<ir<<" "<<ig<<" "<<ib<<"\n";
		}
	}
	cout << "scene: " << input << endl << "output: " << output << endl;
} 
