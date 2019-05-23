#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"
#include "plane.h"
class hitable_list: public hitable{
    public:
        hitable_list(){}
        hitable_list(hitable **l,int n, phongLight *li, int m, plane plu) {list= l ;numObjetos=n;lights = li;numLights = m; planelight= plu; }
        virtual bool hit(const ray& r,float tmin,float tmax,hit_record& rec)const;
        hitable **list;
	    phongLight *lights;
        plane planelight;
        int numObjetos;
	    int numLights;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for(int i = 0; i<numObjetos; i++){
        if (list[i]->hit(r,t_min,closest_so_far,temp_rec)){
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
};
#endif