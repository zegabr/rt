out:rt.cpp camera.h hitable.h hitablelist.h material.h phongmaterial.h ray.h sphere.h vec3.h
	g++ rt.cpp -std=c++11 -Wfatal-errors -o out
	./out
