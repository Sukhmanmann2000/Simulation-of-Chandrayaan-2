/*
  CSX75 Tutorial 6 Texturing a Cube


  Modified from An Introduction to OpenGL Programming, 
  Ed Angel and Dave Shreiner, SIGGRAPH 2013

  Written by Aditya Prakash, 2015
*/
//////////// LOAD TEXTURES
///////// DEBUG

#include "a3-scene.hpp"
#include "a3-scene_texture.hpp"
float width=600,height=600;
int tesselation=50; 
bool wireframe=false;
double PI=3.14159265;
double xPos,yPos;
bool mouseclicked = false;
std::vector<std::string> modelNames;
GLuint earth_shaderProgram,planetshaderProgram,launch_stand_shaderProgram,texshaderProgram,particleshaderProgram,sphereshaderProgram;
GLuint vbo[14], vao[14],moonvao,moonvbo;
GLuint earthtex,spacetex,sitebasetex,launchstandtex,armtex,redtex,conctex,basetex;
GLuint goldtex,paneltex,hoodtex,boosterhead,boosterbody,blaster,centraltex,smoketex,moontex;
float timeElapsed = 100.0*0;
glm::vec3 lookat_pt;
glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;


glm::mat4 modelview_matrix;
glm::mat3 normal_matrix;

GLuint uModelViewMatrix,uModelViewMatrix1,launch_stand_uModelViewMatrix,texuModelViewMatrix;
GLuint particlemat1,particlemat2,particlemat3,sphereumodelViewMatrix,spheretransMatrix;
GLuint particleColor;
GLuint viewMatrix,viewMatrix1;
GLuint normalMatrix,normalMatrix1;
GLuint shaderTime,earthRadius,innerRadius,outerRadius,shaderEye;
//-----------------------------------------------------------------

//6 faces, 2 triangles/face, 3 vertices/triangle
const int num_vertices = 25000;

int earth_tri_idx=0;
glm::vec4 earth_v_positions[num_vertices];
glm::vec4 earth_v_normals[num_vertices];
glm::vec4 earth_v_colors[num_vertices];
glm::vec2 earth_tex_coords[num_vertices];

int prev_tess = 30; 
int Lat = 10;
int Long = 10;

const int space_num_vertices = 36;
int space_tri_idx=0;
glm::vec4 space_v_positions[space_num_vertices];
glm::vec4 space_v_normals[space_num_vertices];
glm::vec2 space_tex_coords[space_num_vertices];

const int launch_stand_num_vertices = 550;
int launch_stand_tri_idx = 0;
glm::vec4 launch_stand_v_positions[launch_stand_num_vertices];
glm::vec2 launch_stand_tex_coords[launch_stand_num_vertices];

const int booster_num_vertices = 2000;
int booster1_tri_idx=0,booster2_tri_idx=0;
glm::vec4 booster1_v_positions[booster_num_vertices];
glm::vec2 booster1_tex_coords[booster_num_vertices];
glm::vec4 booster2_v_positions[booster_num_vertices];
glm::vec2 booster2_tex_coords[booster_num_vertices];

int down_tri_idx = 0;
glm::vec4 down_v_positions[booster_num_vertices];
glm::vec2 down_tex_coords[booster_num_vertices];

int central_tri_idx = 0;
glm::vec4 central_v_positions[booster_num_vertices];
glm::vec2 central_tex_coords[booster_num_vertices];

const int veh_num_vertices = 4000;
int veh_tri_idx=0;
glm::vec4 veh_v_positions[veh_num_vertices];
glm::vec2 veh_tex_coords[veh_num_vertices];

const int pay_cube_num_vertices = 10000;
int pay_cube_tri_idx=0;
glm::vec4 pay_cube_v_positions[pay_cube_num_vertices];
glm::vec2 pay_cube_tex_coords[pay_cube_num_vertices];

const int panel_num_vertices = 50;
int panel1_tri_idx=0,panel2_tri_idx=0;
glm::vec4 panel1_v_positions[panel_num_vertices];
glm::vec2 panel1_tex_coords[panel_num_vertices];
glm::vec4 panel2_v_positions[panel_num_vertices];
glm::vec2 panel2_tex_coords[panel_num_vertices];

glm::vec4 violet = glm::vec4(1.0,0.0,1.0,1.0);
glm::vec4 green = glm::vec4(0.0,1.0,0.0,1.0);
glm::vec4 red = glm::vec4(1.0,0.0,0.0,1.0);
glm::vec4 blue = glm::vec4(0.0,0.0,1.0,1.0);
glm::vec4 rg = glm::vec4(1.0,1.0 ,0.0,1.0);
glm::vec4 bg = glm::vec4(0.0,1.0,1.0,1.0);

  
void loadModelNames(){
  std::ifstream infile("data/launch.scn");
  std::string line;
  while (std::getline(infile,line)){
    modelNames.push_back(line);
  }
}

