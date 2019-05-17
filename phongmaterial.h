#ifndef PHONGMATERIALH
#define PHONGMATERIALH


class phongMaterial {
    public:
        phongMaterial() {}
        phongMaterial(vec3 c, float a, float d, float s, float alp) : color(c), Ka(a), Ks(s), Kd(d), alpha(alp) {}

        vec3 color;
        float Ka;
        float Ks;
        float Kd;
        float alpha;

};


class phongLight {
    public:
        phongLight() {}
        phongLight(vec3 c, vec3 p) : color(c), position(p) {}

        vec3 color;
        vec3 position;
};
#endif
