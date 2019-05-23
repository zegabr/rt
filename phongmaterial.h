#ifndef PHONGMATERIALH
#define PHONGMATERIALH


class phongMaterial {
    public:
        phongMaterial() {}
        phongMaterial(vec3 c, float a, float d, float s, float alp) : color(c), alpha(alp) {
            Ka = c*a;
            Kd = c*d;
            Ks = c*s;
        }

        vec3 color;
        vec3 Ka;
        vec3 Ks;
        vec3 Kd;
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