void loadModels(){
  std::ifstream infile(modelNames[0]);
  std::string line;
  while (std::getline(infile,line)){
    std::vector<std::string> result;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
      result.push_back(line);
    if (result[0].compare("v")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      GLfloat z = std::stof(result[3]);
      earth_v_positions[earth_tri_idx] = glm::vec4(x,y,z,1.0);
    }
    else if(result[0].compare("vt")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      earth_tex_coords[earth_tri_idx] = glm::vec2(x,y);
      earth_tri_idx++;
    }
  }

  std::ifstream infile1(modelNames[1]);
  while (std::getline(infile1,line)){
    std::vector<std::string> result;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
      result.push_back(line);
    if (result[0].compare("v")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      GLfloat z = std::stof(result[3]);
      space_v_positions[space_tri_idx] = glm::vec4(x,y,z,1.0);
    }
    else if(result[0].compare("vt")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      space_tex_coords[space_tri_idx] = glm::vec2(x,y);
      space_tri_idx++;
    }
  }

  std::ifstream infile2(modelNames[2]);
  GLfloat angle = 180.0;
  GLfloat scaling = 0.05;
  glm::mat4 site_trans_mat = glm::translate(glm::mat4(1.0),glm::vec3(0.0,-Radius-0.5,0.0));
  glm::mat4 site_rot = glm::rotate(glm::scale(site_trans_mat,glm::vec3(2.3*scaling,scaling,scaling)),glm::radians(angle),glm::vec3(0.0,0.0,1.0));
  site_rot = glm::rotate(site_rot,glm::radians(angle),glm::vec3(0.0,1.0,0.0));
  GLfloat sxangle = -15.0;
  GLfloat szangle = -10.0;
  glm::mat4 final_site_trans = glm::rotate(glm::mat4(1.0),glm::radians(sxangle),glm::vec3(1.0,0.0,0.0));
  final_site_trans = glm::rotate(final_site_trans,glm::radians(szangle),glm::vec3(0.0,0.0,1.0));
  glm::mat4 site_trans = final_site_trans*site_rot;
  while (std::getline(infile2,line)){
    std::vector<std::string> result;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
      result.push_back(line);
    if (result[0].compare("v")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      GLfloat z = std::stof(result[3]);
      launch_stand_v_positions[launch_stand_tri_idx] = site_trans*glm::vec4(x,y,z,1.0);
    }
    else if(result[0].compare("vt")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      launch_stand_tex_coords[launch_stand_tri_idx] = glm::vec2(x,y);
      launch_stand_tri_idx++;
    }
  }
  GLfloat veh_scaling = 0.04;
  glm::mat4 veh_trans = glm::scale(glm::mat4(1.0),glm::vec3(veh_scaling,veh_scaling/2.5,veh_scaling));
  std::ifstream infile3(modelNames[3]);
  while (std::getline(infile3,line)){
    std::vector<std::string> result;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
      result.push_back(line);
    if (result[0].compare("v")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      GLfloat z = std::stof(result[3]);
      down_v_positions[down_tri_idx] = veh_trans*glm::vec4(x,y,z,1.0);
    }
    else if(result[0].compare("vt")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      down_tex_coords[down_tri_idx] = glm::vec2(x,y);
      down_tri_idx++;
    }

  }
  //std::cout<<down_tri_idx<<std::endl;
  std::ifstream infile4(modelNames[4]);
  while (std::getline(infile4,line)){
    std::vector<std::string> result;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
      result.push_back(line);
    if (result[0].compare("v")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      GLfloat z = std::stof(result[3]);
      booster1_v_positions[booster1_tri_idx] = veh_trans*glm::vec4(x,y,z,1.0);
    }
    else if(result[0].compare("vt")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      booster1_tex_coords[booster1_tri_idx] = glm::vec2(x,y);
      booster1_tri_idx++;
    }
  }

  std::ifstream infile5(modelNames[5]);
  while (std::getline(infile5,line)){
    std::vector<std::string> result;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
      result.push_back(line);
    if (result[0].compare("v")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      GLfloat z = std::stof(result[3]);
      booster2_v_positions[booster2_tri_idx] = veh_trans*glm::vec4(x,y,z,1.0);
    }
    else if(result[0].compare("vt")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      booster2_tex_coords[booster2_tri_idx] = glm::vec2(x,y);
      booster2_tri_idx++;
    }
  }

  std::ifstream infile6(modelNames[6]);
  while (std::getline(infile6,line)){
    std::vector<std::string> result;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
      result.push_back(line);
    if (result[0].compare("v")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      GLfloat z = std::stof(result[3]);
      central_v_positions[central_tri_idx] = veh_trans*glm::vec4(x,y,z,1.0);
    }
    else if(result[0].compare("vt")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      central_tex_coords[central_tri_idx] = glm::vec2(x,y);
      central_tri_idx++;
    }
  }

  std::ifstream infile7(modelNames[7]);
  while (std::getline(infile7,line)){
    std::vector<std::string> result;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
      result.push_back(line);
    if (result[0].compare("v")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      GLfloat z = std::stof(result[3]);
      veh_v_positions[veh_tri_idx] = veh_trans*glm::vec4(x,y,z,1.0);
    }
    else if(result[0].compare("vt")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      veh_tex_coords[veh_tri_idx] = glm::vec2(x,y);
      veh_tri_idx++;
    }
  }

  GLfloat pay_scaling = 0.04*0.15;
  GLfloat payangle = 180.0;
  glm::mat4 pay_trans = glm::rotate(glm::scale(glm::mat4(1.0),glm::vec3(pay_scaling,pay_scaling,pay_scaling)),glm::radians(payangle),glm::vec3(0.0,1.0,0.0));
  std::ifstream infile8(modelNames[8]);
  while (std::getline(infile8,line)){
    std::vector<std::string> result;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
      result.push_back(line);
    if (result[0].compare("v")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      GLfloat z = std::stof(result[3]);
      pay_cube_v_positions[pay_cube_tri_idx] = pay_trans*glm::vec4(x,y,z,1.0);
    }
    else if(result[0].compare("vt")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      pay_cube_tex_coords[pay_cube_tri_idx] = glm::vec2(x,y);
      pay_cube_tri_idx++;
    }
  }

  GLfloat panel_scaling = 0.04*0.15;
  glm::mat4 panel_trans = glm::scale(glm::mat4(1.0),glm::vec3(panel_scaling,panel_scaling,panel_scaling));
  std::ifstream infile9(modelNames[9]);
  while (std::getline(infile9,line)){
    std::vector<std::string> result;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
      result.push_back(line);
    if (result[0].compare("v")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      GLfloat z = std::stof(result[3]);
      panel1_v_positions[panel1_tri_idx] = panel_trans*glm::vec4(x,y,z,1.0);
    }
    else if(result[0].compare("vt")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      panel1_tex_coords[panel1_tri_idx] = glm::vec2(x,y);
      panel1_tri_idx++;
    }
  }
  std::ifstream infile10(modelNames[10]);
  while (std::getline(infile10,line)){
    std::vector<std::string> result;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
      result.push_back(line);
    if (result[0].compare("v")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      GLfloat z = std::stof(result[3]);
      panel2_v_positions[panel2_tri_idx] = panel_trans*glm::vec4(x,y,z,1.0);
    }
    else if(result[0].compare("vt")==0){
      GLfloat x = std::stof(result[1]);
      GLfloat y = std::stof(result[2]);
      panel2_tex_coords[panel2_tri_idx] = glm::vec2(x,y);
      panel2_tri_idx++;
    }
  }
  //std::cout<<earth_tri_idx<<std::endl;
}
//ParticleCube
glm::vec2 t_coords[4] = {
  glm::vec2( 0.0, 0.0),
  glm::vec2( 0.0, 1.0),
  glm::vec2( 1.0, 0.0),
  glm::vec2( 1.0, 1.0)
};
const int smoke_num_vertices = 36;
float smoke_edge_len = 0.007;
glm::vec4 smoke_cube_positions[8] = {
  glm::vec4(-smoke_edge_len, -smoke_edge_len, smoke_edge_len, 1.0),
  glm::vec4(-smoke_edge_len, smoke_edge_len, smoke_edge_len, 1.0),
  glm::vec4(smoke_edge_len, smoke_edge_len, smoke_edge_len, 1.0),
  glm::vec4(smoke_edge_len, -smoke_edge_len, smoke_edge_len, 1.0),
  glm::vec4(-smoke_edge_len, -smoke_edge_len, -smoke_edge_len, 1.0),
  glm::vec4(-smoke_edge_len, smoke_edge_len, -smoke_edge_len, 1.0),
  glm::vec4(smoke_edge_len, smoke_edge_len, -smoke_edge_len, 1.0),
  glm::vec4(smoke_edge_len, -smoke_edge_len, -smoke_edge_len, 1.0)
};
int smoke_tri_idx = 0;
glm::vec4 smoke_v_positions[smoke_num_vertices];
glm::vec2 smoke_tex_coords[smoke_num_vertices];
void smoke_quad(int a, int b, int c, int d)
{
  smoke_tex_coords[smoke_tri_idx] = t_coords[1];
  smoke_v_positions[smoke_tri_idx] = smoke_cube_positions[a]; 
  smoke_tri_idx++;
  smoke_tex_coords[smoke_tri_idx] = t_coords[0];
  smoke_v_positions[smoke_tri_idx] = smoke_cube_positions[b];
  smoke_tri_idx++;
  smoke_tex_coords[smoke_tri_idx] = t_coords[2];
  smoke_v_positions[smoke_tri_idx] = smoke_cube_positions[c]; 
  smoke_tri_idx++;
  smoke_tex_coords[smoke_tri_idx] = t_coords[1];
  smoke_v_positions[smoke_tri_idx] = smoke_cube_positions[a]; 
  smoke_tri_idx++;
  smoke_tex_coords[smoke_tri_idx] = t_coords[2];
  smoke_v_positions[smoke_tri_idx] = smoke_cube_positions[c]; 
  smoke_tri_idx++;
  smoke_tex_coords[smoke_tri_idx] = t_coords[3];
  smoke_v_positions[smoke_tri_idx] = smoke_cube_positions[d]; 
  smoke_tri_idx++;
 }
 void draw_smoke_cube(){
 	smoke_quad( 1, 0, 3, 2);
  	smoke_quad( 2, 3, 7, 6);
  	smoke_quad( 3, 0, 4, 7);
  	smoke_quad( 6, 5, 1, 2);
  	smoke_quad( 4, 5, 6, 7);
  	smoke_quad( 5, 4, 0, 1);
 }
 struct Particle {
    glm::vec4 Position;
    glm::vec3 Velocity;
    glm::vec4 Color;
    GLfloat Life;
  
    Particle() {
    	Position = glm::vec4(0.0,0.0,0.0,1.0);
    	Velocity = glm::vec3(0.0);
    	Color = glm::vec4(1.0);
    	Life = 0.0;
    }
};  

int nr_particles = 10000;
std::vector<Particle> smokeparticles;

