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

vec3 PRETO = vec3(0.0,0.0,0.0), BRANCO = vec3(1.0,1.0,1.0);

vec3 phong(const hit_record &hitou, const camera &cam, const hitable_list *world, int soft){


	vec3 n,l,r,v;
	float attenuation,distance;
	float alpha = hitou.material.alpha*128; // isso aqui é o alpha do material, ele deve ir de 0.0 a 1.0 preferencialmente

	vec3 emissive = vec3(0.0,0.0,0.0);
	vec3 diffuse = vec3(0.0,0.0,0.0);
	vec3 specular = vec3(0.0,0.0,0.0);
	if(soft==0){//usa luzes pontuais inicializadas descritas na cin
		for(int i = 0;i<world->numLights;i++) {
			hit_record h;
			if(world->hit(ray(hitou.p,world->lights[i].position - hitou.p), 0.001, FLT_MAX, h)) { // entra nesse if se houver interseção entre o ponto e a luz i.
				diffuse += vec3(0,0,0);
				specular += vec3(0,0,0);
			} else {

				distance = vec3(world->lights[i].position - hitou.p).size();
				//attenuation = 1/(distance);
				l = unit_vector(world->lights[i].position - hitou.p); // direção da luz
				n = unit_vector(hitou.normal); // normal no ponto que hitou
				v = unit_vector(cam.origin - hitou.p); // view direction

				r = 2*dot(l,n)*n - l; // pega o raio refletido pela luz
				float vr = dot(v,r), cosine =  max(dot(n,l), 0.0f); // pega o cosseno entre n e l

				if(cosine > 0.0) {
					diffuse = diffuse + hitou.material.Kd * world->lights[i].color * cosine;
					specular = specular + hitou.material.Ks * world->lights[i].color*pow(max(0.0f, vr),alpha);

				}

			}

			emissive += hitou.material.Ke * world->lights[i].color/world->numLights;
		}
	}else{//usa plano luminoso
		// monte carlo aqui
		vec3 vy = world->planelight.p2 - world->planelight.p0;//vetor representativo de um dos lados
		vec3 vx = world->planelight.p1 - world->planelight.p0;//vetor representativo do outro lado
		vec3 diffuseaux = vec3(0,0,0);
		vec3 specularaux = vec3(0,0,0);
		for(int i=0;i<soft;i++){
			float uu = drand48();//valores entre 0 e 1
			float vv = drand48();//valores entre 0 e 1

			vec3 ligpos = world->planelight.p0 + uu*vy + vv*vx;//ponto no plano

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

				if(cosine > 0.0) {
					diffuseaux += hitou.material.Kd * world->planelight.material.color * cosine;
					specularaux += hitou.material.Ks * world->planelight.material.color*pow(max(0.0f, vr),alpha);
				}
			}
		}

		diffuse += diffuseaux/soft;//media difusa
		specular += specularaux/soft;//media especular
		emissive += world->planelight.material.Ke *hitou.material.Ke/soft;

	}

	return  emissive + diffuse + specular;

}

bool scatter(const ray& r_in, const hit_record& rec, ray& scattered) {
	vec3 reflected = reflect(unit_vector(r_in.direction()),rec.normal);
	const float ERR = 1e-4;
	scattered = ray(rec.p +rec.normal*ERR, reflected);
	return(dot(scattered.direction(),rec.normal)> 0);
}

vec3 color(const ray& r, const hitable_list *world, const camera &cam, int soft, const int depth){
	hit_record rec;
	if(world->hit(r,0.00000001,FLT_MAX,rec)){ // se acertar algum objeto da imagem, entra nesse if
		ray scattered;
		if(depth < 20) { // máx 20 interações 
			if(rec.material.reflective && scatter(r,rec,scattered)  ) { // entra aqui se o material for refletível
				vec3 attenuation = color(scattered,world,cam,soft,depth+1); // pega a cor do próx material
				return attenuation*rec.material.Ks + phong(rec,cam,world,soft); // retorna a reflexão + a cor do mat
			} 
		} 
		return phong(rec,cam,world,soft);
	} else {
		return vec3(0,0,0);
	}

}


