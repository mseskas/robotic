#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <vector>
#include <GL/glut.h>


#include <fstream>
#include <iostream>
#include <sstream>

#include "glm/glm.hpp"

#include "movable.h"

using namespace std;


struct part_with_material
{
    vector< vector<int> > faces;


    string material_name;
};

struct material
{
    string name;
    float diffuse[4];   // Kd
    float ambient[4];  // Ka
    float specular[4];  // Ks
    float shininess; // Ns

};

class obj_loader : public movable
{
    public:
        obj_loader(string file_name);

        void print();

        void load_object(string filename);
        void load_material(string material_filename);

        void print_material(string material_name);

        vector<glm::vec4> vertices;
        vector<glm::vec3> norml_vectors;


        void set_scale(float x, float y, float z);

        void rotate(float dif_x_deg, float dif_y_deg, float dif_z_deg);


        vector< part_with_material> parts;
        vector< material > mats;
};

#endif // OBJ_LOADER_H
