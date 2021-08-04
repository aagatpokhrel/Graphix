#include "canvas.h"
#include "camera.h"
#include "maths.h"
#include <chrono>
#include "mesh.h"

float lastX = 800;
float lastY = 500;
bool mouseLeftDown = false;

//Global Setup
Canvas* canvas;
Camera* camera;
Mesh* mesh;

float deltaTime = 0;


void processKeys(unsigned char key, int x, int y){
    
    if (key == GLUT_KEY_END)
        glutDestroyWindow(0);
    if (key== 's')
        camera->processKeyboard(FORWARD, deltaTime);
    if (key== 'w')
        camera->processKeyboard(BACKWARD, deltaTime);
    if (key== 'd')
        camera->processKeyboard(LEFT, deltaTime);
    if (key== 'a')
        camera->processKeyboard(RIGHT, deltaTime);
    if (key=='z')
        camera->processKeyboard(ZOOMIN,deltaTime);
    if (key=='x')
        camera->processKeyboard(ZOOMOUT,deltaTime);
}

void processMouse(int xpos, int ypos) {

    static float lastX = 800;
    static float lastY = 500;
    static bool firstMouse = true;
}

void mouseCB(int button,int state,int xpos, int ypos)
{
    // std::cout << xpos << "\t" << ypos << "\n";
        /*if (firstMouse == true)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }*/
    if ((button == GLUT_LEFT_BUTTON))
    {
        if (state == GLUT_DOWN) {

            mouseLeftDown = true;
        }
        else {
            mouseLeftDown = false;
        }

    }

}
void mouseMotionCB(int xpos, int ypos) {
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //firstMouse = false;

    camera->processMouseMovement(xoffset, yoffset);
 }

void renderer(){

    //Calculate deltatime and framePerSecond
    static float lastFrame = 0;
    float currentFrame = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentFrame - lastFrame)/1000;
    lastFrame = currentFrame;


    maths::mat4f view = camera->getViewMatrix();
    maths::mat4f projection = maths::perspective(maths::radians(camera->Zoom), (float)canvas->scrWidth/canvas->scrHeight);
    // maths::mat4f projection = maths::persproject();
    // maths::mat4f view_projection = maths::mul(projection,view);
    mesh->setView(view);
    mesh->setProjection(projection);
    mesh->update();
    mesh->draw();

    canvas->update();
    canvas->display();

}

int main(int argc, char** argv){
    
    //Canvas setup
    canvas = new Canvas(argc, argv);

    //Camera setup
    camera = new Camera(maths::vec3f{0.0f, 0.0f, 0.0f});

    //Creating mesh
    mesh=new Mesh(canvas);
    // mesh->load("../res/cube.obj");
    mesh->parse("../res/sphere.obj");
    mesh->camera = camera;
    mesh->translate(10.0,10.0,0.0);
    mesh->scale(30.0,30.0,30.0);

    //Glut specific functions
    glutDisplayFunc(renderer);
    glutKeyboardFunc(processKeys);
    //glutPassiveMotionFunc(processMouse);
    //glutPassiveMotionFunc(processMouse);
    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);
    //glutMouseFunc(processMouse);
    glutMainLoop();

}