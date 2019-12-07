/*
  CSX75 Tutorial 6 Texturing a Cube


  Modified from An Introduction to OpenGL Programming, 
  Ed Angel and Dave Shreiner, SIGGRAPH 2013

  Written by Aditya Prakash, 2015
*/
//////////// LOAD TEXTURES
///////// DEBUG

#include "earth.hpp"
#include "earth_texture.hpp"
int tesselation=50; 
bool wireframe=false;
double PI=3.14159265;

GLuint shaderProgram,planetshaderProgram;
GLuint vbo[2], vao[2];
GLuint earthtex,spacetex;
float timeElapsed = 0.0;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;


glm::mat4 modelview_matrix;
glm::mat3 normal_matrix;

GLuint uModelViewMatrix,uModelViewMatrix1;
GLuint viewMatrix,viewMatrix1;
GLuint normalMatrix,normalMatrix1;
GLuint shaderTime,earthRadius,innerRadius,outerRadius,shaderEye;
//-----------------------------------------------------------------

//6 faces, 2 triangles/face, 3 vertices/triangle
const int num_vertices = 25000;

int tri_idx=0;
glm::vec4 v_positions[num_vertices];
glm::vec4 v_normals[num_vertices];
glm::vec4 v_colors[num_vertices];
glm::vec2 tex_coords[num_vertices];

int prev_tess = 30; 
int Lat = 10;
int Long = 10;

void sphere(double radius, int Lats, int Longs)
{
  float lats, longs;

  // float slices=float(Lats)/100;
  // float sectors=float(Longs)/100;
  
  float slices=(180/(float(Longs)*10))/9;
  float sectors=(180/(float(Longs)*10))/9;

  float l;

  for (lats = 0.0; lats <= PI; lats+=sectors)  
    {
      for(longs = 0.0; longs <= 2.0*PI; longs+=slices)
  {
    float x = radius * sin(lats) * cos(longs);
    float y = radius * sin(lats) * sin(longs);
    float z = radius * cos(lats);
    glm::vec4 pt(x, y, z, 1.0);
    //if (longs > 2.0*PI - slices)
    //  tex_coords[tri_idx] = glm::vec2((longs-0.01)/20.0*PI,1.0-lats/PI);
    //else
      tex_coords[tri_idx] = glm::vec2(longs/(2*PI),1.0-lats/PI); 
      v_colors[tri_idx] = glm::vec4(1.0,0.0,0.0,1.0);
    v_positions[tri_idx] = pt; 
    v_normals[tri_idx] = pt; tri_idx++;
   
    if(lats+sectors>PI)
      l=PI;
    else
      l=lats+sectors;
    x = radius * sin(l) * cos(longs);
    y = radius * sin(l) * sin(longs);
    z = radius * cos(l);
    pt =glm::vec4(x, y, z, 1.0);
    //if(longs > 2.0*PI - slices)
    //  tex_coords[tri_idx] = glm::vec2((longs-0.01)/20.0*PI,1.0-l/PI);
    //else
      tex_coords[tri_idx] = glm::vec2(longs/(2*PI),1.0-l/PI); 
      v_colors[tri_idx] = glm::vec4(1.0,0.0,0.0,1.0);
    v_positions[tri_idx] = pt; 
    v_normals[tri_idx] = pt; tri_idx++;    
  }
    }
}
const int space_num_vertices = 36;
int space_tri_idx=0;
glm::vec4 space_v_positions[space_num_vertices];
glm::vec4 space_v_normals[space_num_vertices];
glm::vec2 space_tex_coords[space_num_vertices];
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
//RGBA colors

glm::vec2 t_coords[4] = {
  glm::vec2( 0.0, 0.0),
  glm::vec2( 0.0, 1.0),
  glm::vec2( 1.0, 0.0),
  glm::vec2( 1.0, 1.0)
};

