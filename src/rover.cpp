/*
  CSX75 Tutorial 6 Texturing a Cube


  Modified from An Introduction to OpenGL Programming, 
  Ed Angel and Dave Shreiner, SIGGRAPH 2013

  Written by Aditya Prakash, 2015
*/
//////////// LOAD TEXTURES
///////// DEBUG

#include "rover.hpp"
#include "rover_texture.hpp"
double PI=3.14159265;
GLuint texshaderProgram,simpleshaderProgram;
GLuint vbo[8], vao[8];
GLuint basetex,sitebasetex,launchstandtex,armtex,goldtex,paneltex,hoodtex,boosterhead,boosterbody,blaster,centraltex;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;
glm::mat4 model_matrix;
glm::mat4 view_matrix;


glm::mat4 modelview_matrix;
glm::mat3 normal_matrix;

glm::vec4 panel1_pivot,panel2_pivot;
GLuint texuModelViewMatrix,simpleuModelViewMatrix;
GLuint viewMatrix;
GLuint normalMatrix;
//-----------------------------------------------------------------

//6 faces, 2 triangles/face, 3 vertices/triangle
const int pay_cube_num_vertices = 10000;
float pay_cube_edge_len = 7;
glm::vec4 texCoordinates[8];
int idx_array[15],arr_id = 0;
int rov_arr[15],rid = 0;
//Eight vertices in homogenous coordinates
glm::vec4 pay_cube_positions[8] = {
  glm::vec4(-pay_cube_edge_len, -pay_cube_edge_len, pay_cube_edge_len, 1.0),
  glm::vec4(-pay_cube_edge_len, pay_cube_edge_len, pay_cube_edge_len, 1.0),
  glm::vec4(pay_cube_edge_len, pay_cube_edge_len, pay_cube_edge_len, 1.0),
  glm::vec4(pay_cube_edge_len, -pay_cube_edge_len, pay_cube_edge_len, 1.0),
  glm::vec4(-pay_cube_edge_len, -pay_cube_edge_len, -pay_cube_edge_len, 1.0),
  glm::vec4(-pay_cube_edge_len, pay_cube_edge_len, -pay_cube_edge_len, 1.0),
  glm::vec4(pay_cube_edge_len, pay_cube_edge_len, -pay_cube_edge_len, 1.0),
  glm::vec4(pay_cube_edge_len, -pay_cube_edge_len, -pay_cube_edge_len, 1.0)
};

glm::vec2 t_coords[4] = {
  glm::vec2( 0.0, 0.0),
  glm::vec2( 0.0, 1.0),
  glm::vec2( 1.0, 0.0),
  glm::vec2( 1.0, 1.0)
};

glm::vec4 color(0.6, 0.6, 0.6, 1.0);
glm::vec4 black(0.1, 0.1, 0.1, 1.0);
glm::vec4 white(0.2, 0.7, 0.7, 1.0);
glm::vec4 red(1.0, 0.2, 0.2, 1.0);
glm::vec4 yellow(0.8, 0.8, 0.0, 1.0);
glm::vec4 green(0.2, 0.7, 0.2, 1.0);
glm::vec4 blue(0.2, 0.2, 0.7, 1.0);

int pay_cube_tri_idx=0;
glm::vec4 pay_cube_v_positions[pay_cube_num_vertices];
glm::vec2 pay_cube_tex_coords[pay_cube_num_vertices];
// quad generates two triangles for each face and assigns colors to the vertices
void pay_cube_quad(int a, int b, int c, int d, glm::vec4 *p)
{
  pay_cube_v_positions[pay_cube_tri_idx] = p[a]; 
  pay_cube_tex_coords[pay_cube_tri_idx] = t_coords[1];
  pay_cube_tri_idx++;
  pay_cube_v_positions[pay_cube_tri_idx] = p[b];
  pay_cube_tex_coords[pay_cube_tri_idx] = t_coords[0];
  pay_cube_tri_idx++;
  pay_cube_v_positions[pay_cube_tri_idx] = p[c]; 
  pay_cube_tex_coords[pay_cube_tri_idx] = t_coords[2];
  pay_cube_tri_idx++;
  pay_cube_v_positions[pay_cube_tri_idx] = p[a]; 
  pay_cube_tex_coords[pay_cube_tri_idx] = t_coords[1];
  pay_cube_tri_idx++;
  pay_cube_v_positions[pay_cube_tri_idx] = p[c]; 
  pay_cube_tex_coords[pay_cube_tri_idx] = t_coords[2];
  pay_cube_tri_idx++;
  pay_cube_v_positions[pay_cube_tri_idx] = p[d]; 
  pay_cube_tex_coords[pay_cube_tri_idx] = t_coords[3];
  pay_cube_tri_idx++;
 }
