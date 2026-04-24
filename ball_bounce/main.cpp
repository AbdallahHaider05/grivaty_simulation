#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>  
#include <GLFW/glfw3.h>
#include <iostream>
#include "world.h"
#include "body.h"
#include "point.h"
#include <cmath>

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "settings.h"
#include <random>
using namespace std;




string shaderSourceRead(string path)
{
    ifstream shaderFile(path, ios::in);
    if (!shaderFile.is_open()) {
        cout << "cant read " << path << " file !" << endl;

    }
    stringstream str;
    str << shaderFile.rdbuf();
    shaderFile.close();
    return str.str();

}







float* circlePath() {
    const int N = 720;
    float points[N] ;
    const float dgreeFactor = 3.14159265359 / 180;
    for (int i = 0; i < N/2; i++) {
        
            float radin = i * 2 * 3.141 / 360;


        points[ 2*i] = cos(radin) ;
        points[ 1+2*i] = sin(radin) ;

        
    }
   


    return points;
}

World world = World({}, 1, 0.01);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  
    if (yoffset > 0) {
        world.zoom /= 1.1;
    }
    else {
        world.zoom *= 1.1;
    }
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(2000, 850, "gravity simulation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

   vector<Body> &bod = world.bodies;
   world.bodies.push_back(
       Body(6.1 * pow(10, 6), Point(0.0, 0.0))//sun




       );
   srand(1);
   random_device rd;

   mt19937 gen(rd());

   double min = -1;
   double max = 0;
   uniform_real_distribution<double> dis(min, max);

   for (int i = 0; i < 10; i++) {
  
       double x = dis(gen);
       double y = dis(gen);
       Body movingBody1 = Body(0.1 * pow(10, 4), Point(x, 2*y+1));
       movingBody1.speed = Point(0.0, 0.3);
       bod.push_back(movingBody1);

   }


    string vertixFile = shaderSourceRead("shader.vert");
    const char* vertixSource = vertixFile.c_str();
    string fragmentFile = shaderSourceRead("shader.frag");
    const char* fragmentSource = fragmentFile.c_str();
    string pathFile = shaderSourceRead("path.vert");
    const char* pathvertixSource = pathFile.c_str();
    //// 
    vector<vector<double>> path = {};
    unsigned int vertixShader, fragmentShader,shaderProgram,pathvertixShader,pathProgram;

    ////* --create Vertix Shader --
    vertixShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertixShader, 1, &vertixSource, NULL);
    glCompileShader(vertixShader);

    pathvertixShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(pathvertixShader, 1, &pathvertixSource, NULL);
    glCompileShader(pathvertixShader);



    ////* --create Fragment Shader--
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    ////* --create Shader Program--
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertixShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


    pathProgram = glCreateProgram();
    glAttachShader(pathProgram, pathvertixShader);
    glAttachShader(pathProgram, fragmentShader);
    glLinkProgram(pathProgram);

    ////
    ////* --clean shaders--
    glDeleteShader(vertixShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(pathvertixShader);

    

    ////* --prepare circle tamplet
    float* circleTamplet = circlePath();
    unsigned int VAO, VBO,MASSVBO,POSITIONSVBO , ColorVBO,RediueseVBO;
    glGenBuffers(5, world.VBOS);
    glGenVertexArrays(1, &VAO);
    glBindBuffer(GL_ARRAY_BUFFER, world.VBOS[0]);
    glBufferData(GL_ARRAY_BUFFER, 720 * sizeof(float), circleTamplet, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);










 


    //mass

    vector<float> Masses = world.getBodiesMasses();
    glBindBuffer(GL_ARRAY_BUFFER, world.VBOS[1]);
    glBufferData(GL_ARRAY_BUFFER, Masses.size()*sizeof(float), Masses.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    //position


    vector<float> initalPostions = world.getBodiesPositions();

    glBindBuffer(GL_ARRAY_BUFFER, world.VBOS[2]);
    glBufferData(GL_ARRAY_BUFFER, initalPostions.size()* sizeof(float), initalPostions.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);


    //colors


    vector<float> colors = world.getBodiesColors();

    glBindBuffer(GL_ARRAY_BUFFER, world.VBOS[3]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    //rediuses

    vector<float> rediuses = world.getBodiesRadiuses();

    glBindBuffer(GL_ARRAY_BUFFER, world.VBOS[4]);
    glBufferData(GL_ARRAY_BUFFER, rediuses.size() * sizeof(float), rediuses.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);















    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    int aspectRatioLocation= glGetUniformLocation(shaderProgram, "aspectRatio");
    int shift = glGetUniformLocation(shaderProgram, "scroll");
    int zoom = glGetUniformLocation(shaderProgram, "zoom");
    int zoomPath = glGetUniformLocation(pathProgram, "zoom");

    int shiftPath = glGetUniformLocation(pathProgram, "scroll");
    while (!glfwWindowShouldClose(window)) {
        Point shifter = Point(0, 0);  
        if (world.BodyFollow != -1  ) {
            shifter = Point(world.bodies[world.BodyFollow].postion.x,-world.bodies[world.BodyFollow].postion.y);
            world.currentShift = shifter;
        }
        else {
            shifter = Point(world.currentShift.x + world.scroll.x,world.currentShift.y+ world.scroll.y);

        }
        glfwGetWindowSize(window,&world.width,&world.heigth);

        int viewPortWidth = world.width * 0.8;
        glViewport(world.width*0.2, 0, viewPortWidth, world.heigth);
        glUniform1f(aspectRatioLocation, viewPortWidth / (world.heigth * 1.0));
        glUniform2f(shift, shifter.x, shifter.y);
        glUniform1f(zoom, world.zoom);

        glClear(GL_COLOR_BUFFER_BIT);
       

        glUseProgram(pathProgram);    

        
            glUniform2f(shiftPath, shifter.x, shifter.y);
        
            glUniform1f(zoomPath, world.zoom);

        world.updateBodies(viewPortWidth / (world.heigth * 1.0));
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        showObjectsPropertiesSettings(world);
        
     

   
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, world.VBOS[2]);
        vector<float> postions = world.getBodiesPositions();
        glBufferSubData(GL_ARRAY_BUFFER, 0, postions.size() * sizeof(float), postions.data());
        glDrawArraysInstanced(GL_POLYGON, 0, 360, world.bodies.size());

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if (x > world.width * 0.2) {
            glfwSetScrollCallback(window, scroll_callback);

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                if (world.BodyFollow != -1) {
                    world.currentShift = Point(world.bodies[world.BodyFollow].postion.x, -world.bodies[world.BodyFollow].postion.y);
                    world.BodyFollow = -1;
                }
                if (world.isScrolling == false) {

                    world.lastPoisiton = Point(x / world.width, y / world.heigth);

                    world.isScrolling = true;

                }

            }
            else {
                if (world.isScrolling == true) {
                    world.currentShift = Point(world.currentShift.x + world.scroll.x, world.currentShift.y + world.scroll.y);
                    world.scroll = Point(0, 0);
                }
                world.isScrolling = false;
            }
        }

        if (world.isScrolling == true) {
            world.scroll = Point((world.lastPoisiton.x - (x / world.width)) / world.zoom, (world.lastPoisiton.y - (y / world.heigth))/world.zoom);

        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();




    }
    glDeleteProgram(shaderProgram);
    glDeleteBuffers(5, world.VBOS);
    for (int i = 0; i < world.bodies.size(); i++) {
        glDeleteBuffers(1, &world.bodies[i].VBO);
        glDeleteVertexArrays(1, &world.bodies[i].VAO);
    }

    

    glDeleteVertexArrays(1, &VAO);
    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwDestroyWindow(window);

        glfwTerminate();
        return 0;
    }