int main(){
	ios::sync_with_stdio(0); cin.tie(0);
	int W, H ,ns = 70; // precisão do antialiasing 
	int soft = 0;//QUANTIDADE DE ITERACOES NO SOFT SHADOWS
	//=====soft eh agr passado como parametro em cor e phong, pra ficar mais facil inicializar aqui
	camera cam;
	plane lightplane;
	string output;


	
	string action;
	map<string,phongMaterial> material_dictionary;
	vector<sphere> objects;
	vector<phongLight> lights;

	while(cin >> action){
		if(action == "outputfile"){
			cin >> output;//nome do arquivo de saida
		}else if(action == "res"){
			cin >> H >> W;
		}else if(action == "camera"){
			float px,py,pz,tx,ty,tz,ux,uy,uz,fov,aperture,dist;
			cin >> px >> py >> pz >> tx >> ty >> tz >> ux >> uy >> uz >> fov >>aperture >> dist;//dist = distancia focal
			cam = camera(vec3(px,py,pz), vec3(tx,ty,tz), vec3(ux,uy,uz), fov, float(W)/float(H), aperture      ,dist);
			// camera:     origem               alvo           upvector   fov         aspectratio   abertura    distancia focal

		}else if(action == "material"){
			float r, g, b, kd, ks, ke, alpha;
			string name;
			bool reflect;
			cin>> name >> r >> g >> b >> ke >> kd >> ks >> alpha >> reflect;
			material_dictionary[name] = phongMaterial(vec3(r,g,b),ke,kd,ks,alpha,reflect);
		}else if(action == "sphere"){
			float cx, cy, cz, radius;
			string materialname;
			cin >> cx >> cy >> cz >> radius >> materialname;
			objects.push_back(sphere(vec3(cx,cy,cz), radius, material_dictionary[materialname])); 
		}else if(action == "light"){

			float r, g, b,px,py,pz;
			cin >> r >> g >> b >> px >> py >> pz;
			lights.push_back(phongLight(vec3(r,g,b), vec3(px,py,pz)));

		}else if(action == "planelight"){
			float ax,ay,az,bx,by,bz,cx,cy,cz;
			cin >> ax >> ay >> az >> bx >> by >> bz >> cx >> cy >> cz >> soft;
			lightplane = plane(vec3(ax,ay,az),vec3(bx,by,bz),vec3(cx,cy,cz));

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

	hitable_list *world = new hitable_list(list,QUANTIDADE,lightList,numLights,lightplane); // objeto que tem todas as imagens
	
	
	ofstream out(output);//arquivo resultado
	out<<"P3"<<'\n'<<W<<'\n'<<H<<'\n'<<"255"<<'\n'; 
	for(int j = H-1; j >= 0; j--){ // começa a preencher a imagem de cima para baixo
		for(int i = 0; i < W; i++){ // e da esquerda para a direita
			vec3 col(0,0,0); 
			for(int s = 0;s < ns;s++) { // for do anti-aliasing: recomendo ler sobre para entender
				float u = float(i + drand48()) / float(W);
				float v = float(j + drand48()) / float(H);
				ray r = cam.get_ray(u,v);
				col += color(r,world,cam,soft, 0);
			}
			col /= float(ns);
			col = vec3(col.x,col.y,col.z); // serve pra ajustar a gamma de cores para visualizadores de imagem
			int ir = int(255.99*col.x);  // vermelho do pixel
			int ig = int(255.99*col.y); // verde do pixel
			int ib = int(255.99*col.z); // azul do pixel
			out<<ir<<" "<<ig<<" "<<ib<<"\n";
		}
	}
	cout << "output: " << output << endl;
} 
