#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <tuple>
#include <cmath>
#include <cassert>

#define WIDTH 480
#define HEIGHT 480

// Making the width and height be 10x lunar distance for space enough
#define LUNAR_DISTANCE 3.84399e8
#define WIDTH_M 3.84399e9
#define HEIGHT_M 3.84399e9

#define DELTA_T     0.1
#define MOON_SPEED  1670000
#define MOON_R      1.7375e6

// physics
#define MOON_M      7.347e22
#define EARTH_M     5.972e24
#define G           6.67430e-11 // gravitational constant

typedef struct vec2
{
    // 2d vector used for pos, vel and acc
    double pos_x;
    double pos_y;

    // speed
    double vel_x;
    double vel_y;

    // acceleration
    double acc_x;
    double acc_y;

    // constructor
    vec2(
        double x = 0.0, double y = 0.0,
        double vel_x = 0.0, double vel_y = 0.0,
        double acc_x = 0.0, double acc_y = 0.0) : pos_x(x), pos_y(y),
                                                  vel_x(vel_x), acc_x(acc_x),
                                                  vel_y(vel_y), acc_y(acc_y)
    {
        std::cout << "Starting with speed (x, y) -> (" << vel_x << " , " << vel_y << " ) " << std::endl;

    }
} vec2;

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
    vec2 motions;
    double m;
    double r; // this radius would be for collision detection and visualization
    // constructor
    Planet(vec2 motions, double mass, double radius ) : motions(motions),  m(mass), r(radius) {
    }

void draw(const std::vector<Planet>& planets) {
    glColor3f(1.0f, 1.0f, 1.0f);
    int circ_segments = 30;
    
    for (int i = 0; i < planets.size(); i++) {
        // Normalize coordinates from SI units to screen coordinates
        float center_x = planets[i].motions.pos_x / WIDTH_M;
        float center_y = planets[i].motions.pos_y / HEIGHT_M;
        
        float radius = planets[i].r / WIDTH_M;
        
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(center_x, center_y); 
        
        for (int j = 0; j <= circ_segments; j++) {  
            float angle = 2.0f * M_PI * j / circ_segments;
            float x = center_x + radius * cos(angle);
            float y = center_y + radius * sin(angle);

            glVertex2f(x, y);
            std::cout << "Drawing at (x, y) -> (" << x << " , " << y << " ) " << std::endl;
        }
        glEnd();
    }
}


    void physics_step(std::vector<Planet>& planets) {
        for (size_t i = 0; i < planets.size(); i++) {
            // a1 = G m2  /r2
            // a2 = G m1  /r2
            std::tuple<double, double> acc = calculate_sum_gravity_pull(planets, i);
            planets[i].motions.acc_x = std::get<0>(acc);
            planets[i].motions.acc_y = std::get<1>(acc);

            planets[i].motions.vel_x += planets[i].motions.acc_x * DELTA_T;
            planets[i].motions.vel_y += planets[i].motions.acc_y * DELTA_T;

            planets[i].motions.pos_x += planets[i].motions.vel_x * DELTA_T;
            planets[i].motions.pos_y += planets[i].motions.vel_y * DELTA_T;
            // std::cout << "Current speed (x, y) -> (" << planets[i].motions.vel_x << " , " << planets[i].motions.vel_y << " ) " << std::endl;
        }
    };

    double planet_distance(const std::vector<Planet>& planets, int planet_1, int planet_2) {
        assert(planet_1 < planets.size());
        assert(planet_2 < planets.size());
        return sqrt(std::pow(planets[planet_1].r,2) + std::pow(planets[planet_2].r, 2));
    };

    /** helper function/methods */
    std::tuple<double, double> calculate_sum_gravity_pull(const std::vector<Planet>& planets, int planet_index) {
        double acc_x = 0;
        double acc_y = 0;
        for (size_t i = 0; i < planets.size(); i++) {
            if (i == planet_index) {
                continue;
            }
            double distance_x = planets[planet_index].motions.pos_x - planets[i].motions.pos_x;
            double distance_y = planets[planet_index].motions.pos_y - planets[i].motions.pos_y;

            acc_x -= G*planets[i].m * distance_x / std::pow(planet_distance(planets, planet_index, i), 3);
            acc_y -= G*planets[i].m * distance_y / std::pow(planet_distance(planets, planet_index, i), 3);

        }
        return std::make_tuple(acc_x, acc_y);
    };
};

int main() {
    Engine engine;
    //std::vector<Planet> planets = {
    //    Planet(
    //        vec2(-0.5*WIDTH_M, -0.5*WIDTH_M, MOON_SPEED/2, MOON_SPEED/2), 
    //        MOON_M, 
    //        50*MOON_R
    //    ), 
    //    Planet(
    //        vec2( 0.0*WIDTH_M, -0.5*WIDTH_M, MOON_SPEED/2, MOON_SPEED/2),
    //        MOON_M,
    //        50*MOON_R
    //    ), 
    //    Planet(
    //        vec2( 0.0*WIDTH_M,  0.0*WIDTH_M, MOON_SPEED*0.0, MOON_SPEED*0.0),
    //        EARTH_M,
    //        500*MOON_R
    //    ), 
    //};
   
    // many planet setup
    /**
     * (-,+) | (+,+)
     * -------
     * (-,-) | (+, -)
     */


    std::vector<Planet> planets;
    int x_grid = 3;
    int y_grid = 3;

    std::cout << "setting up grid" << std::endl;
    for (int i=0; i < x_grid; i++){
        for (int j=0; j < y_grid; j++) {
            float x = 2*i/(x_grid-1) - 1;
            float y = 2*j/(y_grid-1) - 1;
            std::cout << "coordinates " << x << ", "<<   y << std::endl;

            planets.push_back(
                Planet(vec2(x * WIDTH_M, y * WIDTH_M, -x*MOON_SPEED, -y*MOON_SPEED), 1e4 * MOON_M, 20 * MOON_R)
            );
        }
    }

    
    while (!glfwWindowShouldClose(engine.window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Physics logic
        for (size_t i = 0; i < planets.size(); i++) {
            planets[i].physics_step(planets);
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