GLuint lastUsedParticle = 0;
GLuint FirstUnusedParticle()
{
    // Search from last used particle, this will usually return almost instantly
    for (GLuint i = lastUsedParticle; i < nr_particles; ++i){
        if (smokeparticles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (GLuint i = 0; i < lastUsedParticle; ++i){
        if (smokeparticles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // Override first particle if all others are alive
    lastUsedParticle = 0;
    return 0;
}  

void RespawnParticle(Particle &particle,double r,double life,GLfloat speed,GLfloat init)
{
    GLfloat random1 = r*((rand() % 1000) - 500) / 500.0f;
    double p = sqrt(r*r-random1*random1);
    GLfloat random2 = p*((rand() % 1000) - 500) / 500.0f;
    GLfloat rColor = 0.5 + ((rand() % 500) / 500.0f);
    GLfloat rndy = init + 0.2*((rand() % 1000) - 500) / 500.0f;
    particle.Position = glm::vec4(random1,-0.42,random2,1.0);
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = life - 1.2*(1-((rand() % 500) / 500.0f));
    particle.Velocity = speed*glm::normalize(glm::vec3(particle.Position) - glm::vec3(0.0,rndy,0.0));
}
//-----------------------------------------------------------------
glm::vec3 moonpos = glm::vec3(20*Radius,0,-20*Radius);
float moonRadius = Radius/3;
const int point_num_vertices = 1000;
int point_tri_idx = 0;
glm::vec4 point_v_positions[point_num_vertices];
glm::vec4 point_v_colors[point_num_vertices];
void pointSphere(double radius)
{
  
  float slices=PI/10;
  float sectors=PI/10;

  float l;

  for (float lats = 0.0; lats <= PI; lats+=sectors)  
    {
      for(float longs = 0.0; longs <= 2.0*PI; longs+=slices)
	  {
	    float x = radius * sin(lats) * cos(longs);
	    float y = radius * sin(lats) * sin(longs);
	    float z = radius * cos(lats);
	    glm::vec4 pt(x, y, z, 1.0);
	    point_v_colors[point_tri_idx] = glm::vec4(1.0,0.0,0.0,1.0);
	    point_v_positions[point_tri_idx] = pt; 
	    point_tri_idx++;
	   
	    if(lats+sectors>PI)
	      l=PI;
	    else
	      l=lats+sectors;
	    x = radius * sin(l) * cos(longs);
	    y = radius * sin(l) * sin(longs);
	    z = radius * cos(l);
	    pt =glm::vec4(x, y, z, 1.0);
	    point_v_colors[point_tri_idx] = glm::vec4(1.0,0.0,0.0,1.0);
	    point_v_positions[point_tri_idx] = pt; 
	    point_tri_idx++;    
	  }
    }
}
const int moon_num_vertices = 50000;
int moon_tri_idx = 0;
glm::vec4 moon_v_positions[moon_num_vertices];
glm::vec2 moon_tex_coords[moon_num_vertices];
void drawMoon(glm::vec3 center,double radius)
{
  
  float slices=PI/100;
  float sectors=PI/100;

  float l;

  for (float lats = 0.0; lats <= PI; lats+=sectors)  
    {
      for(float longs = 0.0; longs < 2.0*PI+slices; longs+=slices)
    {
      float x = center.x + radius * sin(lats) * cos(longs);
      float y = center.y + radius * sin(lats) * sin(longs);
      float z = center.z + radius * cos(lats);
      glm::vec4 pt(x, y, z, 1.0);
      moon_tex_coords[moon_tri_idx] = glm::vec2(longs/(2*PI),1.0-lats/PI);
      moon_v_positions[moon_tri_idx] = pt; 
      moon_tri_idx++;
     
      if(lats+sectors>PI)
        l=PI;
      else
        l=lats+sectors;
      x = center.x + radius * sin(l) * cos(longs);
      y = center.y + radius * sin(l) * sin(longs);
      z = center.z + radius * cos(l);
      pt =glm::vec4(x, y, z, 1.0);
      moon_tex_coords[moon_tri_idx] = glm::vec2(longs/(2*PI),1.0-l/PI);
      moon_v_positions[moon_tri_idx] = pt; 
      moon_tri_idx++;    
    }
    }
    //std::cout<<moon_tri_idx<<std::endl;
}
double mypow(double a,int n){
	double ans = 1;
	for (int i=0;i<n;i++)
		ans=ans*a;
	return ans;
}
float C(long long int n,long long int i){
  long long int numer=1,denom=1;
  if (i>=n-i){
    for (long long int p=n;p>i;p--)
      numer=numer*p;
    for (long long int p=n-i;p>1;p--)
      denom = denom*p;
  }
  else {
    for (long long int p=n;p>n-i;p--)
      numer=numer*p;
    for (long long int p=i;p>1;p--)
      denom = denom*p;
  }
  return (float) numer/denom;
}
int trajectory_num_array[15] = {1,1,2,2,3,3,3,3,3,3,3,4,5,6};
glm::vec4 trajectory_color_array[7] = {glm::vec4(0.0),red,blue,green,rg,bg,violet};

std::vector<glm::vec3> control_points;
const int curve_num_vertices = 100000;
int curve_tri_idx = 0;
glm::vec4 curve_v_positions[curve_num_vertices];
float t_values[curve_num_vertices];
int trajectory_nums[curve_num_vertices];
glm::vec3 curve_derivatives[curve_num_vertices];
glm::vec4 curve_v_colors[curve_num_vertices];
void drawBezier(int start,int end,int triid,bool startover,int trajectory_no){
  int maxnum = 250*(end-start);
  double step = 1.0/maxnum;
  if (startover)
    curve_tri_idx = triid;
  for (double t=0;t<1+step;t+=step){
    glm::vec3 P = glm::vec3(0.0,0.0,0.0);
    glm::vec3 D = glm::vec3(0.0,0.0,0.0);
    int n = end-start-1;
    for (int i=0;i<=n;i++){
      GLfloat prod = C(n,i)*mypow(t,i)*mypow(1-t,n-i);
      P = P + prod*control_points[i+start];
      if (i<n){
        prod = n*C(n-1,i)*mypow(t,i)*mypow(1-t,n-1-i);
        D = D + prod*(control_points[i+1+start]-control_points[i+start]);
      }
    }
    t_values[curve_tri_idx] = t;
    trajectory_nums[curve_tri_idx] = trajectory_no;
    curve_v_positions[curve_tri_idx] = glm::vec4(P,1.0);
    curve_derivatives[curve_tri_idx] = glm::normalize(D);
    curve_v_colors[curve_tri_idx] = trajectory_color_array[trajectory_no];
    curve_tri_idx++;
  }
  //std::cout<<curve_tri_idx<<std::endl;
}
int startingControlPoints=0;
void drawBezierEllipse(float x,float y,float w,float h,glm::mat4 RotMatrix,int trajectory_no){
  x = x-w/2;y=y-h/2;
  float kappa = 0.5522848;
  float ox = (w/2)*kappa;
  float oy = (h/2)*kappa;
  float xe = x+w;
  float ye = y+h;
  float xm = x+w/2,ym = y+h/2;
  int start = control_points.size();
  control_points.push_back(glm::vec3(x,ym,0.0));
  control_points.push_back(glm::vec3(x,ym-oy,0.0));
  control_points.push_back(glm::vec3(xm-ox,y,0.0));
  control_points.push_back(glm::vec3(xm,y,0.0));
  control_points.push_back(glm::vec3(xm+ox,y,0.0));
  control_points.push_back(glm::vec3(xe,ym-oy,0.0));
  control_points.push_back(glm::vec3(xe,ym,0.0));
  control_points.push_back(glm::vec3(xe,ym+oy,0.0));
  control_points.push_back(glm::vec3(xm+ox,ye,0.0));
  control_points.push_back(glm::vec3(xm,ye,0.0));
  control_points.push_back(glm::vec3(xm-ox,ye,0.0));
  control_points.push_back(glm::vec3(x,ym+oy,0.0));
  control_points.push_back(glm::vec3(x,ym,0.0));
  int end = control_points.size();
  for (int i=start;i<end;i++){
    glm::vec4 Cpoint = glm::vec4(control_points[i],1.0);
    control_points[i] = glm::vec3(RotMatrix*Cpoint);
  }
  drawBezier(start,end,0,false,trajectory_no);
}
std::vector<int> trajectories(1,0);
std::vector<int> controlPointIndices(1,0);

void saveTrajectories(){
  std::ofstream fout;
  fout.open("data/trajectory.txt");
  for (int i=0;i<controlPointIndices.size()-1;i++){
    fout<<controlPointIndices[i+1]-controlPointIndices[i]<<std::endl;
    for (int j=controlPointIndices[i];j<controlPointIndices[i+1];j++){
      fout<<control_points[j].x<<" "<<control_points[j].y<<" "<<control_points[j].z<<std::endl;
    }
  }
  fout<<control_points.size()-controlPointIndices[controlPointIndices.size()-1]<<std::endl;
  for (int j=controlPointIndices[controlPointIndices.size()-1];j<control_points.size();j++)
    fout<<control_points[j].x<<" "<<control_points[j].y<<" "<<control_points[j].z<<std::endl;
  fout.close();
  fout.open("data/keyframes.txt");
  for (int i=0;i<curve_tri_idx;i++){
  	int tarray[6] = {0};
  	tarray[trajectory_nums[i]-1]=1;
  	for (int j=0;j<6;j++){
  		if (tarray[j]==1)
  			fout<<1<<" "<<t_values[i]<<" ";
  		else
  			fout<<0<<" "<<0<<" ";
  	}
  	fout<<std::endl;
  }
  fout.close();
}
void loadTrajectories(){
  std::ifstream infile("data/trajectory.txt");
  std::string line;
  control_points.clear();
  trajectories.clear();trajectories.push_back(0);
  controlPointIndices.clear();controlPointIndices.push_back(0);
  curve_tri_idx = 0;
  for (int i=0;i<curve_num_vertices;i++)
    curve_v_positions[i] = glm::vec4(0.0);
  int tempsum = 0;
  while (std::getline(infile,line)){
    std::vector<std::string> result;
    std::istringstream iss(line);
    for(std::string line; iss >> line;)
      result.push_back(line);
    if (result.size()==1){
      int numcontrols = std::stoi(result[0]);
      controlPointIndices.push_back(tempsum+numcontrols);
      tempsum+=numcontrols;
    }
    else if (result.size()==3){
      GLfloat x = std::stof(result[0]);
      GLfloat y = std::stof(result[1]);
      GLfloat z = std::stof(result[2]);
      control_points.push_back(glm::vec3(x,y,z));
    }
  }
  if (controlPointIndices.size()>14){
    for (int i=0;i<14;i++){
      drawBezier(controlPointIndices[i],controlPointIndices[i+1],0,false,trajectory_num_array[i]);
      trajectories.push_back(curve_tri_idx);
    }
    trajectories.push_back(curve_tri_idx);
    startingControlPoints = controlPointIndices[controlPointIndices.size()-1];
    if (controlPointIndices.size()>15){
      drawBezier(controlPointIndices[14],controlPointIndices[15],curve_tri_idx,true,trajectory_num_array[trajectories.size()-1]);
      trajectories[trajectories.size()-1]=curve_tri_idx;
      startingControlPoints = controlPointIndices[controlPointIndices.size()-2];
    }
  }
  //std::cout<<control_points.size()<<std::endl;
  
}
void initBuffersGL(void)
{
  for (int i=0;i<nr_particles;i++)
	smokeparticles.push_back(Particle());
  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("src/earth_vshader.glsl");
  std::string fragment_shader_file("src/earth_fshader.glsl");


  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  earth_shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( earth_shaderProgram );
  // getting the attributes from the shader program
  GLuint vPosition = glGetAttribLocation( earth_shaderProgram, "vPosition" );
  GLuint vColor = glGetAttribLocation( earth_shaderProgram, "vColor" ); 
  GLuint vNormal = glGetAttribLocation( earth_shaderProgram, "vNormal" ); 
  GLuint texCoord = glGetAttribLocation( earth_shaderProgram, "texCoord" ); 
  uModelViewMatrix = glGetUniformLocation( earth_shaderProgram, "uModelViewMatrix");
  normalMatrix =  glGetUniformLocation( earth_shaderProgram, "normalMatrix");
  viewMatrix = glGetUniformLocation( earth_shaderProgram, "viewMatrix");
  shaderTime = glGetUniformLocation( earth_shaderProgram , "iTime");
  earthRadius = glGetUniformLocation( earth_shaderProgram , "earthRadius");
  shaderEye = glGetUniformLocation( earth_shaderProgram , "eye");
  // Load Textures 
  earthtex=LoadTexture("src/images/earth.bmp",2048,1024);
  glBindTexture(GL_TEXTURE_2D, earthtex);
  

  //Ask GL for two Vertex Attribute Objects (vao) , one for the sphere and one for the wireframe
  glGenVertexArrays (13, vao);
  //Ask GL for two Vertex Buffer Object (vbo)
  glGenBuffers (13, vbo);

  //Set 0 as the current array to be used by binding it
  glBindVertexArray (vao[0]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);

  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (earth_v_positions) + sizeof(earth_tex_coords) + sizeof(earth_v_normals), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(earth_v_positions), earth_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(earth_v_positions), sizeof(earth_tex_coords), earth_tex_coords);
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(earth_tex_coords)+sizeof(earth_v_positions), sizeof(earth_v_normals), earth_v_normals );
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(earth_v_positions)) );

  //Normal
  glEnableVertexAttribArray( vNormal );
  glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(earth_v_positions)+sizeof(earth_tex_coords)) );

  vertex_shader_file = "src/planet_vshader.glsl";
  fragment_shader_file = "src/planet_fshader.glsl";
  std::vector<GLuint> shaderList1;
  shaderList1.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList1.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));
  planetshaderProgram = csX75::CreateProgramGL(shaderList1);
  glUseProgram( planetshaderProgram );

  uModelViewMatrix1 = glGetUniformLocation( planetshaderProgram, "uModelViewMatrix");
  vPosition = glGetAttribLocation( planetshaderProgram, "vPosition" );
  texCoord = glGetAttribLocation( planetshaderProgram, "texCoord" ); 
  
  spacetex=LoadTexture("src/images/space.bmp",960,540);
  glBindTexture(GL_TEXTURE_2D, spacetex);
  glBindVertexArray (vao[1]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);
  glBufferData (GL_ARRAY_BUFFER, sizeof (space_v_positions) + sizeof(space_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(space_v_positions), space_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(space_v_positions), sizeof(space_tex_coords), space_tex_coords);
  // set up vertex array
  //Position
  
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(space_v_positions)) );
  
  vertex_shader_file = "src/launch_site_simple_tex_vshader.glsl";
  fragment_shader_file = "src/launch_site_simple_tex_fshader.glsl";

  std::vector<GLuint> shaderList2;
  shaderList2.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList2.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  launch_stand_shaderProgram = csX75::CreateProgramGL(shaderList2);
  glUseProgram( launch_stand_shaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( launch_stand_shaderProgram, "vPosition" );
  texCoord = glGetAttribLocation( launch_stand_shaderProgram, "texCoord" ); 
  launch_stand_uModelViewMatrix = glGetUniformLocation( launch_stand_shaderProgram, "uModelViewMatrix");

  glBindVertexArray (vao[2]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[2]);
  sitebasetex=LoadTexture("src/images/base_tex.bmp",376,376);
  
  //Copy the points into the current buffer
  launchstandtex=LoadTexture("src/images/base_tex.bmp",376,376);
  
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (launch_stand_v_positions) + sizeof(launch_stand_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(launch_stand_v_positions), launch_stand_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(launch_stand_v_positions), sizeof(launch_stand_tex_coords), launch_stand_tex_coords);
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(launch_stand_v_positions)) );

  armtex = LoadTexture("src/images/dark-metal-texture.bmp",3000,2000);
  redtex = LoadTexture("src/images/red_tex.bmp",450,450);
  conctex = LoadTexture("src/images/conc_texture.bmp",1000,563);

  vertex_shader_file = "src/rover_simple_tex_vshader.glsl";
  fragment_shader_file = "src/rover_simple_tex_fshader.glsl";

  std::vector<GLuint> shaderList3;
  shaderList3.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList3.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  texshaderProgram = csX75::CreateProgramGL(shaderList3);
  glUseProgram( texshaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( texshaderProgram, "vPosition" );
  texCoord = glGetAttribLocation( texshaderProgram, "texCoord" ); 
  texuModelViewMatrix = glGetUniformLocation( texshaderProgram, "uModelViewMatrix");
  
  // Load Textures 
  basetex=LoadTexture("src/images/base_tex.bmp",376,376);

  glBindVertexArray (vao[3]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[3]);
  //std::cout<<pay_cube_tri_idx<<std::endl;
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (pay_cube_v_positions) + sizeof(pay_cube_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(pay_cube_v_positions), pay_cube_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(pay_cube_v_positions), sizeof(pay_cube_tex_coords), pay_cube_tex_coords);
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pay_cube_v_positions)) );

  glBindVertexArray (vao[4]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[4]);

  //std::cout<<pay_cube_tri_idx<<std::endl;
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (panel1_v_positions) + sizeof(panel1_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(panel1_v_positions), panel1_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(panel1_v_positions), sizeof(panel1_tex_coords), panel1_tex_coords);
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(panel1_v_positions)) );

  glBindVertexArray (vao[5]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[5]);

  //std::cout<<pay_cube_tri_idx<<std::endl;
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (panel2_v_positions) + sizeof(panel2_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(panel2_v_positions), panel2_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(panel2_v_positions), sizeof(panel2_tex_coords), panel2_tex_coords);
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(panel2_v_positions)) );

  glBindVertexArray (vao[6]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[6]);

  //std::cout<<pay_cube_tri_idx<<std::endl;
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (veh_v_positions) + sizeof(veh_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(veh_v_positions), veh_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(veh_v_positions), sizeof(veh_tex_coords), veh_tex_coords);
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(veh_v_positions)) );

  glBindVertexArray (vao[7]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[7]);

  //std::cout<<pay_cube_tri_idx<<std::endl;
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (booster1_v_positions) + sizeof(booster1_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(booster1_v_positions), booster1_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(booster1_v_positions), sizeof(booster1_tex_coords), booster1_tex_coords);
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(booster1_v_positions)) );

  glBindVertexArray (vao[8]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[8]);

  //std::cout<<pay_cube_tri_idx<<std::endl;
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (booster2_v_positions) + sizeof(booster2_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(booster2_v_positions), booster2_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(booster2_v_positions), sizeof(booster2_tex_coords), booster2_tex_coords);
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(booster2_v_positions)) );

  glBindVertexArray (vao[9]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[9]);

  //std::cout<<pay_cube_tri_idx<<std::endl;
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (down_v_positions) + sizeof(down_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(down_v_positions), down_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(down_v_positions), sizeof(down_tex_coords), down_tex_coords);
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(down_v_positions)) );

  glBindVertexArray (vao[10]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[10]);

  //std::cout<<pay_cube_tri_idx<<std::endl;
  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (central_v_positions) + sizeof(central_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(central_v_positions), central_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(central_v_positions), sizeof(central_tex_coords), central_tex_coords);
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(central_v_positions)) );
  goldtex=LoadTexture("src/images/gold-foil.bmp",720,540);
  paneltex=LoadTexture("src/images/panel_tex.bmp",612,408);
  hoodtex=LoadTexture("src/images/hood_tex.bmp",516,318);
  boosterbody=LoadTexture("src/images/booster_body.bmp",1000,1000);
  centraltex=LoadTexture("src/images/central_tex.bmp",450,112);
  blaster=LoadTexture("src/images/metallic_tex.bmp",600,450);
  boosterhead=LoadTexture("src/images/booster_head.bmp",509,339);

  //Particles---------------------------------------------------------------------------------------

  vertex_shader_file = "src/particle_vshader.glsl";
  fragment_shader_file = "src/particle_fshader.glsl";

  std::vector<GLuint> shaderList4;
  shaderList4.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList4.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  particleshaderProgram = csX75::CreateProgramGL(shaderList4);
  glUseProgram( particleshaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( particleshaderProgram, "vPosition" );
  texCoord = glGetAttribLocation( particleshaderProgram, "texCoord" ); 
  particleColor = glGetUniformLocation (particleshaderProgram, "color" );
  particlemat1 = glGetUniformLocation( particleshaderProgram, "particlemodelViewMatrix");
  particlemat2 = glGetUniformLocation ( particleshaderProgram, "particlebasetransMatrix");
  particlemat3 = glGetUniformLocation ( particleshaderProgram, "particleinitMatrix");
  
  // Load Textures 
  smoketex=LoadTexture("src/images/base_tex.bmp",376,376);
  draw_smoke_cube();
  glBindVertexArray (vao[11]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[11]);

  glBufferData (GL_ARRAY_BUFFER, sizeof (smoke_v_positions) + sizeof(smoke_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(smoke_v_positions), smoke_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(smoke_v_positions), sizeof(smoke_tex_coords), smoke_tex_coords);
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(smoke_v_positions)) );

  vertex_shader_file = "src/color_vshader.glsl";
  fragment_shader_file = "src/color_fshader.glsl";
  std::vector<GLuint> shaderList5;
  shaderList5.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList5.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));
  sphereshaderProgram = csX75::CreateProgramGL(shaderList5);
  glUseProgram( sphereshaderProgram );

  vPosition = glGetAttribLocation( sphereshaderProgram, "vPosition" );
  vColor = glGetAttribLocation( sphereshaderProgram, "vColor" ); 
  sphereumodelViewMatrix = glGetUniformLocation( sphereshaderProgram, "uModelViewMatrix");
  spheretransMatrix = glGetUniformLocation( sphereshaderProgram, "transMatrix");
  pointSphere(0.4);
  
  glBindVertexArray (vao[12]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[12]);
  glBufferData (GL_ARRAY_BUFFER, sizeof (point_v_positions) + sizeof(point_v_colors), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point_v_positions), point_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(point_v_positions), sizeof(point_v_colors), point_v_colors);
  // set up vertex array
  //Position
  
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point_v_positions)) );

  //draw base trajectory
  float curveA = 2*Radius,curveB = 5*Radius/3.5,deltaC = PI/22,deltaX = Radius*0.8;
  GLfloat Crotx = -15.0,Crotz=-10.0;
  glm::mat4 curveRotMatrix = glm::rotate(glm::mat4(1.0),glm::radians(Crotx),glm::vec3(1.0,0.0,0.0));
  curveRotMatrix = glm::rotate(curveRotMatrix,glm::radians(Crotz+90),glm::vec3(0.0,0.0,1.0));
  glm::mat4 curveRotMatrix1 = glm::rotate(glm::mat4(1.0),glm::radians(Crotx),glm::vec3(1.0,0.0,0.0));
  curveRotMatrix1 = glm::rotate(curveRotMatrix,glm::radians(Crotz),glm::vec3(0.0,0.0,1.0));
  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-0.42,0.0,0.0,1.0)));
  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/7,0.0,0.0,1.0)));
  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/4.1,-Radius/5,0.0,1.0)));
  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/32,-Radius/2,0.0,1.0)));
  drawBezier(0,control_points.size(),0,false,trajectory_num_array[trajectories.size()-1]);
  controlPointIndices.push_back(control_points.size());trajectories.push_back(curve_tri_idx);
  drawBezierEllipse(1.87*deltaX,0.0,2.7*curveA,2.3*curveB,curveRotMatrix,trajectory_num_array[trajectories.size()-1]);
  trajectories.push_back(curve_tri_idx);controlPointIndices.push_back(control_points.size());

  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/5.1,Radius/8,0.0,1.0)));
  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/4.7,0.0,0.0,1.0)));
  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/4.8,-Radius/6,0.0,1.0)));
  drawBezier(controlPointIndices[controlPointIndices.size()-1],control_points.size(),0,false,trajectory_num_array[trajectories.size()-1]);
  controlPointIndices.push_back(control_points.size());trajectories.push_back(curve_tri_idx);
  drawBezierEllipse(3.47*deltaX,0.0,4*curveA,3*curveB,curveRotMatrix,trajectory_num_array[trajectories.size()-1]);
  trajectories.push_back(curve_tri_idx);controlPointIndices.push_back(control_points.size());

  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/50-Radius/5.1,Radius/8,0.0,1.0)));
  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/50-Radius/4.7,0.0,0.0,1.0)));
  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/50-Radius/4.9,-Radius/6,0.0,1.0)));
  drawBezier(controlPointIndices[controlPointIndices.size()-1],control_points.size(),0,false,trajectory_num_array[trajectories.size()-1]);
  controlPointIndices.push_back(control_points.size());trajectories.push_back(curve_tri_idx);
  drawBezierEllipse(5.2*deltaX,0.0,5.4*curveA,3.5*curveB,curveRotMatrix,trajectory_num_array[trajectories.size()-1]);
  trajectories.push_back(curve_tri_idx);controlPointIndices.push_back(control_points.size());

  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/28-Radius/5.1,Radius/8,0.0,1.0)));
  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/28-Radius/4.7,0.0,0.0,1.0)));
  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/28-Radius/4.9,-Radius/6,0.0,1.0)));
  drawBezier(controlPointIndices[controlPointIndices.size()-1],control_points.size(),0,false,trajectory_num_array[trajectories.size()-1]);
  controlPointIndices.push_back(control_points.size());trajectories.push_back(curve_tri_idx);
  drawBezierEllipse(7.43*deltaX,0.0,7.2*curveA,4.1*curveB,curveRotMatrix,trajectory_num_array[trajectories.size()-1]);
  trajectories.push_back(curve_tri_idx);controlPointIndices.push_back(control_points.size());

  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/19.5-Radius/5.1,Radius/8,0.0,1.0)));
  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/19.5-Radius/4.7,0.0,0.0,1.0)));
  control_points.push_back(glm::vec3(curveRotMatrix*glm::vec4(-Radius-Radius/19-Radius/4.95,-Radius/6,0.0,1.0)));
  drawBezier(controlPointIndices[controlPointIndices.size()-1],control_points.size(),0,false,trajectory_num_array[trajectories.size()-1]);
  controlPointIndices.push_back(control_points.size());trajectories.push_back(curve_tri_idx);
  drawBezierEllipse(10.29*deltaX,0.0,9.5*curveA,4.85*curveB,curveRotMatrix,trajectory_num_array[trajectories.size()-1]);
  trajectories.push_back(curve_tri_idx);controlPointIndices.push_back(control_points.size());
  control_points.push_back(control_points[control_points.size()-1]);
  GLfloat dummy2 = 3,dummy3=2,dummyangle=90;
  control_points.push_back((dummy3)*control_points[control_points.size()-1]-control_points[control_points.size()-3]);
  glm::mat4 modeltemp = glm::rotate(glm::mat4(1.0),glm::radians(dummyangle),glm::vec3(1.0,0.0,0.0));
  glm::vec3 nmoonpos = glm::vec3(modeltemp*glm::vec4(moonpos,1.0));
  control_points.push_back(control_points[control_points.size()-1]+(1/dummy3)*nmoonpos);
  control_points.push_back(nmoonpos+glm::vec3(-2*moonRadius,10*moonRadius,0.0));
  control_points.push_back(nmoonpos+glm::vec3(0,4*moonRadius,0.0));
  control_points.push_back(nmoonpos+glm::vec3(3.5*moonRadius,3.2*moonRadius,0.0));
  control_points.push_back(nmoonpos+glm::vec3(4*moonRadius,2.8*moonRadius,0.0));
  drawBezier(controlPointIndices[controlPointIndices.size()-1],control_points.size(),0,false,trajectory_num_array[trajectories.size()-1]);
  controlPointIndices.push_back(control_points.size());trajectories.push_back(curve_tri_idx);
  GLfloat dumangle = 30;
  GLfloat piangle=180;
  glm::mat4 moonmat = glm::translate(glm::mat4(1.0),nmoonpos)*glm::rotate(glm::rotate(glm::mat4(1.0),glm::radians(dumangle),glm::vec3(0.0,0.0,1.0)),glm::radians(piangle),glm::vec3(1.0,0.0,0.0));
  drawBezierEllipse(moonRadius,0.0,11*moonRadius,5*moonRadius,moonmat,trajectory_num_array[trajectories.size()-1]);
  trajectories.push_back(curve_tri_idx);controlPointIndices.push_back(control_points.size());
  drawBezierEllipse(0.0,0.0,9*moonRadius,4.4*moonRadius,moonmat,trajectory_num_array[trajectories.size()-1]);
  trajectories.push_back(curve_tri_idx);controlPointIndices.push_back(control_points.size());
  drawBezierEllipse(0.0,0.0,4.4*moonRadius,4.4*moonRadius,moonmat,trajectory_num_array[trajectories.size()-1]);
  trajectories.push_back(curve_tri_idx);controlPointIndices.push_back(control_points.size());
  //std::cout<<curve_tri_idx;
  trajectories.push_back(curve_tri_idx);
  startingControlPoints = control_points.size();
  if (control_points.size()>1){
      //drawBezier();
      glGenVertexArrays (1, &vao[13]);
      //Ask GL for two Vertex Buffer Object (vbo)
      glGenBuffers (1, &vbo[13]);
      glBindVertexArray (vao[13]);
      //Set 0 as the current buffer to be used by binding it
      glBindBuffer (GL_ARRAY_BUFFER, vbo[13]);
      glBufferData (GL_ARRAY_BUFFER, sizeof (curve_v_positions) + sizeof(curve_v_colors), NULL, GL_STATIC_DRAW);
      glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(curve_v_positions), curve_v_positions );
      glBufferSubData( GL_ARRAY_BUFFER, sizeof(curve_v_positions), sizeof(curve_v_colors), curve_v_colors);
      // set up vertex array
      //Position
      GLuint vPosition = glGetAttribLocation( sphereshaderProgram, "vPosition" );
      GLuint vColor = glGetAttribLocation( sphereshaderProgram, "vColor" ); 
      glEnableVertexAttribArray( vPosition );
      glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
      //Textures
      glEnableVertexAttribArray( vColor );
      glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(curve_v_positions)) );
  }
  glGenVertexArrays (1, &moonvao);
  glGenBuffers (1, &moonvbo);
  drawMoon(moonpos,moonRadius);
  glUseProgram( planetshaderProgram );

  vPosition = glGetAttribLocation( planetshaderProgram, "vPosition" );
  texCoord = glGetAttribLocation( planetshaderProgram, "texCoord" ); 
  moontex=LoadTexture("src/images/moon_texture.bmp",2048,1024);
  glBindTexture(GL_TEXTURE_2D, moontex);
  glBindVertexArray (moonvao);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, moonvbo);
  glBufferData (GL_ARRAY_BUFFER, sizeof (moon_v_positions) + sizeof(moon_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(moon_v_positions), moon_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(moon_v_positions), sizeof(moon_tex_coords), moon_tex_coords);
  // set up vertex array
  //Position
  
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(moon_v_positions)) );
  
} 
int calculateID(GLfloat d,GLfloat a,GLfloat t0,int trid){
  float dis = d-Radius-0.4199;
  float t01 = 10,t02=25,t03=5,t04=26,t05=4,t06=27,t07=4,t08=29,t09=4,t010=35,t011=25,t012=18,t013=17,t014=13;
  int diff = trajectories[1]-trajectories[0];
  float tempcal = dis/(a*t01*t01);
  float val;
  int id;
  if (tempcal<=1){
  	val = tempcal - floor(tempcal);
 	id = trajectories[0] + val*diff;
 	return id;
  }
  dis = dis - a*t01*t01+(a*t01*t01)/3.5;
  tempcal = dis/(a*t02*t02);
  diff = trajectories[2]-trajectories[1];
  if (tempcal<=0.992){
  	val = tempcal - floor(tempcal);
 	id = trajectories[1] + val*diff;
 	return id;
  }
  dis = dis - a*t02*t02;
  if (dis<0)
  	dis=0;
  tempcal = dis/(a*t03*t03);
  diff = trajectories[3]-trajectories[2];
  if (tempcal<=1){
  	val = tempcal - floor(tempcal);
  	id = trajectories[2] + val*diff;
  	return id;
  }
  dis = dis - a*t03*t03 + a*t03*t03/3.5;
  tempcal = dis/(a*t04*t04);
  diff = trajectories[4]-trajectories[3];
  if (tempcal<=0.992){
  	val = tempcal - floor(tempcal);
  	id = trajectories[3] + val*diff;
  	return id;
  }
  dis = dis - a*t04*t04;
  if (dis<0)
  	dis=0;
  tempcal = dis/(a*t05*t05);
  diff = trajectories[5]-trajectories[4];
  if (tempcal<=1){
  	val = tempcal - floor(tempcal);
  	id = trajectories[4] + val*diff;
  	return id;
  }
  dis = dis - a*t05*t05 + a*t05*t05/2.4;
  tempcal = dis/(a*t06*t06);
  diff = trajectories[6]-trajectories[5];
  if (tempcal<=0.993){
  	val = tempcal - floor(tempcal);
  	id = trajectories[5] + val*diff;
  	return id;
  }
  dis = dis - a*t06*t06;
  if (dis<0)
  	dis=0;
  tempcal = dis/(a*t07*t07);
  diff = trajectories[7]-trajectories[6];
  if (tempcal<=1){
  	val = tempcal - floor(tempcal);
  	id = trajectories[6] + val*diff;
  	return id;
  }
  dis = dis - a*t07*t07 +a*t07*t07/2;
  tempcal = dis/(a*t08*t08);
  diff = trajectories[8]-trajectories[7];
  if (tempcal<=0.994){
  	val = tempcal - floor(tempcal);
  	id = trajectories[7] + val*diff;
  	return id;
  }
  dis = dis - a*t08*t08;
  if (dis<0)
  	dis=0;
  tempcal = dis/(a*t09*t09);
  diff = trajectories[9]-trajectories[8];
  if (tempcal<=1){
  	val = tempcal - floor(tempcal);
  	id = trajectories[8] + val*diff;
  	return id;
  }
  dis = dis - a*t09*t09 + a*t09*t09/1.9;
  tempcal = dis/(a*t010*t010);
  diff = trajectories[10]-trajectories[9];
  if (tempcal<=1){
  	val = tempcal - floor(tempcal);
  	id = trajectories[9] + val*diff;
  	return id;
  }
  dis = dis - a*t010*t010;
  tempcal = dis/(a*t011*t011);
  diff = trajectories[11]-trajectories[10];
  if (tempcal<=1){
	  val = tempcal - floor(tempcal);
	  id = trajectories[10] + val*diff;
	  return id;
  }
  dis = dis - a*t011*t011+a*t012*t012/2.1;
  tempcal = dis/(a*t012*t012);
  diff = trajectories[12]-trajectories[11];
  if (tempcal<=2){
	  val = tempcal - floor(tempcal);
	  id = trajectories[11] + val*diff;
	  return id;
  }
  dis = dis - 2*a*t012*t012;
  tempcal = dis/(a*t013*t013);
  diff = trajectories[13]-trajectories[12];
  if (tempcal<=1.25){
	  val = tempcal - floor(tempcal);
	  id = trajectories[12] + val*diff;
	  return id;
  }
  dis = dis - 1.25*a*t013*t013 + a*t014*t014/4;
  tempcal = dis/(a*t014*t014);
  diff = trajectories[14]-trajectories[13];
  val = tempcal - floor(tempcal);
  id = trajectories[13] + val*diff;
  return id;
  
  //glm::vec3 ans = glm::vec3(curve_v_positions[id]);
}
bool enteredc1 = true;
int DerRid = 0;
void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(xrot), glm::vec3(1.0f,0.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(yrot), glm::vec3(0.0f,1.0f,0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(zrot), glm::vec3(0.0f,0.0f,1.0f));
  model_matrix = rotation_matrix;

  //Creating the lookat and the up vectors for the camera
  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix;
  glm::vec4 c_up = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix;
  if (launched)
    timeElapsed += 1.0;
  GLfloat veh_sxangle = 20;
  GLfloat veh_szangle = -40.0;
  GLfloat t = 0.01*timeElapsed;
  GLfloat a = 0.1;
  GLfloat acc = a*t*t;
  GLfloat R = Radius + 0.42 + acc;
  GLfloat phi = glm::radians(veh_sxangle);
  GLfloat theta = glm::radians(veh_szangle); 
  GLfloat angle = 180.0;
  GLfloat final_t = 16.0;
  double vh = 10*0.04;
  double bh = 13*vh/14-vh/5-vh/17;
  if (t>final_t)
    R = Radius +0.42 + a*final_t*final_t + 2*a*final_t*(t-final_t);
  
  int Rid = calculateID(R,a,10.0,1);
  //if (t<2)
  	DerRid = Rid;
  glm::mat4 veh_trans_mat = glm::translate(glm::mat4(1.0),glm::vec3(curve_v_positions[Rid]));
  glm::mat4 veh_rot = glm::rotate(glm::mat4(1.0),glm::radians(angle),glm::vec3(0.0,0.0,1.0));
  veh_rot = glm::rotate(veh_rot,glm::radians(angle),glm::vec3(0.0,1.0,0.0));
  veh_rot = glm::rotate(veh_rot,glm::radians(-angle/4),glm::vec3(0.0,1.0,0.0));
  veh_rot = glm::rotate(veh_rot,theta,glm::vec3(0.0,0.0,1.0));
  veh_rot = glm::rotate(veh_rot,phi,glm::vec3(1.0,0.0,0.0));
  //veh_rot = veh_rot*glm::rotate(glm::mat4(1.0),glm::radians(-angle/10),glm::vec3(1.0,1.0,0.0));
  glm::vec3 der = curve_derivatives[DerRid];
  GLfloat arotx = glm::asin(der.z/sqrt(der.x*der.x+der.z*der.z));
  GLfloat aroty = glm::asin(der.y);
  veh_rot = glm::rotate(veh_rot,arotx,glm::vec3(0.0,1.0,0.0));
  veh_rot = glm::rotate(veh_rot,aroty,glm::vec3(1.0,0.0,0.0));
  // glm::mat4 sphere_veh_trans = glm::rotate(glm::mat4(1.0),phi,glm::vec3(1.0,0.0,0.0));
  // sphere_veh_trans = glm::rotate(sphere_veh_trans,theta,glm::vec3(0.0,0.0,1.0));
  // glm::mat4 sphere_veh_trans = glm::mat4(1.0);
  glm::mat4 veh_trans = veh_trans_mat*veh_rot;
  glm::mat4 cam_trans = glm::rotate(glm::mat4(1.0),glm::radians(-angle/2),glm::vec3(1.0,0.0,0.0))*veh_trans_mat;
  GLfloat lpt = 0.0;
  //if (t>1)
  //  std::cout<<t<<std::endl;
  GLfloat camfinal = 2500;
  if (t>final_t && t<camfinal)
    lpt = (bh)*(t-final_t)/(camfinal-final_t);
  else if (t>=camfinal)
    lpt = bh;
  lookat_pt = glm::vec3(cam_trans*glm::vec4(0.0,lpt,0.0,1.0));
  //Creating the lookat matrix
  glm::vec4 init_close = glm::vec4(-3,-3,-3,1.0);
  glm::vec3 lp = glm::vec3(0.0);
  if (c2){
    lp = lookat_pt;
  }
  if (c3){
    lp = lookat_pt;
    c_pos = cam_trans*init_close;
    glm::vec3 temp = glm::vec3(c_pos);
    GLfloat rt = 0.19;
    if (t > final_t && t<camfinal)
      temp = lookat_pt - (1-rt*(t-final_t)/(camfinal-final_t))*(lookat_pt - temp);
    else if (t >= camfinal)
      temp = lookat_pt - (1-rt)*(lookat_pt-temp);
    c_pos = glm::vec4(temp,1.0);
  }
  if (c4){
  	lp = moonpos;
  	GLfloat rat = 1.1;
  	c_pos = glm::translate(glm::mat4(1.0),rat*moonpos)*c_pos;
  }
  lookat_matrix = glm::lookAt(glm::vec3(c_pos),lp,glm::vec3(c_up));

  GLfloat BR = R;
  GLfloat booster_detach_t = 4;
  if (t>booster_detach_t){
    BR = Radius + 0.42 + a*booster_detach_t*booster_detach_t + 2*a*booster_detach_t*(t-booster_detach_t);
    //std::cout<<t<<std::endl;
  }
  if (t>booster_detach_t+10)
  	BR = Radius + 0.42 + a*booster_detach_t*booster_detach_t;
  int BRid = calculateID(BR,a,10.0,1);
  //glm::mat4 booster1_trans_mat = glm::translate(glm::mat4(1.0),glm::vec3(0.0,-BR,0.0));
  glm::mat4 booster1_trans_mat = glm::translate(glm::mat4(1.0),glm::vec3(curve_v_positions[BRid]));
  glm::mat4 booster1_trans = booster1_trans_mat*veh_rot; 

  GLfloat BaseR = R;
  GLfloat base_detach_t = 7.0;
  if (t > base_detach_t){
    BaseR = Radius + 0.42 + a*base_detach_t*base_detach_t + 2*a*base_detach_t*(t-base_detach_t);
    
  }
  //std::cout<<t<<std::endl;
  if (t>base_detach_t+10)
  	BaseR = Radius + 0.42 + a*base_detach_t*base_detach_t;
  int BaseRid = calculateID(BaseR,a,10.0,1);
  // glm::mat4 base_trans_mat = glm::translate(glm::mat4(1.0),glm::vec3(0.0,-BaseR,0.0));
  glm::mat4 base_trans_mat = glm::translate(glm::mat4(1.0),glm::vec3(curve_v_positions[BaseRid]));
  glm::mat4 base_trans = base_trans_mat*veh_rot; 

  GLfloat HoodR = R;
  GLfloat hood_detach_t = 35.0;
  if (t > hood_detach_t){
    HoodR = Radius + 0.42 + a*hood_detach_t*hood_detach_t + 2*a*hood_detach_t*(t-hood_detach_t);
    //std::cout<<t<<std::endl;
  }
  if (t > hood_detach_t+10){
    HoodR = Radius + 0.42 + a*hood_detach_t*hood_detach_t ;
    //std::cout<<t<<std::endl;
  }
  int HoodRid = calculateID(HoodR,a,10.0,1);
  //glm::mat4 hood_trans_mat = glm::translate(glm::mat4(1.0),glm::vec3(0.0,-HoodR,0.0));
  glm::mat4 hood_trans_mat = glm::translate(glm::mat4(1.0),glm::vec3(curve_v_positions[HoodRid]));
  glm::mat4 hood_trans = hood_trans_mat*veh_rot; 
  //std::cout<<t<<std::endl;
  GLfloat central_detach_t = 184.0;
  GLfloat CR = R;
  if (t > central_detach_t)
    CR = Radius + 0.42 + a*central_detach_t*central_detach_t + 2*a*central_detach_t*(t-central_detach_t);
  //glm::mat4 central_trans_mat = glm::translate(glm::mat4(1.0),glm::vec3(0.0,-CR,0.0));
  int CRid = calculateID(CR,a,10.0,1);
  glm::mat4 central_trans_mat = glm::translate(glm::mat4(1.0),glm::vec3(curve_v_positions[CRid]));
  glm::mat4 central_trans = central_trans_mat*veh_rot; 

  GLfloat PR = R + bh;
  int PRid = calculateID(R,a,10.0,1);
  //glm::mat4 pay_trans_mat = glm::translate(glm::mat4(1.0),glm::vec3(0.0,-PR,0.0));
  glm::mat4 pay_trans_mat = glm::translate(glm::mat4(1.0),glm::vec3(curve_v_positions[PRid]));
  glm::mat4 pay_trans = pay_trans_mat*veh_rot; 
  //creating the projection matrix
  //std::cout<<c_pos.x<<" "<<c_pos.y<<" "<<c_pos.z<<std::endl;
  projection_matrix = glm::frustum(-1.0, 1.0, -1.0, 1.0, 1.0, 3.0*(double)edge_len);
  glUseProgram(earth_shaderProgram);
  
  view_matrix = projection_matrix*lookat_matrix;
  glUniform4f(shaderEye,c_pos.x,c_pos.y,c_pos.z,1.0);
  glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
 // Draw the sphere
  modelview_matrix = view_matrix*model_matrix;
  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  glUniform1f(shaderTime, timeElapsed);
  glUniform1f(earthRadius, Radius);
  
  //glUniform1f(outerRadius, 2*Radius);
  normal_matrix = glm::transpose (glm::inverse(glm::mat3(modelview_matrix)));
  glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
  //glBindVertexArray (vao[1]);
  //glDrawArrays(GL_TRIANGLE_STRIP, 0, num_vertices);
  glUseProgram(planetshaderProgram);
  glUniformMatrix4fv(uModelViewMatrix1, 1, GL_FALSE, glm::value_ptr(view_matrix));
  glBindTexture(GL_TEXTURE_2D, spacetex);
  glBindVertexArray (vao[1]);
  glDrawArrays(GL_TRIANGLES, 0, space_num_vertices);
  //glUniformMatrix4fv(uModelViewMatrix1, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  glBindTexture(GL_TEXTURE_2D, moontex);
  glBindVertexArray (moonvao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, moon_tri_idx);
  glUseProgram(earth_shaderProgram);
  glBindTexture(GL_TEXTURE_2D, earthtex);
  glBindVertexArray (vao[0]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, num_vertices);

  glUseProgram(launch_stand_shaderProgram);
  glUniformMatrix4fv(launch_stand_uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  glBindTexture(GL_TEXTURE_2D, sitebasetex);
  glBindVertexArray (vao[2]);
  glDrawArrays(GL_TRIANGLES, 0, 12);
  glDrawArrays(GL_TRIANGLES, 18, 30-18);
  glBindTexture(GL_TEXTURE_2D, conctex);
  glDrawArrays(GL_TRIANGLES, 12, 6);

  glBindTexture(GL_TEXTURE_2D, launchstandtex);
  glDrawArrays(GL_TRIANGLES, 30, 84-30);
  glDrawArrays(GL_TRIANGLES, 228, 396-228);
  glBindTexture(GL_TEXTURE_2D, redtex);
  glDrawArrays(GL_TRIANGLES, 84, 228-84);
  glBindTexture(GL_TEXTURE_2D, armtex);
  glDrawArrays(GL_TRIANGLES, 396, 504 - 396); 

  glUseProgram(texshaderProgram);
  glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix*central_trans));
  glBindVertexArray(vao[10]);
  glBindTexture(GL_TEXTURE_2D,centraltex);
  glDrawArrays(GL_TRIANGLE_STRIP,0,606);
  glBindTexture(GL_TEXTURE_2D,blaster);
  glDrawArrays(GL_TRIANGLE_STRIP,606,808-606);

  glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix*base_trans));
  glBindVertexArray(vao[9]);
  glBindTexture(GL_TEXTURE_2D,blaster);
  glDrawArrays(GL_TRIANGLE_STRIP,0,202);
  glBindTexture(GL_TEXTURE_2D,basetex);
  glDrawArrays(GL_TRIANGLE_STRIP,202,1414-202);

  glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix*hood_trans));
  glBindVertexArray(vao[6]);
  glBindTexture(GL_TEXTURE_2D,hoodtex);
  glDrawArrays(GL_TRIANGLE_STRIP,0,1818);

  glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix*booster1_trans));
  glBindVertexArray(vao[7]);
  glBindTexture(GL_TEXTURE_2D,blaster);
  glDrawArrays(GL_TRIANGLE_STRIP,0,606);
  glBindTexture(GL_TEXTURE_2D,boosterbody);
  glDrawArrays(GL_TRIANGLE_STRIP,606,1212-606);
  glBindTexture(GL_TEXTURE_2D,boosterhead);
  glDrawArrays(GL_TRIANGLE_STRIP,1212,1818-1212);

  glBindVertexArray(vao[8]);
  glBindTexture(GL_TEXTURE_2D,blaster);
  glDrawArrays(GL_TRIANGLE_STRIP,0,606);
  glBindTexture(GL_TEXTURE_2D,boosterbody);
  glDrawArrays(GL_TRIANGLE_STRIP,606,1212-606);
  glBindTexture(GL_TEXTURE_2D,boosterhead);
  glDrawArrays(GL_TRIANGLE_STRIP,1212,1818-1212);

  glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix*pay_trans));
  glBindTexture(GL_TEXTURE_2D, goldtex);
  glBindVertexArray (vao[3]);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDrawArrays(GL_TRIANGLE_STRIP, 36, 120-36);
  glDrawArrays(GL_TRIANGLE_STRIP, 120, 204-120);
  glDrawArrays(GL_TRIANGLE_STRIP, 204, 288-204);
  glDrawArrays(GL_TRIANGLE_STRIP, 288, 372-288);
  glBindTexture(GL_TEXTURE_2D, basetex);
  glDrawArrays(GL_TRIANGLE_STRIP, 372, 2998-372);
  glDrawArrays(GL_TRIANGLES, 2998, 3118-2998);

  if (t>186){
    glBindVertexArray(vao[4]);
    glBindTexture(GL_TEXTURE_2D, armtex);
    glDrawArrays(GL_TRIANGLES,0,27);
    glBindTexture(GL_TEXTURE_2D, paneltex);
    glDrawArrays(GL_TRIANGLES,27,33-27);

    glBindVertexArray(vao[5]);
    glBindTexture(GL_TEXTURE_2D, armtex);
    glDrawArrays(GL_TRIANGLES,0,27);
    glBindTexture(GL_TEXTURE_2D, paneltex);
    glDrawArrays(GL_TRIANGLES,27,33-27);
  }
  glUseProgram(sphereshaderProgram);

  if (loadtrajectories){
    loadtrajectories=false;
    loadTrajectories();
    glGenVertexArrays (1, &vao[13]);
    //Ask GL for two Vertex Buffer Object (vbo)
    glGenBuffers (1, &vbo[13]);
    glBindVertexArray (vao[13]);
    //Set 0 as the current buffer to be used by binding it
    glBindBuffer (GL_ARRAY_BUFFER, vbo[13]);
    glBufferData (GL_ARRAY_BUFFER, sizeof (curve_v_positions) + sizeof(curve_v_colors), NULL, GL_STATIC_DRAW);
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(curve_v_positions), curve_v_positions );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(curve_v_positions), sizeof(curve_v_colors), curve_v_colors);
    // set up vertex array
    //Position
    GLuint vPosition = glGetAttribLocation( sphereshaderProgram, "vPosition" );
    GLuint vColor = glGetAttribLocation( sphereshaderProgram, "vColor" ); 
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    //Textures
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(curve_v_positions)) );
  }
  if (!c1 && mouseclicked){
  	mouseclicked = false;
    float mouse_current_z;
    glReadPixels(xPos, yPos, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouse_current_z);
    if (mouse_current_z > 0.973)
      mouse_current_z = 0.973;
    //std::cout<<xPos<<" "<<yPos<<" "<<mouse_current_z<<std::endl;
  	glm::vec3 wrld=glm::unProject(glm::vec3(xPos,yPos,mouse_current_z),lookat_matrix*model_matrix,projection_matrix,glm::vec4(0,0,width,height));
    wrld.z = -wrld.z;
  	//std::cout<<wrld.x<<" "<<wrld.y<<" "<<wrld.z<<std::endl;
  	control_points.push_back(wrld);
  	//point_trans = glm::translate(glm::mat4(1.0),wrld);
    if ((control_points.size()-startingControlPoints)>1){
      //std::cout<<startingControlPoints<<std::endl;
      int startpoint = 0;
      if (trajectories.size()>1)
        startpoint = trajectories[trajectories.size()-2];
	  	drawBezier(startingControlPoints,control_points.size(),startpoint,true,6);
      trajectories[trajectories.size()-1]=curve_tri_idx;
	  	glGenVertexArrays (1, &vao[13]);
	    //Ask GL for two Vertex Buffer Object (vbo)
	    glGenBuffers (1, &vbo[13]);
	  	glBindVertexArray (vao[13]);
	    //Set 0 as the current buffer to be used by binding it
	    glBindBuffer (GL_ARRAY_BUFFER, vbo[13]);
	    glBufferData (GL_ARRAY_BUFFER, sizeof (curve_v_positions) + sizeof(curve_v_colors), NULL, GL_STATIC_DRAW);
	    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(curve_v_positions), curve_v_positions );
	    glBufferSubData( GL_ARRAY_BUFFER, sizeof(curve_v_positions), sizeof(curve_v_colors), curve_v_colors);
	    // set up vertex array
	    //Position
	  	GLuint vPosition = glGetAttribLocation( sphereshaderProgram, "vPosition" );
	    GLuint vColor = glGetAttribLocation( sphereshaderProgram, "vColor" ); 
	    glEnableVertexAttribArray( vPosition );
	    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	    //Textures
	    glEnableVertexAttribArray( vColor );
	    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(curve_v_positions)) );
  	}
  }
  if (control_points.size()>1){
  	glBindVertexArray (vao[13]);
	glBindBuffer (GL_ARRAY_BUFFER, vbo[13]);
  	glUniformMatrix4fv(spheretransMatrix,1,GL_FALSE,glm::value_ptr(glm::mat4(1.0)));
	glUniformMatrix4fv(sphereumodelViewMatrix,1,GL_FALSE,glm::value_ptr(modelview_matrix));
  if (trajectories.size()>1){
    for (int i=0;i<trajectories.size()-1;i++)
  	 glDrawArrays(GL_LINE_STRIP, trajectories[i], trajectories[i+1]-trajectories[i]);
  }
  else
    glDrawArrays(GL_LINE_STRIP,0,curve_tri_idx);
  }
  glBindVertexArray (vao[12]);
  for (int i=startingControlPoints;i<control_points.size();i++){
    glm::mat4 point_trans = glm::translate(glm::mat4(1.0),control_points[i]);
    glUniformMatrix4fv(spheretransMatrix,1,GL_FALSE,glm::value_ptr(point_trans));
    glUniformMatrix4fv(sphereumodelViewMatrix,1,GL_FALSE,glm::value_ptr(modelview_matrix));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, point_tri_idx);
  }
  //smokeParticles------------------------------------------------------------------
  if (savetrajectories){
    savetrajectories = false;
    saveTrajectories();
  }
  if (launched){

	  GLuint nr_new_particles;
	  if (t<base_detach_t)
	  	nr_new_particles = 80;
	  else 
	  	nr_new_particles = 0;
	  GLfloat dt = 0.025;
	  // Add new particles
	  for (GLuint i = 0; i < nr_new_particles; ++i)
	  {
	    int unusedParticle = FirstUnusedParticle();
	    if (t<booster_detach_t)
	    	RespawnParticle(smokeparticles[unusedParticle],0.35,2,1.5,1.0);
	    else
	    	RespawnParticle(smokeparticles[unusedParticle],0.15,1.8,1.5,0.7);
	  }
	  // Uupdate all particles
	  for (GLuint i = 0; i < nr_particles; ++i)
	  {
	    Particle &p = smokeparticles[i];
	    p.Life -= dt; // reduce life
	    if (p.Life > 0.0f)
	    {	// particle is alive, thus update
	    	glm::vec3 tempos = glm::vec3(p.Position);
	    	tempos += dt*p.Velocity;
	    	p.Position = glm::vec4(tempos,1.0);
	    	//p.Position.y -= 1.5*dt;
	        p.Color.a = 1.0;
	    }
	  }  

	  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	  glUseProgram(particleshaderProgram);
	  glBindTexture(GL_TEXTURE_2D, smoketex);
	  glBindVertexArray (vao[11]);
	  for (Particle particle : smokeparticles)
	  {
	    if (particle.Life > 0.0f)
	    {
	    	glm::mat4 ptrans = glm::translate(glm::mat4(1.0),glm::vec3(particle.Position));
	    	glUniformMatrix4fv(particlemat1, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	    	glUniformMatrix4fv(particlemat2, 1, GL_FALSE, glm::value_ptr(base_trans));
	    	glUniformMatrix4fv(particlemat3, 1, GL_FALSE, glm::value_ptr(ptrans));
	    	glUniform4fv(particleColor,1,glm::value_ptr(particle.Color));
	        glDrawArrays(GL_TRIANGLES, 0, smoke_num_vertices);
	    } 
	  } 
	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
}

void mouseButtonCallback (GLFWwindow *window, int button, int action, int mods){
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		
		mouseclicked = true;
	}
}
int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;
  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow((int) width, (int) height, "CS475/CS675 Assignment3: Launch", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }
  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);


  //Initialize GL state
  csX75::initGL();
  loadModelNames();
  loadModels();
  initBuffersGL();
  //saveModel();
//std::cout<<tri_idx<<std::endl;
  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
      glfwGetCursorPos(window, &xPos , &yPos);
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------