void drawCylinder(double radius, double height,glm::vec4 start){
	double dtheta = PI/10;
	double dh = height/2;
	for (double theta = 0.0;theta <= 2*PI;theta += dtheta){
		GLfloat x = radius * cos(theta);
		GLfloat y = radius * sin(theta);
		glm::vec4 pt = glm::vec4(start.x+x,start.y,start.z+y,1.0);
		pay_cube_v_positions[pay_cube_tri_idx] = pt;
		pay_cube_tex_coords[pay_cube_tri_idx] = glm::vec2(theta/(2*PI),0.0);
		pay_cube_tri_idx++;
		pt = glm::vec4(start.x+x,start.y+height,start.z+y,1.0);
		pay_cube_v_positions[pay_cube_tri_idx] = pt;
		pay_cube_tex_coords[pay_cube_tri_idx] = glm::vec2(theta/(2*PI),1.0);
		pay_cube_tri_idx++;
	}
	
	for (double theta = 0.0;theta <= 2*PI;theta+=dtheta){
		pay_cube_v_positions[pay_cube_tri_idx] = glm::vec4(start.x,start.y+height,start.z,1.0);
		pay_cube_tex_coords[pay_cube_tri_idx] = glm::vec2(0.0,0.0);
		pay_cube_tri_idx++;
		glm::vec4 pt = glm::vec4(start.x+radius*cos(theta),start.y+height,start.z+radius*sin(theta),1.0);
		pay_cube_v_positions[pay_cube_tri_idx] = pt;
		pay_cube_tex_coords[pay_cube_tri_idx] = glm::vec2(theta/(2*PI),1.0);
		pay_cube_tri_idx++;
	}
}

const int panel_num_vertices = 50;
int panel1_tri_idx=0;
glm::vec4 panel1_v_positions[panel_num_vertices];
glm::vec2 panel1_tex_coords[panel_num_vertices];
glm::vec4 panel2_v_positions[panel_num_vertices];
glm::vec2 panel2_tex_coords[panel_num_vertices];
void drawSphere(double radius,double thetalim,glm::vec4 start){
	double dtheta = PI/50;
	double dphi = PI/50;
	for (double theta = 0.0;theta <= thetalim;theta += dtheta){
		for (double phi = 0.0;phi <= 2*PI;phi += dphi){
			GLfloat z = start.z - radius*cos(theta);
			GLfloat y = start.y + radius*sin(theta)*sin(phi);
			GLfloat x = start.x + radius*sin(theta)*cos(phi);
			glm::vec4 pt = glm::vec4(x,y,z,1.0);
			pay_cube_v_positions[pay_cube_tri_idx] = pt;
			pay_cube_tex_coords[pay_cube_tri_idx] = glm::vec2(phi/(2*PI),theta/thetalim);
			pay_cube_tri_idx++;
			double l;
			if (theta + dtheta > thetalim)
				l = thetalim;
			else
				l = theta + dtheta;
			z = start.z - radius*cos(l);
			y = start.y + radius*sin(l)*sin(phi);
			x = start.x + radius*sin(l)*cos(phi);
			pt = glm::vec4(x,y,z,1.0);
			pay_cube_v_positions[pay_cube_tri_idx] = pt;
			pay_cube_tex_coords[pay_cube_tri_idx] = glm::vec2(phi/(2*PI),l/thetalim);
			pay_cube_tri_idx++;
		}
	}
}
void panel1_quad(int a, int b, int c, int d, glm::vec4 *p)
{
  panel1_v_positions[panel1_tri_idx] = p[a]; 
  panel1_tex_coords[panel1_tri_idx] = t_coords[1];
  panel1_tri_idx++;
  panel1_v_positions[panel1_tri_idx] = p[b];
  panel1_tex_coords[panel1_tri_idx] = t_coords[0];
  panel1_tri_idx++;
  panel1_v_positions[panel1_tri_idx] = p[c]; 
  panel1_tex_coords[panel1_tri_idx] = t_coords[2];
  panel1_tri_idx++;
  panel1_v_positions[panel1_tri_idx] = p[a]; 
  panel1_tex_coords[panel1_tri_idx] = t_coords[1];
  panel1_tri_idx++;
  panel1_v_positions[panel1_tri_idx] = p[c]; 
  panel1_tex_coords[panel1_tri_idx] = t_coords[2];
  panel1_tri_idx++;
  panel1_v_positions[panel1_tri_idx] = p[d]; 
  panel1_tex_coords[panel1_tri_idx] = t_coords[3];
  panel1_tri_idx++;
 }