void quad(int a, int b, int c, int d)
{
  space_v_positions[space_tri_idx] = positions[a]; 
  space_tex_coords[space_tri_idx] = t_coords[1];
  space_tri_idx++;
  space_v_positions[space_tri_idx] = positions[b];
  space_tex_coords[space_tri_idx] = t_coords[0];
  space_tri_idx++;
  space_v_positions[space_tri_idx] = positions[c]; 
  space_tex_coords[space_tri_idx] = t_coords[2];
  space_tri_idx++;
  space_v_positions[space_tri_idx] = positions[a]; 
  space_tex_coords[space_tri_idx] = t_coords[1];
  space_tri_idx++;
  space_v_positions[space_tri_idx] = positions[c]; 
  space_tex_coords[space_tri_idx] = t_coords[2];
  space_tri_idx++;
  space_v_positions[space_tri_idx] = positions[d]; 
  space_tex_coords[space_tri_idx] = t_coords[3];
  space_tri_idx++;
 }

 void spacecube(void)
{
  quad( 1, 0, 3, 2);
  quad( 2, 3, 7, 6);
  quad( 3, 0, 4, 7);
  quad( 6, 5, 1, 2);
  quad( 4, 5, 6, 7);
  quad( 5, 4, 0, 1);
}
//-----------------------------------------------------------------
void saveModel(){
  std::ofstream fout;
  fout.open("data/earth.OBJ");
  for (int i=0;i<tri_idx;i++){
    fout<<"v "<<v_positions[i].x<<" "<<v_positions[i].y<<" "<<v_positions[i].z<<std::endl;
    fout<<"vt "<<tex_coords[i].x<<" "<<tex_coords[i].y<<std::endl;
  }
  fout.close();
  fout.open("data/space.OBJ");
  for (int i=0;i<space_tri_idx;i++){
    fout<<"v "<<space_v_positions[i].x<<" "<<space_v_positions[i].y<<" "<<space_v_positions[i].z<<std::endl;
    fout<<"vt "<<space_tex_coords[i].x<<" "<<space_tex_coords[i].y<<std::endl;
  }
  fout.close();
}

void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("src/earth_vshader.glsl");
  std::string fragment_shader_file("src/earth_fshader.glsl");


  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );
  // getting the attributes from the shader program
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  GLuint vNormal = glGetAttribLocation( shaderProgram, "vNormal" ); 
  GLuint texCoord = glGetAttribLocation( shaderProgram, "texCoord" ); 
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
  normalMatrix =  glGetUniformLocation( shaderProgram, "normalMatrix");
  viewMatrix = glGetUniformLocation( shaderProgram, "viewMatrix");
  shaderTime = glGetUniformLocation( shaderProgram , "iTime");
  earthRadius = glGetUniformLocation( shaderProgram , "earthRadius");
  shaderEye = glGetUniformLocation( shaderProgram , "eye");
  // Load Textures 
  earthtex=LoadTexture("src/images/earth.bmp",2048,1024);
  glBindTexture(GL_TEXTURE_2D, earthtex);
  

  //Ask GL for two Vertex Attribute Objects (vao) , one for the sphere and one for the wireframe
  glGenVertexArrays (2, vao);
  //Ask GL for two Vertex Buffer Object (vbo)
  glGenBuffers (2, vbo);

  //Set 0 as the current array to be used by binding it
  glBindVertexArray (vao[0]);
  //Set 0 as the current buffer to be used by binding it
  glBindBuffer (GL_ARRAY_BUFFER, vbo[0]);

  Lat = tesselation;
  Long = tesselation;
  sphere(Radius, Lat, Long);

  //Copy the points into the current buffer
  glBufferData (GL_ARRAY_BUFFER, sizeof (v_positions) + sizeof(tex_coords) + sizeof(v_normals), NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(v_positions), v_positions );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(tex_coords), tex_coords);
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(tex_coords)+sizeof(v_positions), sizeof(v_normals), v_normals );
  // set up vertex array
  //Position
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  //Textures
  glEnableVertexAttribArray( texCoord );
  glVertexAttribPointer( texCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions)) );

  //Normal
  glEnableVertexAttribArray( vNormal );
  glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions)+sizeof(tex_coords)) );

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
  spacecube();
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
  //std::cout<<c_pos.x<<" "<<c_pos.y<<" "<<c_pos.z<<std::endl;
  projection_matrix = glm::frustum(-1.0, 1.0, -1.0, 1.0, 0.5, 3.0*(double)edge_len);
  glUseProgram(shaderProgram);
  
  view_matrix = projection_matrix*lookat_matrix;
  glUniform4f(shaderEye,c_pos.x,c_pos.y,c_pos.z,1.0);
  glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
  timeElapsed += 1.0;
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
  glUseProgram(shaderProgram);
  glBindTexture(GL_TEXTURE_2D, earthtex);
  glBindVertexArray (vao[0]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, num_vertices);
  
  
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
  window = glfwCreateWindow(512, 512, "CS475/CS675 Assignment3: Earth Model", NULL, NULL);
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
//std::cout<<tri_idx<<std::endl;
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

