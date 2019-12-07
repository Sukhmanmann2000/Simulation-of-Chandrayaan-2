/*
  CSX75 Tutorial 6 Texturing a Cube


  Modified from An Introduction to OpenGL Programming, 
  Ed Angel and Dave Shreiner, SIGGRAPH 2013

  Written by Aditya Prakash, 2015
*/
//////////// LOAD TEXTURES
///////// DEBUG

#include "launch_site.hpp"
#include "launch_site_texture.hpp"

GLuint texshaderProgram,simpleshaderProgram;
GLuint vbo[3], vao[3];
GLuint basetex,sitebasetex,launchstandtex,armtex,redtex,conctex;
int idx_arr[5],idx=0;
glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;


glm::mat4 modelview_matrix;
glm::mat3 normal_matrix;

GLuint texuModelViewMatrix,simpleuModelViewMatrix;
GLuint viewMatrix;
GLuint normalMatrix;
//-----------------------------------------------------------------

//6 faces, 2 triangles/face, 3 vertices/triangle
const int num_vertices = 36;
float edge_len = 10;
glm::vec4 texCoordinates[8];
//Eight vertices in homogenous coordinates
glm::vec4 positions[8] = {
  glm::vec4(-edge_len, -edge_len, edge_len, 1.0),
  glm::vec4(-edge_len, edge_len, edge_len, 1.0),
  glm::vec4(edge_len, edge_len, edge_len, 1.0),
  glm::vec4(edge_len, -edge_len, edge_len, 1.0),
  glm::vec4(-edge_len, -edge_len, -edge_len, 1.0),
  glm::vec4(-edge_len, edge_len, -edge_len, 1.0),
  glm::vec4(edge_len, edge_len, -edge_len, 1.0),
  glm::vec4(edge_len, -edge_len, -edge_len, 1.0)
};

