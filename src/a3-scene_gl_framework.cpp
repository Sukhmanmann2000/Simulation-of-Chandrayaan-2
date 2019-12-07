#include "a3-scene_gl_framework.hpp"

extern GLfloat xrot,yrot,zrot,c_xpos,c_ypos,c_zpos,c_yrot;
extern int tesselation;
extern float edge_len;
extern double Radius;
extern bool enable_perspective,wireframe,c1,c2,c3,c4,launched,savetrajectories,loadtrajectories;
float edge = 7*edge_len/8;
float trans_step = 0.25;
float rot_step = 1.0;
double r = Radius + 2;
namespace csX75
{
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_LEFT){
    	//std::cout<<"Hi\n";
    	if (c_xpos>-edge && (c_zpos*c_zpos + c_xpos*c_xpos + c_ypos*c_ypos > r*r))
      		c_xpos -= trans_step;
    }
    else if (key == GLFW_KEY_RIGHT){
    	//std::cout<<"Hi\n";
    	if (c_xpos<edge ) 
      		c_xpos += trans_step;
    }
    else if (key == GLFW_KEY_UP){
    	if (!c1 && c_ypos>-edge && (c_zpos*c_zpos + c_xpos*c_xpos + c_ypos*c_ypos > r*r))
      		c_ypos -= trans_step;
    }
    else if (key == GLFW_KEY_DOWN){
    	if (!c1 && c_ypos<edge)
      		c_ypos += trans_step;
    }
    else if (key == GLFW_KEY_LEFT_BRACKET){
    	if (c_zpos>-edge && (c_zpos*c_zpos + c_xpos*c_xpos + c_ypos*c_ypos > r*r))
      		c_zpos -= trans_step;
    }
    else if (key == GLFW_KEY_RIGHT_BRACKET){
    	if (c_zpos<edge)
      		c_zpos += trans_step;
    }
    else if (key == GLFW_KEY_A)
      c_yrot -= rot_step;
    else if (key == GLFW_KEY_D)
      c_yrot += rot_step;
    else if (key == GLFW_KEY_1){
      c1=true;c2=false;c3=false,c4=false;
      c_xpos=0;c_ypos=0;c_zpos=80;
    }
    else if (key == GLFW_KEY_2){
      c2=true;c1=false;c3=false,c4=false;
    }
    else if (key == GLFW_KEY_3){
      c2=false;c1=false;c3=true,c4=false;
    }
    else if (key == GLFW_KEY_4){
      c2=false;c1=false;c4=true,c3=false;
    }
    else if (key == GLFW_KEY_L)
      launched = true;
    else if (key == GLFW_KEY_T && action == GLFW_PRESS)
      savetrajectories = true;
    else if (key == GLFW_KEY_Y && action == GLFW_PRESS)
      loadtrajectories = true;
  }
};  
  


