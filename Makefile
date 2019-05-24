out:rt.cpp camera.h hitable.h hitablelist.h material.h phongmaterial.h ray.h sphere.h vec3.h plane.h
	g++ rt.cpp -std=c++11 -Wfatal-errors -O2 -o out
	./out