glm::vec4 normals[8] = {
  glm::vec4(-edge_len, -edge_len, edge_len, 1.0),
  glm::vec4(-edge_len, edge_len, edge_len, 1.0),
  glm::vec4(edge_len, edge_len, edge_len, 1.0),
  glm::vec4(edge_len, -edge_len, edge_len, 1.0),
  glm::vec4(-edge_len, -edge_len, -edge_len, 1.0),
   glm::vec4(-edge_len, edge_len, -edge_len, 1.0),
  glm::vec4(edge_len, edge_len, -edge_len, 1.0),
  glm::vec4(edge_len, -edge_len, -edge_len, 1.0)
};
//RGBA colors
glm::vec4 colors[8] = {
  glm::vec4(0.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 1.0, 0.0, 1.0),
  glm::vec4(0.0, 1.0, 0.0, 1.0),
  glm::vec4(0.0, 0.0, 1.0, 1.0),
  glm::vec4(1.0, 0.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(0.0, 1.0, 1.0, 1.0)
};

glm::vec2 t_coords[4] = {
  glm::vec2( 0.0, 0.0),
  glm::vec2( 0.0, 1.0),
  glm::vec2( 1.0, 0.0),
  glm::vec2( 1.0, 1.0)
};

const int num_vertices_base = 6;
int base_tri_idx = 0;
glm::vec4 base_v_positions[num_vertices_base];
glm::vec2 base_tex_coords[num_vertices_base];
glm::vec4 base_positions[4] = {
  glm::vec4(2*edge_len, -edge_len, edge_len, 1.0),
  glm::vec4(-2*edge_len, -edge_len, edge_len, 1.0),
  glm::vec4(-2*edge_len, -edge_len, -edge_len, 1.0),
  glm::vec4(2*edge_len, -edge_len, -edge_len, 1.0)
};
void drawbase(){
  base_v_positions[base_tri_idx] = base_positions[0]; 
  base_tex_coords[base_tri_idx] = t_coords[1];
  base_tri_idx++;
  base_v_positions[base_tri_idx] = base_positions[1];
  base_tex_coords[base_tri_idx] = t_coords[0];
  base_tri_idx++;
  base_v_positions[base_tri_idx] = base_positions[2]; 
  base_tex_coords[base_tri_idx] = t_coords[2];
  base_tri_idx++;
  base_v_positions[base_tri_idx] = base_positions[0]; 
  base_tex_coords[base_tri_idx] = t_coords[1];
  base_tri_idx++;
  base_v_positions[base_tri_idx] = base_positions[2]; 
  base_tex_coords[base_tri_idx] = t_coords[2];
  base_tri_idx++;
  base_v_positions[base_tri_idx] = base_positions[3]; 
  base_tex_coords[base_tri_idx] = t_coords[3];
  base_tri_idx++;
}
float site_base_edge = edge_len/2.5;
float site_base_height = site_base_edge/4;

const int launch_stand_num_vertices = 550;
int launch_stand_tri_idx = 0;
glm::vec4 launch_stand_v_positions[launch_stand_num_vertices];
glm::vec4 launch_stand_v_colors[launch_stand_num_vertices];
glm::vec2 launch_stand_tex_coords[launch_stand_num_vertices];
float launch_stand_base_height = site_base_height+site_base_edge/3.4;
float launch_stand_tot_height = launch_stand_base_height + edge_len;
float launch_offset_x = site_base_edge/4;
float launch_offset_y = site_base_edge/5;
float launch_width_control = site_base_edge/4;
glm::vec4 launch_stand_color = glm::vec4(1.0,0.0,0.0,1.0);

glm::vec4 site_base_positions[8] = {
  glm::vec4(-site_base_edge/1.2, -edge_len, site_base_edge*1.5, 1.0),
  glm::vec4(-site_base_edge/1.2, -edge_len+site_base_height, site_base_edge*1.5, 1.0),
  glm::vec4(site_base_edge/1.2, -edge_len+site_base_height, site_base_edge*1.5, 1.0),
  glm::vec4(site_base_edge/1.2, -edge_len, site_base_edge*1.5, 1.0),
  glm::vec4(-site_base_edge/1.2, -edge_len, -site_base_edge*1.5, 1.0),
  glm::vec4(-site_base_edge/1.2, -edge_len+site_base_height, -site_base_edge*1.5, 1.0),
  glm::vec4(site_base_edge/1.2, -edge_len+site_base_height, -site_base_edge*1.5, 1.0),
  glm::vec4(site_base_edge/1.2, -edge_len, -site_base_edge*1.5, 1.0)
};
glm::vec4 site_base_color = glm::vec4(1.0,1.0,1.0,1.0);
void site_base_quad(int a, int b, int c, int d)
{
  launch_stand_tex_coords[launch_stand_tri_idx] = t_coords[1];
  launch_stand_v_colors[launch_stand_tri_idx] = launch_stand_color;
  launch_stand_v_positions[launch_stand_tri_idx] = site_base_positions[a]; 
  launch_stand_tri_idx++;
  launch_stand_tex_coords[launch_stand_tri_idx] = t_coords[0];
  launch_stand_v_colors[launch_stand_tri_idx] = launch_stand_color; 
  launch_stand_v_positions[launch_stand_tri_idx] = site_base_positions[b];
  launch_stand_tri_idx++;
  launch_stand_tex_coords[launch_stand_tri_idx] = t_coords[2];
  launch_stand_v_colors[launch_stand_tri_idx] = launch_stand_color; 
  launch_stand_v_positions[launch_stand_tri_idx] = site_base_positions[c]; 
  launch_stand_tri_idx++;
  launch_stand_tex_coords[launch_stand_tri_idx] = t_coords[1];
  launch_stand_v_colors[launch_stand_tri_idx] = launch_stand_color; 
  launch_stand_v_positions[launch_stand_tri_idx] = site_base_positions[a]; 
  launch_stand_tri_idx++;
  launch_stand_tex_coords[launch_stand_tri_idx] = t_coords[2];
  launch_stand_v_colors[launch_stand_tri_idx] = launch_stand_color; 
  launch_stand_v_positions[launch_stand_tri_idx] = site_base_positions[c]; 
  launch_stand_tri_idx++;
  launch_stand_tex_coords[launch_stand_tri_idx] = t_coords[3];
  launch_stand_v_colors[launch_stand_tri_idx] = launch_stand_color; 
  launch_stand_v_positions[launch_stand_tri_idx] = site_base_positions[d]; 
  launch_stand_tri_idx++;
 }
 void drawSiteBase(){
  site_base_quad( 1, 0, 3, 2);
  site_base_quad( 2, 3, 7, 6);
  //site_base_quad( 3, 0, 4, 7);
  //std::cout<<site_base_tri_idx<<std::endl;
  site_base_quad( 6, 5, 1, 2);
  //std::cout<<site_base_tri_idx<<std::endl;
  site_base_quad( 4, 5, 6, 7);
  site_base_quad( 5, 4, 0, 1);
  idx_arr[idx] = launch_stand_tri_idx;idx++;
  //std::cout<<site_base_tri_idx<<std::endl;
 }

glm::vec4 launch_stand_positions[8] = {
  glm::vec4(-site_base_edge/1.5, -edge_len+site_base_height, -site_base_edge*1.5 + site_base_edge/4, 1.0),
  glm::vec4(-site_base_edge/1.5 + launch_offset_x, -edge_len+launch_stand_base_height, -site_base_edge*1.5 + site_base_edge/4+launch_offset_y, 1.0),
  glm::vec4(site_base_edge/1.5 - launch_offset_x, -edge_len+launch_stand_base_height, -site_base_edge*1.5 + site_base_edge/4+launch_offset_y, 1.0),
  glm::vec4(site_base_edge/1.5, -edge_len+site_base_height, -site_base_edge*1.5 + site_base_edge/4, 1.0),
  glm::vec4(-site_base_edge/1.5, -edge_len+site_base_height, -site_base_edge*0.2, 1.0),
  glm::vec4(-site_base_edge/1.5 + launch_offset_x, -edge_len+launch_stand_base_height, -site_base_edge*0.2-launch_offset_y, 1.0),
  glm::vec4(site_base_edge/1.5 - launch_offset_x, -edge_len+launch_stand_base_height, -site_base_edge*0.2-launch_offset_y, 1.0),
  glm::vec4(site_base_edge/1.5, -edge_len+site_base_height, -site_base_edge*0.2, 1.0)
};
glm::vec4 launch_stand_positions1[8] = {
  glm::vec4(-site_base_edge/1.5 + launch_offset_x + launch_width_control, -edge_len+launch_stand_base_height, -site_base_edge*1.5 + site_base_edge/4+launch_offset_y+launch_width_control, 1.0),
  glm::vec4(-site_base_edge/1.5 + launch_offset_x+launch_width_control, -edge_len+launch_stand_tot_height, -site_base_edge*1.5 + site_base_edge/4+launch_offset_y+launch_width_control, 1.0),
  glm::vec4(site_base_edge/1.5 - launch_offset_x-launch_width_control, -edge_len+launch_stand_tot_height, -site_base_edge*1.5 + site_base_edge/4+launch_offset_y+launch_width_control, 1.0),
  glm::vec4(site_base_edge/1.5 - launch_offset_x-launch_width_control, -edge_len+launch_stand_base_height, -site_base_edge*1.5 + site_base_edge/4+launch_offset_y+launch_width_control, 1.0),
  glm::vec4(-site_base_edge/1.5 + launch_offset_x+launch_width_control, -edge_len+launch_stand_base_height, -site_base_edge*0.2-launch_offset_y-launch_width_control, 1.0),
  glm::vec4(-site_base_edge/1.5 + launch_offset_x+launch_width_control, -edge_len+launch_stand_tot_height, -site_base_edge*0.2-launch_offset_y-launch_width_control, 1.0),
  glm::vec4(site_base_edge/1.5 - launch_offset_x-launch_width_control, -edge_len+launch_stand_tot_height, -site_base_edge*0.2-launch_offset_y-launch_width_control, 1.0),
  glm::vec4(site_base_edge/1.5 - launch_offset_x-launch_width_control, -edge_len+launch_stand_base_height, -site_base_edge*0.2-launch_offset_y-launch_width_control, 1.0)
};
void launch_stand_quad(int a, int b, int c, int d,glm::vec4 *p)
{
  launch_stand_tex_coords[launch_stand_tri_idx] = t_coords[1];
  launch_stand_v_colors[launch_stand_tri_idx] = launch_stand_color;
  launch_stand_v_positions[launch_stand_tri_idx] = p[a];
  launch_stand_tri_idx++;
  launch_stand_tex_coords[launch_stand_tri_idx] = t_coords[0];
  launch_stand_v_colors[launch_stand_tri_idx] = launch_stand_color; 
  launch_stand_v_positions[launch_stand_tri_idx] = p[b];
  launch_stand_tri_idx++;
  launch_stand_tex_coords[launch_stand_tri_idx] = t_coords[2];
  launch_stand_v_colors[launch_stand_tri_idx] = launch_stand_color; 
  launch_stand_v_positions[launch_stand_tri_idx] = p[c];
  launch_stand_tri_idx++;
  launch_stand_tex_coords[launch_stand_tri_idx] = t_coords[1];
  launch_stand_v_colors[launch_stand_tri_idx] = launch_stand_color; 
  launch_stand_v_positions[launch_stand_tri_idx] = p[a];
  launch_stand_tri_idx++;
  launch_stand_tex_coords[launch_stand_tri_idx] = t_coords[2];
  launch_stand_v_colors[launch_stand_tri_idx] = launch_stand_color; 
  launch_stand_v_positions[launch_stand_tri_idx] = p[c];
  launch_stand_tri_idx++;
  launch_stand_tex_coords[launch_stand_tri_idx] = t_coords[3];
  launch_stand_v_colors[launch_stand_tri_idx] = launch_stand_color; 
  launch_stand_v_positions[launch_stand_tri_idx] = p[d];
  launch_stand_tri_idx++;
 }
 float arm_width = site_base_edge/8 ;
 float arm_offset = 0;
 float arm_x = site_base_edge/6;
void draw_launch_stand_arm(GLfloat x,GLfloat y,int dir){
  GLfloat h = -edge_len+launch_stand_base_height+y;
  GLfloat zh = -site_base_edge*0.2-launch_offset_y-arm_offset;
  glm::vec4 arm_positions[12] = {
    glm::vec4(x,h,zh,1.0),
    glm::vec4(x,h+arm_width,zh,1.0),
    glm::vec4(x + dir*arm_x,h+arm_width,zh+arm_x,1.0),
    glm::vec4(x + dir*arm_x,h,zh+arm_x,1.0),
    glm::vec4(x + dir*arm_x,h,zh+arm_x,1.0),
    glm::vec4(x + dir*arm_x,h+arm_width,zh+arm_x,1.0),
    glm::vec4(x + dir*arm_x,h+arm_width,zh+arm_x+site_base_edge,1.0),
    glm::vec4(x + dir*arm_x,h,zh+arm_x+site_base_edge,1.0),
    glm::vec4(x + dir*arm_x,h,zh+arm_x+site_base_edge,1.0),
    glm::vec4(x + dir*arm_x,h+arm_width,zh+arm_x+site_base_edge,1.0),
    glm::vec4(x - dir*arm_x,h+arm_width,zh+arm_x + 13*site_base_edge/12,1.0),
    glm::vec4(x - dir*arm_x,h,zh+arm_x + 13*site_base_edge/12,1.0)
  };
  launch_stand_quad(3,0,1,2,arm_positions);
  launch_stand_quad(4,5,6,7,arm_positions);
  launch_stand_quad(8,9,10,11,arm_positions);
}
void draw_launch_stand_tunnel(float y){
  GLfloat w = site_base_edge/8;
  GLfloat tlen = site_base_edge/3;
  GLfloat h = -edge_len+launch_stand_base_height+y;
  GLfloat zh = -site_base_edge*0.2-launch_offset_y;
  glm::vec4 tunnel_positions[8] = {
    glm::vec4(w,h,zh,1.0),
    glm::vec4(w,h+2*w,zh,1.0),
    glm::vec4(w,h+2*w,zh+tlen,1.0),
    glm::vec4(w,h,zh+tlen,1.0),
    glm::vec4(-w,h,zh,1.0),
    glm::vec4(-w,h+2*w,zh,1.0),
    glm::vec4(-w,h+2*w,zh+tlen,1.0),
    glm::vec4(-w,h,zh+tlen,1.0)
  };
  launch_stand_quad(0,1,2,3,tunnel_positions);
  launch_stand_quad(4,5,6,7,tunnel_positions);
  launch_stand_quad(0,4,7,3,tunnel_positions);
  launch_stand_quad(1,5,6,2,tunnel_positions);
}
void draw_launch_stand_pillars(){
  float boundx = site_base_edge/1.5 - launch_offset_x;
  float boundy = (launch_stand_positions1[4].z - launch_stand_positions1[0].z)/2 + launch_width_control;
  float t = (launch_stand_positions1[4].z + launch_stand_positions1[0].z)/2;
  GLfloat scaling = 0.3;
  glm::vec4 base_pillar[8],pillar1[8],pillar2[8],pillar3[8],pillar4[8];
  for (int j=0;j<8;j++){
    base_pillar[j] = glm::vec4(scaling,1.0,scaling,1.0)*launch_stand_positions1[j];
  }
  float tn = (base_pillar[0].z + base_pillar[4].z)/2;
  for (int i=0;i<8;i++){
    base_pillar[i].z += t-tn; 
  }
  float wx = scaling * (boundx - launch_width_control);
  float wy = scaling * (boundy - launch_width_control);
  for (int i=0;i<8;i++){
    glm::vec4 temp = base_pillar[i];
    pillar1[i] = glm::vec4(boundx - wx + temp.x,temp.y,boundy - wy+temp.z,1.0);
    pillar2[i] = glm::vec4(-boundx + wx + temp.x,temp.y,boundy - wy+temp.z,1.0);
    pillar3[i] = glm::vec4(boundx - wx + temp.x,temp.y,-boundy + wy+temp.z,1.0);
    pillar4[i] = glm::vec4(-boundx + wx + temp.x,temp.y,-boundy + wy+temp.z,1.0);
  }
  //std::cout<<launch_stand_tri_idx<<std::endl;
  launch_stand_quad( 1, 0, 3, 2,pillar1);
  launch_stand_quad( 2, 3, 7, 6,pillar1);
  //launch_stand_quad( 3, 0, 4, 7);
  launch_stand_quad( 6, 5, 1, 2,pillar1);
  launch_stand_quad( 4, 5, 6, 7,pillar1);
  launch_stand_quad( 5, 4, 0, 1,pillar1);

  launch_stand_quad( 1, 0, 3, 2,pillar2);
  launch_stand_quad( 2, 3, 7, 6,pillar2);
  //launch_stand_quad( 3, 0, 4, 7);
  launch_stand_quad( 6, 5, 1, 2,pillar2);
  launch_stand_quad( 4, 5, 6, 7,pillar2);
  launch_stand_quad( 5, 4, 0, 1,pillar2);

  launch_stand_quad( 1, 0, 3, 2,pillar3);
  launch_stand_quad( 2, 3, 7, 6,pillar3);
  //launch_stand_quad( 3, 0, 4, 7);
  launch_stand_quad( 6, 5, 1, 2,pillar3);
  launch_stand_quad( 4, 5, 6, 7,pillar3);
  launch_stand_quad( 5, 4, 0, 1,pillar3);

  launch_stand_quad( 1, 0, 3, 2,pillar4);
  launch_stand_quad( 2, 3, 7, 6,pillar4);
  //launch_stand_quad( 3, 0, 4, 7);
  launch_stand_quad( 6, 5, 1, 2,pillar4);
  launch_stand_quad( 4, 5, 6, 7,pillar4);
  launch_stand_quad( 5, 4, 0, 1,pillar4);
  //std::cout<<launch_stand_tri_idx<<std::endl;
  idx_arr[idx] = launch_stand_tri_idx;idx++;
  float num_planes = 10;
  float sw = wx;
  float bh = -edge_len + launch_stand_base_height ;
  float th = (launch_stand_tot_height-launch_stand_base_height)/num_planes;
  float bz1 = launch_stand_positions1[0].z - launch_width_control;
  float bz2 = launch_stand_positions1[4].z + launch_width_control;
  for (int i=0;i<num_planes;i++){
    bh += th;
    glm::vec4 temp_pos[10] = {
      glm::vec4(boundx,bh,bz1,1.0),
      glm::vec4(boundx,bh,bz2,1.0),
      glm::vec4(-boundx,bh,bz2,1.0),
      glm::vec4(-boundx,bh,bz1,1.0),
      glm::vec4(-boundx,bh-sw,bz1,1.0),
      glm::vec4(-boundx,bh-sw+th,bz2,1.0),
      glm::vec4(-boundx,bh+th,bz2,1.0),
      glm::vec4(boundx,bh-sw,bz1,1.0),
      glm::vec4(boundx-wx,bh-sw+th,bz2,1.0),
      glm::vec4(boundx,bh+th,bz2,1.0),
    };
    launch_stand_quad(0,1,2,3,temp_pos);
    if (i<num_planes-1){
      launch_stand_quad(3,4,5,6,temp_pos);
      launch_stand_quad(0,7,8,9,temp_pos);
    }
  }

}
void draw_launch_stand(){
  launch_stand_quad( 1, 0, 3, 2,launch_stand_positions);
  launch_stand_quad( 2, 3, 7, 6,launch_stand_positions);
  //launch_stand_quad( 3, 0, 4, 7);
  launch_stand_quad( 6, 5, 1, 2,launch_stand_positions);
  launch_stand_quad( 4, 5, 6, 7,launch_stand_positions);
  launch_stand_quad( 5, 4, 0, 1,launch_stand_positions);
  launch_stand_quad( 1, 0, 3, 2,launch_stand_positions1);
  launch_stand_quad( 2, 3, 7, 6,launch_stand_positions1);
  //launch_stand_quad( 3, 0, 4, 7);
  //launch_stand_quad( 6, 5, 1, 2,launch_stand_positions1);
  launch_stand_quad( 4, 5, 6, 7,launch_stand_positions1);
  launch_stand_quad( 5, 4, 0, 1,launch_stand_positions1);
  idx_arr[idx] = launch_stand_tri_idx;idx++;
  draw_launch_stand_tunnel(site_base_edge*0.8);
  draw_launch_stand_pillars();
  idx_arr[idx] = launch_stand_tri_idx;idx++;
  //std::cout<<launch_stand_tri_idx<<std::endl;
  draw_launch_stand_arm(site_base_edge/1.5 - launch_offset_x,site_base_edge,1);
  draw_launch_stand_arm(site_base_edge/1.5 - launch_offset_x,site_base_edge/3,1);
  draw_launch_stand_arm(site_base_edge/1.5 - launch_offset_x,site_base_edge/5,1);
  draw_launch_stand_arm(-site_base_edge/1.5 + launch_offset_x,site_base_edge,-1);
  draw_launch_stand_arm(-site_base_edge/1.5 + launch_offset_x,site_base_edge/3,-1);
  draw_launch_stand_arm(-site_base_edge/1.5 + launch_offset_x,site_base_edge/5,-1);
  idx_arr[idx] = launch_stand_tri_idx;idx++;
  //std::cout<<launch_stand_tri_idx<<std::endl;
  //draw_launch_stand_arm(site_base_edge/1.5 - launch_offset_x,1.7*site_base_edge);
}
//-----------------------------------------------------------------
void saveModel(){
  std::ofstream fout;
  fout.open("data/launch_site.OBJ");
  for (int i=0;i<launch_stand_tri_idx;i++){
    fout<<"v "<<launch_stand_v_positions[i].x<<" "<<launch_stand_v_positions[i].y<<" "<<launch_stand_v_positions[i].z<<std::endl;
    fout<<"vt "<<launch_stand_tex_coords[i].x<<" "<<launch_stand_tex_coords[i].y<<std::endl;
  }
  fout.close();
}
void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("src/launch_site_simple_tex_vshader.glsl");
  std::string fragment_shader_file("src/launch_site_simple_tex_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  texshaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( texshaderProgram );

  // getting the attributes from the shader program
  GLuint vPosition = glGetAttribLocation( texshaderProgram, "vPosition" );
  GLuint texCoord = glGetAttribLocation( texshaderProgram, "texCoord" ); 
  texuModelViewMatrix = glGetUniformLocation( texshaderProgram, "uModelViewMatrix");
  
  // Load Textures 
  basetex=LoadTexture("src/images/grass_tex.bmp",700,465);
  glBindTexture(GL_TEXTURE_2D, basetex);

  //Ask GL for two Vertex Attribute Objects (vao) , one for the sphere and one for the wireframe
  glGenVertexArrays (2, vao);
  //Ask GL for two Vertex Buffer Object (vbo)
  glGenBuffers (2, vbo);

  //Set 0 as the current array to be used by binding it
  glBindVertexArray (vao[0]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);

  drawbase();

  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (base_v_positions) + sizeof(base_tex_coords), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(base_v_positions), base_v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(base_v_positions), sizeof(base_tex_coords), base_tex_coords);
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(base_v_positions)) );

  glBindVertexArray (vao[1]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);
  sitebasetex=LoadTexture("src/images/base_tex.bmp",376,376);
  drawSiteBase();

  //Copy the points into the current buffer
  launchstandtex=LoadTexture("src/images/base_tex.bmp",376,376);
  draw_launch_stand();

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
  //for (int i=0;i<5;i++)
  //  std::cout<<idx_arr[i]<<std::endl;
  /*vertex_shader_file = "simple_vshader.glsl";
  fragment_shader_file = "simple_fshader.glsl";
  std::vector<GLuint> shaderList1;
  shaderList1.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList1.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  simpleshaderProgram = csX75::CreateProgramGL(shaderList1);
  glUseProgram( simpleshaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( simpleshaderProgram, "vPosition" );
  GLuint vColor = glGetAttribLocation( simpleshaderProgram, "vColor" ); 
  simpleuModelViewMatrix = glGetUniformLocation( simpleshaderProgram, "uModelViewMatrix");*/
}

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
  //Creating the lookat matrix
  lookat_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));

  //creating the projection matrix
  projection_matrix = glm::frustum(-1.0, 1.0, -1.0, 1.0, 0.5, 50.0);

  view_matrix = projection_matrix*lookat_matrix;

  //glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));

 // Draw the sphere
  modelview_matrix = view_matrix*model_matrix*glm::translate(glm::mat4(1.0),glm::vec3(0.0,3.5,0.0));
  normal_matrix = glm::transpose (glm::inverse(glm::mat3(modelview_matrix)));
  //  glBindTexture(GL_TEXTURE_2D, tex);
  glUseProgram(texshaderProgram);
  glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));

  glBindTexture(GL_TEXTURE_2D, basetex);
  glBindVertexArray (vao[0]);
  //glDrawArrays(GL_TRIANGLES, 0, num_vertices_base);
  //glUseProgram(simpleshaderProgram);
  //glUniformMatrix4fv(simpleuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
  glBindTexture(GL_TEXTURE_2D, sitebasetex);
  glBindVertexArray (vao[1]);
  glDrawArrays(GL_TRIANGLES, 0, 12);
  glDrawArrays(GL_TRIANGLES, 18, 30-18);
  glBindTexture(GL_TEXTURE_2D, conctex);
  glDrawArrays(GL_TRIANGLES, 12, 6);

  glBindTexture(GL_TEXTURE_2D, launchstandtex);
  glDrawArrays(GL_TRIANGLES, idx_arr[0], idx_arr[1]-idx_arr[0]);
  glDrawArrays(GL_TRIANGLES, idx_arr[2], idx_arr[3]-idx_arr[2]);
  glBindTexture(GL_TEXTURE_2D, redtex);
  glDrawArrays(GL_TRIANGLES, idx_arr[1], idx_arr[2]-idx_arr[1]);
  glBindTexture(GL_TEXTURE_2D, armtex);
  glDrawArrays(GL_TRIANGLES, idx_arr[3], idx_arr[4] - idx_arr[3]);
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
  window = glfwCreateWindow(512, 512, "CS475/CS675 Assignment3: Launch Site Model", NULL, NULL);
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

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();
  saveModel();
  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
       
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

