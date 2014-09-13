#include "obj_loader.h"

obj_loader::obj_loader(string file_name) : movable()
{
    sc_x = 1;
    sc_y = 1;
    sc_z = 1;

    init(0,0,0,0,1,0);


    load_object(file_name);

    string material_file = file_name.substr(0, file_name.length() - 4);

    load_material(material_file.append(".mtl"));
}

void obj_loader::set_scale(float x, float y, float z)
{
    if ((x >= 0) && (y >=0) && (z>=0))
    {
        sc_x = x;
        sc_y = y;
        sc_z = z;
    }
}

void obj_loader::rotate(float dif_x_deg, float dif_y_deg, float dif_z_deg)
{
  //  angle_x += dif_x_deg;
  //  angle_y += dif_y_deg;
  //  angle_z += dif_z_deg;

    rotate_z(dif_z_deg);
    rotate_x(dif_x_deg);
}


void obj_loader::load_material(string material_filename)
{
    ifstream in(material_filename.c_str(), ios::in);
    if (!in) { cerr << "Cannot open " << material_filename << endl; exit(1); }

    string line;
    material curr_mater;

    while (getline(in, line)) {

        if (line.substr(0,6) == "newmtl")
        {
            curr_mater.name = line.substr(7); // first set name
        }
        else if (line.substr(0,3) == "Kd ")   // diffuse
        {
            line =  line.substr(3); // first set name

            istringstream iis(line);

            iis >> curr_mater.diffuse[0];
            iis >> curr_mater.diffuse[1];
            iis >> curr_mater.diffuse[2];

            //sscanf(line.c_str(), "%f %f %f\r", curr_mater.rgb.x, curr_mater.rgb.y, curr_mater.rgb.z);
            mats.push_back(curr_mater);
        }
        else if (line.substr(0,3) == "Ka ")   // diffuse
        {
            line =  line.substr(3);

            istringstream iis(line);

            iis >> curr_mater.ambient[0];
            iis >> curr_mater.ambient[1];
            iis >> curr_mater.ambient[2];

        }
        else if (line.substr(0,3) == "Ks ")   // diffuse
        {
            line =  line.substr(3);

            istringstream iis(line);

            iis >> curr_mater.specular[0];
            iis >> curr_mater.specular[1];
            iis >> curr_mater.specular[2];

        }
        else if (line.substr(0,3) == "Ns ")   // exponent of specular
        {
            line =  line.substr(3);

            istringstream iis(line);
            iis >> curr_mater.shininess; // Ns


        }

    }
}

void obj_loader::load_object(string filename)
{
    ifstream in(filename.c_str(), ios::in);
    if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }

    string line;

    part_with_material  curr_part =  part_with_material();

    while (getline(in, line)) {

        if (line.substr(0,2) == "v ")
        {
            istringstream s(line.substr(2));
            glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
            vertices.push_back(v);
        }
        else if (line.substr(0,3) == "vn ")
        {
            istringstream s(line.substr(3));
            glm::vec3 v;
            s >> v.x;
            s >> v.y;
            s >> v.z;
            norml_vectors.push_back(v);
        }
        else if (line.substr(0,2) == "f ")
        {
            vector<int> curr_face;

            std::istringstream iss(line.substr(2));
            std::string result;


            while( std::getline( iss, result , ' ') )
            {
                int no = strtol(result.c_str(), NULL, 10);
                curr_face.push_back(no-1);
            }

            if (curr_face.size() > 0) curr_part.faces.push_back(curr_face);

        }
        else if (line.substr(0,7) == "usemtl ")
        {
            if (curr_part.faces.size() == 0)
                curr_part.material_name = line.substr(7);
            else
            {
                parts.push_back(curr_part);
                curr_part =  part_with_material();
                curr_part.material_name = line.substr(7);
            }


        }
        else if (line[0] == '#') { /* ignoring comment line */ }
        else { /* ignoring this line */ }
    }
    if (curr_part.faces.size() > 0) // last part must be saved
        parts.push_back(curr_part);
}

void obj_loader::print_material(string material_name)
{
    int index = 0;
    for (; mats.size() > index; index++)
    {
        // string.compare() - return 0 if equels
        if ( material_name.compare(mats[index].name) == 0)
        {

            //glColor3f (mats[index].diffuse[0], mats[index].diffuse[1], mats[index].diffuse[2]);

            glMaterialfv(GL_FRONT, GL_DIFFUSE, mats[index].diffuse);
            glMaterialfv(GL_FRONT, GL_AMBIENT, mats[index].diffuse); // developer note : must be ambient
            glMaterialfv(GL_FRONT, GL_SPECULAR, mats[index].specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, &mats[index].shininess);  // developer note : may be array of one element
        }
    }
}


void obj_loader::print()
{
    int face_id = 0;
    int vert_id = 0;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

// glScalef(sc_x, sc_y,sc_z);


    glTranslatef(pos_x, pos_y, pos_z);

    movable::print();

    glRotatef(angle_x, 1.0, 0.0, 0.0);
    glRotatef(angle_y, 0.0, 1.0, 0.0);
    glRotatef(angle_z, 0.0, 0.0, 1.0);

    int part_id = 0;
    while (parts.size() > part_id)
    {
        part_with_material curr_part = parts[part_id];
        print_material(curr_part.material_name);
        face_id=0;
        while (curr_part.faces.size() > face_id)
        {
            vector<int> face = curr_part.faces[face_id];
            glBegin(GL_TRIANGLE_FAN);

            vert_id = 0;
            while (face.size() > vert_id)
            {
                    // set normal
                    glNormal3f( norml_vectors[face[vert_id]].x,
                    norml_vectors[face[vert_id]].y, norml_vectors[face[vert_id]].z);

                    glVertex3f( vertices[face[vert_id]].x*sc_x, vertices[face[vert_id]].y*sc_y,
                        vertices[face[vert_id]].z*sc_z);
                    vert_id++;
            }
            glEnd();
            face_id++;
        }
        part_id++;
    }
         glLoadIdentity();  // leave default 0,0,0 position
}




