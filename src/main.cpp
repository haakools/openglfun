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
#define DELTA_T 0.01

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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
    };
};

struct Planet {
    double x;
    double y;
    double m;
    double r;
    // constructor
    Planet(double x_pos, double y_pos, double mass, double radius ) : x(x_pos), y(y_pos), m(mass), r(radius) {
    }

    void draw(const std::vector<Planet>& planets) {
        glPointSize(10.0f);
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
        Planet(0.5, 0.5, 500, 2), 
        Planet(-0.5, 0.5, 500, 2), 
    };
    
    while (!glfwWindowShouldClose(engine.window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Physics logic
        for (size_t i = 0; i < planets.size(); i++) {
            planets[i].step(0, -0.1);  
        }

        // Rendering - order important        
        engine.render();
        planets[0].draw(planets);  
        
        // Cleanup stuff
        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}