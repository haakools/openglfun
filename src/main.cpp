#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "linmath.h"
#define WIDTH 480
#define HEIGHT 480
#define DELTA_T 0.001

class Engine{
    public:
        GLFWwindow* window;
    Engine(){
        this->window = StartGLFW();
    }
    GLFWwindow* StartGLFW(){
        if(!glfwInit()){
            std::cout<<"glfw failed init, PANIC PANIC!"<<std::endl;
            return nullptr;
        }
        GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ray tracer", NULL, NULL);
        glfwMakeContextCurrent(window);
        
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW." << std::endl;
            glfwTerminate();
            return nullptr;
        }
        glViewport(0, 0, WIDTH, HEIGHT);
        return window;
    };
    void render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        // Add buffer swap and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    };
};

struct Planet {
    double x = WIDTH / 2;
    double y = HEIGHT / 2;
    double mass = 40;
    // constructor
    // struct planet(vec2 pos, vec2 dir) : x(pos.x), y(pos.y) {
    // }
    void draw(const std::vector<Planet>& planets) {
        glPointSize(50.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POINTS);
        for (size_t i = 0; i < planets.size(); i++) {
            glVertex2f(planets[i].x, planets[i].y);
        }
        glEnd();
    };
    void step(double dx, double dy) {
        x += DELTA_T*dx;
        y += DELTA_T*dy;
    }
};

int main() {
    Engine engine;
    std::vector<Planet> planets = {
        Planet()
    };
    
    while (!glfwWindowShouldClose(engine.window)) {
        for (size_t i = 0; i < planets.size(); i++) {
            planets[i].step(0, 0);  
        }
        
        engine.render();
        
        planets[0].draw(planets);  
    }
    
    // Cleanup
    glfwTerminate();
    return 0;
}