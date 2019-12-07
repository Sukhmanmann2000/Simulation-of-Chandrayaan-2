#include "earth_gl_framework.hpp"

extern GLfloat xrot,yrot,zrot,c_xpos,c_ypos,c_zpos;
extern int tesselation;
extern float edge_len;
extern double Radius;
extern bool enable_perspective,wireframe;
float edge = edge_len - 2;
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
    	if (c_ypos>-edge && (c_zpos*c_zpos + c_xpos*c_xpos + c_ypos*c_ypos > r*r))
      		c_ypos -= trans_step;
    }
    else if (key == GLFW_KEY_DOWN){
    	if (c_ypos<edge)
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
      zrot -= rot_step;
    else if (key == GLFW_KEY_D)
      zrot += rot_step;
  }
};  
  