void drawPanel1(){
	//std::cout<<panel1_tri_idx<<std::endl;
	float top_offset = pay_cube_edge_len/10;
	panel1_pivot = glm::vec4(pay_cube_edge_len,pay_cube_edge_len-top_offset,0.0,1.0);
	panel2_pivot = glm::vec4(-pay_cube_edge_len,pay_cube_edge_len-top_offset,0.0,1.0);
	panel1_v_positions[panel1_tri_idx] = glm::vec4(pay_cube_edge_len,pay_cube_edge_len-top_offset,0.0,1.0);
	panel1_tex_coords[panel1_tri_idx] = t_coords[0]; panel1_tri_idx++;
	panel1_v_positions[panel1_tri_idx] = glm::vec4(5*pay_cube_edge_len/4,9*pay_cube_edge_len/8-top_offset,0.0,1.0);
	panel1_tex_coords[panel1_tri_idx] = t_coords[3]; panel1_tri_idx++;
	panel1_v_positions[panel1_tri_idx] = glm::vec4(5*pay_cube_edge_len/4,7*pay_cube_edge_len/8-top_offset,0.0,1.0);
	panel1_tex_coords[panel1_tri_idx] = t_coords[3]; panel1_tri_idx++;
	glm::vec4 panel1_positions[15] = {
		glm::vec4(1.27*pay_cube_edge_len,-0.2*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(1.27*pay_cube_edge_len,1.5*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(1.35*pay_cube_edge_len,1.5*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(1.35*pay_cube_edge_len,-0.2*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(1.27*pay_cube_edge_len,-0.1*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(1.57*pay_cube_edge_len,-0.3*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(1.57*pay_cube_edge_len,-0.4*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(1.27*pay_cube_edge_len,1.4*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(1.27*pay_cube_edge_len,1.5*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(1.57*pay_cube_edge_len,1.7*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(1.57*pay_cube_edge_len,1.6*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(1.65*pay_cube_edge_len,1.7*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(1.65*pay_cube_edge_len,-0.4*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(5.65*pay_cube_edge_len,-0.4*pay_cube_edge_len-top_offset,0.0,1.0),
		glm::vec4(5.65*pay_cube_edge_len,1.7*pay_cube_edge_len-top_offset,0.0,1.0)
	};
	panel1_quad(0,1,2,3,panel1_positions);
	panel1_quad(0,4,5,6,panel1_positions);
	panel1_quad(7,8,9,10,panel1_positions);
	panel1_quad(9,6,12,11,panel1_positions);
	
	idx_array[arr_id] = panel1_tri_idx;arr_id++;
	//std::cout<<panel1_tri_idx<<" "<<idx_array[arr_id-1]<<std::endl;
	panel1_quad(11,12,13,14,panel1_positions);
	//std::cout<<panel1_tri_idx<<std::endl;
	idx_array[arr_id] = panel1_tri_idx;arr_id++;
}
void drawPanel2(){
	for (int i=0;i<panel1_tri_idx;i++){
		panel2_v_positions[i] = panel1_v_positions[i];
		panel2_tex_coords[i] = panel1_tex_coords[i];
		panel2_v_positions[i].x = -panel2_v_positions[i].x;
	}
}
// generate 12 triangles: 36 vertices and 36 colors
void draw_payload(void)
{
  pay_cube_quad( 1, 0, 3, 2, pay_cube_positions);
  pay_cube_quad( 2, 3, 7, 6, pay_cube_positions);
  pay_cube_quad( 3, 0, 4, 7, pay_cube_positions);
  pay_cube_quad( 6, 5, 1, 2, pay_cube_positions);
  pay_cube_quad( 4, 5, 6, 7, pay_cube_positions);
  pay_cube_quad( 5, 4, 0, 1, pay_cube_positions);
  //std::cout<<pay_cube_tri_idx<<std::endl;
  idx_array[arr_id] = pay_cube_tri_idx;arr_id++;
  drawCylinder(1,2,glm::vec4(0.0,pay_cube_edge_len,pay_cube_edge_len/1.4,1.0));
  idx_array[arr_id] = pay_cube_tri_idx;arr_id++;
  //std::cout<<pay_cube_tri_idx<<std::endl;
  drawCylinder(0.5,1,glm::vec4(pay_cube_edge_len/1.2,pay_cube_edge_len,pay_cube_edge_len/1.2,1.0));
  idx_array[arr_id] = pay_cube_tri_idx;arr_id++;
  drawCylinder(0.1,6,glm::vec4(pay_cube_edge_len/1.5,pay_cube_edge_len,pay_cube_edge_len/1.3,1.0));
  idx_array[arr_id] = pay_cube_tri_idx;arr_id++;
  drawCylinder(0.7,1.3,glm::vec4(pay_cube_edge_len/1.5,pay_cube_edge_len,-pay_cube_edge_len/1.2,1.0));
  idx_array[arr_id] = pay_cube_tri_idx;arr_id++;
  drawSphere(1.3*pay_cube_edge_len,PI/4,glm::vec4(0.0,0.0,2.3*pay_cube_edge_len,1.0));
  idx_array[arr_id] = pay_cube_tri_idx;arr_id++;
  
  float offset = pay_cube_edge_len/10;
  float w = pay_cube_edge_len/4.3;
  float h = pay_cube_edge_len/6;
  glm::vec4 base_cube_positions[8] = {
  	glm::vec4(pay_cube_edge_len - offset - w, -pay_cube_edge_len - h, pay_cube_edge_len - offset - w,1.0),
  	glm::vec4(pay_cube_edge_len - offset - w, -pay_cube_edge_len, pay_cube_edge_len - offset - w,1.0),
  	glm::vec4(pay_cube_edge_len - offset, -pay_cube_edge_len, pay_cube_edge_len - offset - w,1.0),
  	glm::vec4(pay_cube_edge_len - offset, -pay_cube_edge_len - h, pay_cube_edge_len - offset - w,1.0),
  	glm::vec4(pay_cube_edge_len - offset - w, -pay_cube_edge_len - h, pay_cube_edge_len - offset,1.0),
  	glm::vec4(pay_cube_edge_len - offset - w, -pay_cube_edge_len, pay_cube_edge_len - offset,1.0),
  	glm::vec4(pay_cube_edge_len - offset, -pay_cube_edge_len, pay_cube_edge_len - offset,1.0),
  	glm::vec4(pay_cube_edge_len - offset, -pay_cube_edge_len - h, pay_cube_edge_len - offset,1.0)
  };
  pay_cube_quad( 1, 0, 3, 2, base_cube_positions);
  pay_cube_quad( 2, 3, 7, 6, base_cube_positions);
  pay_cube_quad( 3, 0, 4, 7, base_cube_positions);
  //pay_cube_quad( 6, 5, 1, 2, base_cube_positions);
  pay_cube_quad( 4, 5, 6, 7, base_cube_positions);
  pay_cube_quad( 5, 4, 0, 1, base_cube_positions);
  for (int i=0;i<8;i++)
  	base_cube_positions[i].x -= 2*pay_cube_edge_len - w - 2*offset;
  pay_cube_quad( 1, 0, 3, 2, base_cube_positions);
  pay_cube_quad( 2, 3, 7, 6, base_cube_positions);
  pay_cube_quad( 3, 0, 4, 7, base_cube_positions);
  //pay_cube_quad( 6, 5, 1, 2, base_cube_positions);
  pay_cube_quad( 4, 5, 6, 7, base_cube_positions);
  pay_cube_quad( 5, 4, 0, 1, base_cube_positions);
  for (int i=0;i<8;i++)
  	base_cube_positions[i].z -= 2*pay_cube_edge_len - w - 2*offset;
  pay_cube_quad( 1, 0, 3, 2, base_cube_positions);
  pay_cube_quad( 2, 3, 7, 6, base_cube_positions);
  pay_cube_quad( 3, 0, 4, 7, base_cube_positions);
  //pay_cube_quad( 6, 5, 1, 2, base_cube_positions);
  pay_cube_quad( 4, 5, 6, 7, base_cube_positions);
  pay_cube_quad( 5, 4, 0, 1, base_cube_positions);
  for (int i=0;i<8;i++)
  	base_cube_positions[i].x += 2*pay_cube_edge_len - w - 2*offset;
  pay_cube_quad( 1, 0, 3, 2, base_cube_positions);
  pay_cube_quad( 2, 3, 7, 6, base_cube_positions);
  pay_cube_quad( 3, 0, 4, 7, base_cube_positions);
  //pay_cube_quad( 6, 5, 1, 2, base_cube_positions);
  pay_cube_quad( 4, 5, 6, 7, base_cube_positions);
  pay_cube_quad( 5, 4, 0, 1, base_cube_positions);
  idx_array[arr_id] = pay_cube_tri_idx;arr_id++;
  drawPanel1();
  drawPanel2();
}
const int veh_num_vertices = 4000;
int veh_tri_idx=0;
glm::vec4 veh_v_positions[veh_num_vertices];
glm::vec2 veh_tex_coords[veh_num_vertices];
const int booster_num_vertices = 2000;
int booster1_tri_idx=0;
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
void drawVehCylinder(double bradius,double tradius, double height,glm::vec4 start,bool flag,int slanted,glm::vec4 *a1,glm::vec2 *a2,int &idx){
	double dtheta = PI/50;
	if (flag){
		for (double theta = 0.0;theta <= 2*PI;theta+=dtheta){
			a1[idx] = glm::vec4(start.x,start.y,start.z,1.0);
			a2[idx] = glm::vec2(0.0,0.0);
			idx++;
			glm::vec4 pt = glm::vec4(start.x+bradius*cos(theta),start.y,start.z+bradius*sin(theta),1.0);
			a1[idx] = pt;
			a2[idx] = glm::vec2(theta/(2*PI),1.0);
			idx++;
		}
	}
	for (double theta = 0.0;theta <= 2*PI;theta += dtheta){
		
		GLfloat x = bradius * cos(theta);
		GLfloat y = bradius * sin(theta);
		glm::vec4 pt = glm::vec4(start.x+x,start.y,start.z+y,1.0);
		a1[idx] = pt;
		a2[idx] = glm::vec2(theta/(2*PI),0.0);
		idx++;
		x = tradius * cos(theta) - slanted*(bradius-tradius);
		y = tradius * sin(theta);
		pt = glm::vec4(start.x+x,start.y+height,start.z+y,1.0);
		a1[idx] = pt;
		a2[idx] = glm::vec2(theta/(2*PI),1.0);
		idx++;
	}
	if (flag){
		for (double theta = 0.0;theta <= 2*PI;theta+=dtheta){
			a1[idx] = glm::vec4(start.x,start.y+height,start.z,1.0);
			a2[idx] = glm::vec2(0.0,0.0);
			idx++;
			glm::vec4 pt = glm::vec4(start.x+tradius*cos(theta)- slanted*(bradius-tradius),start.y+height,start.z+tradius*sin(theta),1.0);
			a1[idx] = pt;
			a2[idx] = glm::vec2(theta/(2*PI),1.0);
			idx++;
		}
	}
}
double vr = 2;
double vh = 20;
void drawVehicle(){
	drawVehCylinder(vr*0.5,vr*0.3,vh/25,glm::vec4(0.0,-vh-vh/25,0.0,1.0),false,0,down_v_positions,down_tex_coords,down_tri_idx);
	rov_arr[rid] = down_tri_idx; rid++;
	drawVehCylinder(vr*0.8,vr,vh/9,glm::vec4(0.0,-vh,0.0,1.0),true,0,down_v_positions,down_tex_coords,down_tri_idx);
	drawVehCylinder(vr,vr,13*vh/14-vh/5-vh/17-vh/2.2+vh-vh/9,glm::vec4(0.0,-vh+vh/9,0.0,1.0),true,0,down_v_positions,down_tex_coords,down_tri_idx);
	rov_arr[rid] = down_tri_idx; rid++;
	drawVehCylinder(vr,vr,vh/2.2,glm::vec4(0.0,13*vh/14-vh/5-vh/17-vh/2.2,0.0,1.0),true,0,central_v_positions,central_tex_coords,central_tri_idx);
	rov_arr[rid] = central_tri_idx; rid++;
	drawVehCylinder(vr*0.5,vr*0.3,vh/25,glm::vec4(0.0,13*vh/14-vh/5-vh/17-vh/2.2-vh/25,0.0,1.0),false,0,central_v_positions,central_tex_coords,central_tri_idx);
	//std::cout<<veh_tri_idx<<std::endl;
	//std::cout<<veh_tri_idx<<std::endl;
	rov_arr[rid] = central_tri_idx; rid++;
	drawVehCylinder(vr,1.2*vr,vh/17,glm::vec4(0.0,13*vh/14-vh/5-vh/17,0.0,1.0),true,0,veh_v_positions,veh_tex_coords,veh_tri_idx);
	drawVehCylinder(1.2*vr,1.2*vr,vh/5,glm::vec4(0.0,13*vh/14-vh/5,0.0,1.0),true,0,veh_v_positions,veh_tex_coords,veh_tri_idx);
	drawVehCylinder(1.2*vr,0.7*vr,vh/14,glm::vec4(0.0,13*vh/14,0.0,1.0),true,0,veh_v_positions,veh_tex_coords,veh_tri_idx);
	//std::cout<<veh_tri_idx<<std::endl;
	rov_arr[rid] = veh_tri_idx; rid++;
	drawVehCylinder(1.2*vr,vr,vh/13,glm::vec4(2.2*vr,-vh-vh/13,0.0,1.0),true,1,booster1_v_positions,booster1_tex_coords,booster1_tri_idx);
	rov_arr[rid] = booster1_tri_idx; rid++;
	drawVehCylinder(vr,vr,13*vh/14-vh/5-vh/17-vh/2.2+vh,glm::vec4(2*vr,-vh,0.0,1.0),true,0,booster1_v_positions,booster1_tex_coords,booster1_tri_idx);
	rov_arr[rid] = booster1_tri_idx; rid++;
	drawVehCylinder(vr,vr*0.5,vh/7,glm::vec4(2*vr,13*vh/14-vh/5-vh/17-vh/2.2,0.0,1.0),true,1,booster1_v_positions,booster1_tex_coords,booster1_tri_idx);
	rov_arr[rid] = booster1_tri_idx; rid++;
	for (int i=0;i<booster1_tri_idx;i++){
		booster2_v_positions[i] = booster1_v_positions[i];
		booster2_tex_coords[i] = booster1_tex_coords[i];
		booster2_v_positions[i].x = -booster2_v_positions[i].x;
	}
	//std::cout<<booster1_tri_idx<<std::endl;
}
void saveModel(){
  std::ofstream fout;
  fout.open("data/launch_vehicle_hood.OBJ");
  for (int i=0;i<veh_tri_idx;i++){
    fout<<"v "<<veh_v_positions[i].x<<" "<<veh_v_positions[i].y<<" "<<veh_v_positions[i].z<<std::endl;
    fout<<"vt "<<veh_tex_coords[i].x<<" "<<veh_tex_coords[i].y<<std::endl;
  }
  fout.close();
  fout.open("data/launch_vehicle_central.OBJ");
  for (int i=0;i<central_tri_idx;i++){
    fout<<"v "<<central_v_positions[i].x<<" "<<central_v_positions[i].y<<" "<<central_v_positions[i].z<<std::endl;
    fout<<"vt "<<central_tex_coords[i].x<<" "<<central_tex_coords[i].y<<std::endl;
  }
  fout.close();
  fout.open("data/launch_vehicle_base.OBJ");
  for (int i=0;i<down_tri_idx;i++){
    fout<<"v "<<down_v_positions[i].x<<" "<<down_v_positions[i].y<<" "<<down_v_positions[i].z<<std::endl;
    fout<<"vt "<<down_tex_coords[i].x<<" "<<down_tex_coords[i].y<<std::endl;
  }
  fout.close();
  fout.open("data/launch_vehicle_booster1.OBJ");
  for (int i=0;i<booster1_tri_idx;i++){
    fout<<"v "<<booster1_v_positions[i].x<<" "<<booster1_v_positions[i].y<<" "<<booster1_v_positions[i].z<<std::endl;
    fout<<"vt "<<booster1_tex_coords[i].x<<" "<<booster1_tex_coords[i].y<<std::endl;
  }
  fout.close();
  fout.open("data/launch_vehicle_booster2.OBJ");
  for (int i=0;i<booster1_tri_idx;i++){
    fout<<"v "<<booster2_v_positions[i].x<<" "<<booster2_v_positions[i].y<<" "<<booster2_v_positions[i].z<<std::endl;
    fout<<"vt "<<booster2_tex_coords[i].x<<" "<<booster2_tex_coords[i].y<<std::endl;
  }
  fout.close();
  fout.open("data/payload_base.OBJ");
  for (int i=0;i<pay_cube_tri_idx;i++){
    fout<<"v "<<pay_cube_v_positions[i].x<<" "<<pay_cube_v_positions[i].y<<" "<<pay_cube_v_positions[i].z<<std::endl;
    fout<<"vt "<<pay_cube_tex_coords[i].x<<" "<<pay_cube_tex_coords[i].y<<std::endl;
  }
  fout.close();
  fout.open("data/payload_panel1.OBJ");
  for (int i=0;i<panel1_tri_idx;i++){
    fout<<"v "<<panel1_v_positions[i].x<<" "<<panel1_v_positions[i].y<<" "<<panel1_v_positions[i].z<<std::endl;
    fout<<"vt "<<panel1_tex_coords[i].x<<" "<<panel1_tex_coords[i].y<<std::endl;
  }
  fout.close();
  fout.open("data/payload_panel2.OBJ");
  for (int i=0;i<panel1_tri_idx;i++){
    fout<<"v "<<panel2_v_positions[i].x<<" "<<panel2_v_positions[i].y<<" "<<panel2_v_positions[i].z<<std::endl;
    fout<<"vt "<<panel2_tex_coords[i].x<<" "<<panel2_tex_coords[i].y<<std::endl;
  }
  fout.close();
}
void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("src/rover_simple_tex_vshader.glsl");
  std::string fragment_shader_file("src/rover_simple_tex_fshader.glsl");

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
  basetex=LoadTexture("src/images/base_tex.bmp",376,376);
  glBindTexture(GL_TEXTURE_2D, basetex);

  //Ask GL for two Vertex Attribute Objects (vao) , one for the sphere and one for the wireframe
  glGenVertexArrays (8, vao);
  //Ask GL for two Vertex Buffer Object (vbo)
  glGenBuffers (8, vbo);

  //Set 0 as the current array to be used by binding it
  glBindVertexArray (vao[0]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);

  draw_payload();
  drawVehicle();
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

  glBindVertexArray (vao[1]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[1]);

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

  glBindVertexArray (vao[2]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[2]);

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

  glBindVertexArray (vao[3]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[3]);

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

  glBindVertexArray (vao[4]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[4]);

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

  glBindVertexArray (vao[5]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[5]);

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

  glBindVertexArray (vao[6]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[6]);

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

  glBindVertexArray (vao[7]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[7]);

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
 	armtex=LoadTexture("src/images/dark-metal-texture.bmp",3000,2000);
 	hoodtex=LoadTexture("src/images/hood_tex.bmp",516,318);
 	boosterbody=LoadTexture("src/images/booster_body.bmp",1000,1000);
 	centraltex=LoadTexture("src/images/central_tex.bmp",450,112);
 	blaster=LoadTexture("src/images/metallic_tex.bmp",600,450);
 	boosterhead=LoadTexture("src/images/booster_head.bmp",509,339);
 	/*for (int i=0;i<8;i++)
 		std::cout<<i<<" ";
 	std::cout<<std::endl;
 	for (int i=0;i<8;i++)
 		std::cout<<rov_arr[i]<<" ";
 	std::cout<<std::endl;
 	for (int i=0;i<9;i++)
 		std::cout<<idx_array[i]<<" ";
 	std::cout<<std::endl;*/
  
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
  modelview_matrix = view_matrix*model_matrix;
  normal_matrix = glm::transpose (glm::inverse(glm::mat3(modelview_matrix)));
  //  glBindTexture(GL_TEXTURE_2D, tex);
  glUseProgram(texshaderProgram);
  glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	glm::mat4 newmodelview_matrix;
  if (payload){
	  glBindTexture(GL_TEXTURE_2D, goldtex);
	  glBindVertexArray (vao[0]);
	  glDrawArrays(GL_TRIANGLES, 0, idx_array[0]);
	  glDrawArrays(GL_TRIANGLE_STRIP, idx_array[0], idx_array[1]-idx_array[0]);
	  glDrawArrays(GL_TRIANGLE_STRIP, idx_array[1], idx_array[2]-idx_array[1]);
	  glDrawArrays(GL_TRIANGLE_STRIP, idx_array[2], idx_array[3]-idx_array[2]);
	  glDrawArrays(GL_TRIANGLE_STRIP, idx_array[3], idx_array[4]-idx_array[3]);
	  glBindTexture(GL_TEXTURE_2D, basetex);
	  glDrawArrays(GL_TRIANGLE_STRIP, idx_array[4], idx_array[5]-idx_array[4]);
	  glDrawArrays(GL_TRIANGLES, idx_array[5], idx_array[6]-idx_array[5]);

	  glm::mat4 T1 = glm::translate(glm::mat4(1.0),glm::vec3(-panel1_pivot));
	  rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(panel1_xrot), glm::vec3(1.0f,0.0f,0.0f));
	  glm::mat4 T2 = glm::translate(glm::mat4(1.0),glm::vec3(panel1_pivot));
	  newmodelview_matrix = modelview_matrix*T2*rotation_matrix*T1;
	  glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(newmodelview_matrix));
	  glBindVertexArray(vao[1]);
	  glBindTexture(GL_TEXTURE_2D, armtex);
	  glDrawArrays(GL_TRIANGLES,0,idx_array[7]);
	  glBindTexture(GL_TEXTURE_2D, paneltex);
	  glDrawArrays(GL_TRIANGLES,idx_array[7],idx_array[8]-idx_array[7]);
	  T1 = glm::translate(glm::mat4(1.0),glm::vec3(-panel2_pivot));
	  rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(panel2_xrot), glm::vec3(1.0f,0.0f,0.0f));
	  T2 = glm::translate(glm::mat4(1.0),glm::vec3(panel2_pivot));
	  newmodelview_matrix = modelview_matrix*T2*rotation_matrix*T1;
	  glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(newmodelview_matrix));
	  glBindVertexArray(vao[2]);
	  glBindTexture(GL_TEXTURE_2D, armtex);
	  glDrawArrays(GL_TRIANGLES,0,idx_array[7]);
	  glBindTexture(GL_TEXTURE_2D, paneltex);
	  glDrawArrays(GL_TRIANGLES,idx_array[7],idx_array[8]-idx_array[7]);
	}
	else if (rover){
		glBindVertexArray(vao[7]);
		glBindTexture(GL_TEXTURE_2D,centraltex);
		glDrawArrays(GL_TRIANGLE_STRIP,0,rov_arr[2]);
		glBindTexture(GL_TEXTURE_2D,blaster);
		glDrawArrays(GL_TRIANGLE_STRIP,rov_arr[2],rov_arr[3]-rov_arr[2]);
		double bh = 13*vh/14-vh/5-vh/17;
		double scaling = 0.15;
		newmodelview_matrix = modelview_matrix*glm::translate(glm::mat4(1.0),glm::vec3(0.0,bh + pay_cube_edge_len*0.2,0.0))*glm::scale(glm::mat4(1.0),glm::vec3(scaling,scaling,scaling));
		glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(newmodelview_matrix));
		glBindTexture(GL_TEXTURE_2D, goldtex);
		  glBindVertexArray (vao[0]);
		  glDrawArrays(GL_TRIANGLES, 0, idx_array[0]);
		  glDrawArrays(GL_TRIANGLE_STRIP, idx_array[0], idx_array[1]-idx_array[0]);
		  glDrawArrays(GL_TRIANGLE_STRIP, idx_array[1], idx_array[2]-idx_array[1]);
		  glDrawArrays(GL_TRIANGLE_STRIP, idx_array[2], idx_array[3]-idx_array[2]);
		  glDrawArrays(GL_TRIANGLE_STRIP, idx_array[3], idx_array[4]-idx_array[3]);
		  glBindTexture(GL_TEXTURE_2D, basetex);
		  glDrawArrays(GL_TRIANGLE_STRIP, idx_array[4], idx_array[5]-idx_array[4]);
		  glDrawArrays(GL_TRIANGLES, idx_array[5], idx_array[6]-idx_array[5]);

		newmodelview_matrix = modelview_matrix*glm::translate(glm::mat4(1.0),glm::vec3(0.0,hood_ypos,0.0));
		glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(newmodelview_matrix));
		glBindVertexArray(vao[3]);
		glBindTexture(GL_TEXTURE_2D,hoodtex);
		glDrawArrays(GL_TRIANGLE_STRIP,0,rov_arr[4]);

		newmodelview_matrix = modelview_matrix*glm::translate(glm::mat4(1.0),glm::vec3(0.0,base_ypos,0.0));
		glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(newmodelview_matrix));
		glBindVertexArray(vao[6]);
		glBindTexture(GL_TEXTURE_2D,blaster);
		glDrawArrays(GL_TRIANGLE_STRIP,0,rov_arr[0]);
		glBindTexture(GL_TEXTURE_2D,basetex);
		glDrawArrays(GL_TRIANGLE_STRIP,rov_arr[0],rov_arr[1]-rov_arr[0]);
		
		newmodelview_matrix = modelview_matrix*glm::translate(glm::mat4(1.0),glm::vec3(booster_xpos,0.0,0.0));
		glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(newmodelview_matrix));
		glBindVertexArray(vao[4]);
		glBindTexture(GL_TEXTURE_2D,blaster);
		glDrawArrays(GL_TRIANGLE_STRIP,0,rov_arr[5]);
		glBindTexture(GL_TEXTURE_2D,boosterbody);
		glDrawArrays(GL_TRIANGLE_STRIP,rov_arr[5],rov_arr[6]-rov_arr[5]);
		glBindTexture(GL_TEXTURE_2D,boosterhead);
		glDrawArrays(GL_TRIANGLE_STRIP,rov_arr[6],rov_arr[7]-rov_arr[6]);

		newmodelview_matrix = modelview_matrix*glm::translate(glm::mat4(1.0),glm::vec3(-booster_xpos,0.0,0.0));
		glUniformMatrix4fv(texuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(newmodelview_matrix));
		glBindVertexArray(vao[5]);
		glBindTexture(GL_TEXTURE_2D,blaster);
		glDrawArrays(GL_TRIANGLE_STRIP,0,rov_arr[5]);
		glBindTexture(GL_TEXTURE_2D,boosterbody);
		glDrawArrays(GL_TRIANGLE_STRIP,rov_arr[5],rov_arr[6]-rov_arr[5]);
		glBindTexture(GL_TEXTURE_2D,boosterhead);
		glDrawArrays(GL_TRIANGLE_STRIP,rov_arr[6],rov_arr[7]-rov_arr[6]);
	}
  //glUseProgram(simpleshaderProgram);
  //glUniformMatrix4fv(simpleuModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
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
  window = glfwCreateWindow(512, 512, "CS475/CS675 Assignment3: Launch Vehicle and Payload Model", NULL, NULL);
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

