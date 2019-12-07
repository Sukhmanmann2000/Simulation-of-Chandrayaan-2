#include "rover_gl_framework.hpp"

extern GLfloat xrot,yrot,zrot,c_xrot,c_yrot,c_zrot,panel1_xrot,panel2_xrot,booster_xpos,base_ypos,hood_ypos;
extern bool enable_perspective,payload,rover;

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
    else if (key == GLFW_KEY_LEFT )
      yrot -= 1.0;
    else if (key == GLFW_KEY_RIGHT )
      yrot += 1.0;
    else if (key == GLFW_KEY_UP )
      xrot -= 1.0;
    else if (key == GLFW_KEY_DOWN )
      xrot += 1.0;
    else if (key == GLFW_KEY_LEFT_BRACKET )
      zrot -= 1.0;
    else if (key == GLFW_KEY_RIGHT_BRACKET)
      zrot += 1.0;
    else if (key == GLFW_KEY_A  )
      c_yrot -= 1.0;
    else if (key == GLFW_KEY_D  )
      c_yrot += 1.0;
    else if (key == GLFW_KEY_W  )
      c_xrot -= 1.0;
    else if (key == GLFW_KEY_S  )
      c_xrot += 1.0;        
    else if (key == GLFW_KEY_Q  )
      c_zrot -= 1.0;
    else if (key == GLFW_KEY_E  )
      c_zrot += 1.0;  
    else if (key == GLFW_KEY_K && payload) {
      if (panel1_xrot <= 90)
        panel1_xrot += 1.0;
    }
    else if (key == GLFW_KEY_L && payload ) {
      if (panel1_xrot >= -90)
        panel1_xrot -= 1.0;
    }
    else if (key == GLFW_KEY_I && payload) {
      if (panel2_xrot <= 90)
        panel2_xrot += 1.0;
    }
    else if (key == GLFW_KEY_O && payload) {
      if (panel2_xrot >= -90)
        panel2_xrot -= 1.0;
    }
    else if (key == GLFW_KEY_1 && payload){
      payload = false;
      rover = true;
    }
    else if (key == GLFW_KEY_2 && rover){
      rover = false;
      payload = true;
    }
    else if (key == GLFW_KEY_Z && rover){
      booster_xpos += 1.0;
    }
    else if (key == GLFW_KEY_X && rover){
      if (booster_xpos > 0)
        booster_xpos -= 1.0;
    }
    else if (key == GLFW_KEY_Y && rover){
      base_ypos -= 1.0;
    }
    else if (key == GLFW_KEY_T && rover){
      if (base_ypos < 0)
        base_ypos += 1.0;
    }
    else if (key == GLFW_KEY_G && rover){
      hood_ypos += 1.0;
    }
    else if (key == GLFW_KEY_H && rover){
      if (hood_ypos > 0)
        hood_ypos -= 1.0;
    }
  }
};  
  


