#ifndef PHONGMATERIALH
#define PHONGMATERIALH


class phongMaterial {
	public:
		phongMaterial() {}
		phongMaterial(vec3 c, float e, float d, float s, float alp, bool r) : color(c), alpha(alp), reflective(r) {
			Ke = c*e;
			Kd = c*d;
			Ks = c*s;
		}

		vec3 color;
		vec3 Ke;
		vec3 Ks;
		vec3 Kd;
		bool reflective;
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
