#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "world.h"
#include "shaders/phong_shader.h"
#include "shaders/flat_shader.h"
#include "objects/sphere.h"
#include "objects/plane.h"
#include "objects/mesh.h"
#include "lights/point_light.h"
#include "lights/direction_light.h"


template<class T>
T get(std::unordered_map<std::string, T> m, std::string s) {
    auto r = m.find(s);
    assert(r != m.end());
    return m[s];
}

void parse_scene(World& world, int width, int height, double ar, const char* fn) {
    std::ifstream f(fn);
    std::string b;

    std::unordered_map<std::string, vec3> colors;
    std::unordered_map<std::string, Shader*> shaders;

    while (std::getline(f, b)) {
        std::stringstream ss(b);
        std::string entry, name, s0,s1,s2;
        vec3 u,v,w;
        double d0,d1;

        if (!(ss>>entry) || !entry.size() || entry[0]=='#') continue;

        if (entry=="color") {
            ss>>name>>u;
            assert(ss);
            colors[name] = u;
        } else if (entry=="sphere") {
            ss>>u>>d0>>s0;
            assert(ss);
            Object* o = new Sphere(u, d0);
            o->shader = get(shaders, s0);
            world.objects.push_back(o);
        } else if (entry=="plane") {
            ss>>u>>v>>s0;
            assert(ss);
            Object* o = new Plane(u,v);
            o->shader = get(shaders, s0);
            world.objects.push_back(o);
        } else if (entry=="mesh") {
            ss>>u>>d0>>d1>>s0>>s1;
            assert(ss);
            Object* o = new Mesh(s0.c_str(), u, (pi/180)*d0, (pi/180)*d1);
            o->shader = get(shaders, s1);
            world.objects.push_back(o);  
        } else if (entry=="flat_shader") {
            ss>>name>>s0;
            assert(ss);
            shaders[name] = new Flat_Shader(world, get(colors, s0));
        } else if (entry=="phong_shader") {
            ss>>name>>s0>>s1>>s2>>d0;
            assert(ss);
            vec3 c0 = get(colors, s0);
            vec3 c1 = get(colors, s1);
            vec3 c2 = get(colors, s2);
            shaders[name] = new Phong_Shader(world, c0, c1, c2, d0);
        } else if (entry=="point_light") {
            ss>>u>>s0>>d0;
            assert(ss);
            Light* l = new Point_Light(u, get(colors, s0), d0);
            world.lights.push_back(l);
        } else if (entry=="direction_light") {
            ss>>u>>s0>>d0;
            assert(ss);
            Light* l = new Direction_Light(u, get(colors, s0), d0);
            world.lights.push_back(l);
        } else if (entry=="ambient_light") {
            ss>>s0>>d0;
            assert(ss);
            world.ambient_color = get(colors, s0);
            world.ambient_intensity = d0;
        } else if (entry=="camera") {
            ss>>u>>v>>w>>d0;
            assert(ss);
            world.camera.set_pos_and_aim(u,v,w);
            world.camera.focus(1, (double(width)/height) * ar,
                    d0*(pi/180));
        } else if (entry=="background") {
            ss>>s0;
            assert(ss);
            world.background_shader = get(shaders, s0);
        } else if (entry=="recursion_depth") {
            ss>>world.recursion_depth_limit;
            assert(ss);
        } else if (entry=="antialiasing") {
            ss>>world.samples;
            assert(ss);
        }else {
            std::cout << "Cannot parse " << b << std::endl;
            exit(1);
        }
    }
    if (!world.background_shader)
        world.background_shader = new Flat_Shader(world, {0,0,0});

    f.close();
}
