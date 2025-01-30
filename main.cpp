
#include<iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "basic_camera.h"
#include "camera.h"
#include "pointLight.h"
#include "cube.h"
#include "stb_image.h"
#include "SpotLight.h"

#include<vector>
#include<cmath>
#include "cylinder.h"
#include "cone.h"
#include "sphere.h"
#include "sphere.h"
#include "cylinder_base.h"
#include "cube_front.h"



using namespace std;

#define M_PI 3.14159265358979323846



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawFan(unsigned int VAO, Shader ourShader, glm::mat4 matrix);
void drawFurnace(unsigned int VAO, Shader ourShader, glm::mat4 matrix);
void drawCube(unsigned int& VAO, Shader& lightingShader, glm::mat4 model, glm::vec3 color);
//void drawSofas(unsigned int VAO, Shader lightingShader, glm::mat4 matrix);
void lightEffect(unsigned int VAO, Shader lightShader, glm::mat4 model, glm::vec3 color);
void read_file(string file_name, vector<float>& vec);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector<float>& coordinates, vector<float>& normals, vector<int>& indices, vector<float>& vertices, float div);
//void trans(Cube& cube, Cube& glass, Shader& lightingShader, glm::mat4 als);
//void r_window(Cube& glass, Shader& lightingShader, glm::mat4 als);
//void windows(Cube& glass, Shader& lightingShader, glm::mat4 als);
//void tree(Sphere2& sphere, Cylinder2& cyl, Shader& lightingShader, glm::mat4 als);
void drawTree(unsigned int trr, unsigned int twood, Shader ourShader, glm::mat4 matrix);
void washingmachine(Cube& cube, Cube& cube2, Shader& lightingShader, glm::mat4 als);
void drawSofas(Cube& cube, Shader& lightingShader, glm::mat4 als);
void drawWheel(unsigned int tire, unsigned int ring, Shader ourShader, glm::mat4 matrix, float rotationAngle);
void bed(Cube& frame, Cube& body, Cube& foam, Cube blank, Shader& lightingShader, glm::mat4 als);
void tapwater(Cube& cube, Shader& lightingShader, glm::mat4 als);
unsigned int treeSetUp();
void generateTree(std::vector<float>& vertices, std::vector<unsigned int>& indices, glm::vec3 start, glm::vec3 direction, float length, int depth, int maxDepth);
void drawLamppost(Cube& ver, Cube& hor, Shader ourShader, glm::mat4 matrix);


// Bus Door Variables
float busDoorPositionX = 20.5f;    // Initial position of the bus door along the X-axis
float busDoorSpeed = 5.0f;         // Speed of translation
bool isBusDoorMovingForward = false; // Tracks forward movement (H key)
bool isBusDoorMovingBackward = false; // Tracks backward movement (J key)
// Key States
static bool isKeyHPressed = false; // Tracks whether the H key was previously pressed
static bool isKeyJPressed = false; // Tracks whether the J key was previously pressed



//tree generation
vector<float> treeVertices;
std::vector<unsigned int> treeIndices;
unsigned int fractalVAO;



//wheel rotation
bool rotateWheel = false; // Tracks whether the wheel should rotate
float wheelRotationAngle = 0.0f; // Tracks the current rotation angle
float rotationSpeed = 30.0f; // Increase the speed of rotation (degrees per frame)
int rotationDirection = 1;       // 1 for forward, -1 for backward


//bathroom door rotation
float bathdoor_rotationAngle = 0.0f;
float bathdoor_rotationSpeed = 5.0f; // Degrees per second
bool bathdoor_isRotatingForward = false; // Flag for forward rotation
bool bathdoor_isRotatingReverse = false; // Flag for reverse rotation


//scene transition
float translationOffsetX = 0.0f;    // X-axis offset for movement
float movementSpeed = 5.0f;         // Movement speed (units per second)
bool isMovingForward = false;       // Flag for forward movement
bool isMovingBackward = false;      // Flag for backward movement
static bool isKeyLPressed = false; // Tracks whether the L key was previously pressed
static bool isKeyKPressed = false; // Tracks whether the K key was previously pressed


//bed tranlation
bool bedIsMovingUp = false;    // Tracks upward movement
bool bedIsMovingDown = false; // Tracks downward movement
float bedYTranslation = 0.0f; // Current Y translation
float bedTranslationSpeed = 2.0f; // Speed of translation
float bedMaxYTranslation = 5.0f; // Upper limit for Y-axis movement
float bedMinYTranslation = 0.0f; // Lower limit for Y-axis movement



//tap water
bool watr_tap = false;
float tap = 0.0;


//bezier
const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 30;

unsigned int sphereVAO, coneVAO, treeTopVAO, treeMidVAO, basinVAO;

vector <float> cntrlPointsSphere, cntrlPointsCone, cntrlPointsTreeTop, cntrlPointsTreeMid, cntrlPointsBasin;
vector <float> coordinatesSphere, coordinatesCone, coordinatesTreeTop, coordinatesBasin;
vector <float> normalsSphere, normalsCone, normalsTreeTop, normalsBasin;
vector <int> indicesSphere, indicesCone, indicesTreeTop, indicesBasin;
vector <float> verticesSphere, verticesCone, verticesTreeTop, verticesBasin;


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


//texture on/off
bool useTexture = true;


//cylinder
const int SEGMENTS = 36; // Number of segments for the circle
const float RADIUS = 0.5f;
const float HEIGHT = 2.0f;

// Vertices and indices arrays
float lampVertices[(SEGMENTS + 1) * 6 * 2 + 12]; // (Position + Normal) * 2 (top and bottom) + 2 center points
unsigned int lampIndices[SEGMENTS * 12];        // Indices for sides and caps

float eyeX = 5.0f, eyeY = 4.0f, eyeZ = 25.0f;
float lookAtX = 1.25f, lookAtY = 4.0f, lookAtZ = 10.0f;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 eye = glm::vec3(eyeX, eyeY, eyeZ);
glm::vec3 lookAt = glm::vec3(lookAtX, lookAtY, lookAtZ);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
Camera camera(glm::vec3(eyeX, eyeY, eyeZ));

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

float r = 0.0f;
bool fanOn = false;
float aspectRatio = 4.0f / 3.0f;

bool birdEyeView = false;
glm::vec3 birdEyePosition(5.25f, 10.0f, 10.0f); // Initial position (10 units above)
glm::vec3 birdEyeTarget(5.25f, 0.0f, 6.0f);   // Focus point
float birdEyeSpeed = 1.0f;                   // Speed of movement

float theta = 0.0f; // Angle around the Y-axis
float radius = 2.0f; // Distance from lookAt point

//directional light
bool directionLightOn = true;
bool directionalAmbient = true;
bool directionalDiffuse = true;
bool directionalSpecular = true;

//spot light
bool spotLightOn = true;

//point light
bool point1 = true;
bool point2 = true;
bool point3 = true;
bool point4 = true;
bool point5 = true;
bool point6 = true;
bool point7 = true;
bool point8 = true;
bool point9 = true;
bool point10 = true;
bool point11 = true;
bool point12 = true;
bool point13 = true;

void drawChair(Cube& cube, Shader& lightingShader, glm::mat4 als);
void drawTable(Cube& table, Shader& ourShader, glm::mat4 matrix);

void drawCylinder(Shader shaderProgram, glm::vec4 color, unsigned int VAO, glm::mat4 parentTrans, float posX = 0.0, float posY = 0.0, float posZ = 0.0, float rotX = 0.0, float rotY = 0.0, float rotZ = 0.0, float scX = 1.0, float scY = 1.0, float scZ = 1.0);


void drawCylinder(unsigned int& VAO_C, Shader& lightingShader, glm::vec3 color, glm::mat4 model, std::vector<unsigned int>& indices);
void drawSphere(unsigned int& VAO_S, Shader& lightingShader, glm::vec3 color, glm::mat4 model, std::vector<unsigned int>& indices);
void generateCylinder(float radius, float height, int segments, std::vector<float>& vertices, std::vector<unsigned int>& indices);
void generateCone(float radius, float height, int segments, std::vector<float>& vertices, std::vector<unsigned int>& indices);
void generateSphere(float radius, int sectorCount, int stackCount, std::vector<float>& vertices, std::vector<unsigned int>& indices);


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(2.0f,  7.0f,  0.0f),
    glm::vec3(0.0f,  7.0f,  7.0f),
    glm::vec3(23.0f,  7.0f,  0.0f),
    glm::vec3(42.0f,  6.8f,  -3.0f),
    glm::vec3(62.0f,  6.8f,  -3.0f),
    glm::vec3(82.0f,  6.8f,  -3.0f),
    glm::vec3(102.0f,  6.8f,  -3.0f),
    glm::vec3(122.0f,  6.8f,  -3.0f),
    glm::vec3(142.0f,  6.8f,  -3.0f),
    glm::vec3(162.0f,  6.8f,  -3.0f),
    glm::vec3(182.0f,  6.8f,  -3.0f),
    glm::vec3(202.0f,  6.8f,  -3.0f),
    glm::vec3(222.0f,  6.8f,  -3.0f),

    
};

PointLight pointlight1(
    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,       // position
    0.2f, 0.2f, 0.2f,       //ambient
    0.8f, 0.8f, 0.8f,       //diffuse
    0.2f, 0.2f, 0.2f,       //specular
    1.0f,       //k_c
    0.09f,      //k_l
    0.032f,     //k_q
    1       //light number
);

PointLight pointlight2(
    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,
    0.2f, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    2
);

PointLight pointlight3(
    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,
    0.2f, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    3
);
PointLight pointlight4(
    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,
    .2, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    4
);
PointLight pointlight5(
    pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z,
    .2, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    5
);
PointLight pointlight6(
    pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z,
    .2, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    6
);
PointLight pointlight7(
    pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z,
    .2, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    7
);
PointLight pointlight8(
    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,
    .2, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    8
);
PointLight pointlight9(
    pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z,
    .2, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    9
);
PointLight pointlight10(
    pointLightPositions[9].x, pointLightPositions[9].y, pointLightPositions[9].z,
    .2, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    10
);
PointLight pointlight11(
    pointLightPositions[10].x, pointLightPositions[10].y, pointLightPositions[10].z,
    .2, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    11
);
PointLight pointlight12(
    pointLightPositions[11].x, pointLightPositions[11].y, pointLightPositions[11].z,
    .2, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    12
);
PointLight pointlight13(
    pointLightPositions[12].x, pointLightPositions[12].y, pointLightPositions[12].z,
    .2, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    13
);

//spot light
SpotLight spotlight1(
    40, 10, 0,  // position
    0.0f, 0.0f, 0.0f,     // ambient
    0.0f, 0.0f, 0.0f,      // diffuse
    0.0f, 0.0f, 0.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1,       // light number
    glm::cos(glm::radians(20.5f)),
    glm::cos(glm::radians(25.0f)),
    0, -1, 0
);

// light settings
bool onOffToggle = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool dl = true;
bool spt = true;


glm::vec3 amb(0.2f, 0.2f, 0.2f);
glm::vec3 def(0.8f, 0.8f, 0.8f);
glm::vec3 spec(1.0f, 1.0f, 1.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "1907056: Computer Graphics Project", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    //Shader constantShader("vertexShader.vs", "fragmentShaderV2.fs");

    /*Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");*/

    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");


    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };


    // Generate cylinder vertices and indices
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    generateCylinder(0.25f, 0.5f, 36, vertices, indices);

    // Generate cone data
    std::vector<float> vertices_k;
    std::vector<unsigned int> indices_k;
    generateCone(1.0f, 2.0f, 36, vertices_k, indices_k);

    // Generate sphere data
    std::vector<float> vertices_s;
    std::vector<unsigned int> indices_s;
    generateSphere(1.0f, 36, 18, vertices_s, indices_s);

    // Create VAO_S, VBO_S, and EBO_S
    unsigned int VAO_S, VBO_S, EBO_S;
    glGenVertexArrays(1, &VAO_S);
    glGenBuffers(1, &VBO_S);
    glGenBuffers(1, &EBO_S);

    // Bind VAO
    glBindVertexArray(VAO_S);

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_S);
    glBufferData(GL_ARRAY_BUFFER, vertices_s.size() * sizeof(float), vertices_s.data(), GL_STATIC_DRAW);

    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_S);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_s.size() * sizeof(unsigned int), indices_s.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);


    //for cube
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    read_file("bathtub.txt", cntrlPointsSphere);
    sphereVAO = hollowBezier(cntrlPointsSphere.data(), ((unsigned int)cntrlPointsSphere.size() / 3) - 1, coordinatesSphere, normalsSphere, indicesSphere, verticesSphere, 2.0);

    read_file("sphere.txt", cntrlPointsBasin);
    basinVAO = hollowBezier(cntrlPointsBasin.data(), ((unsigned int)cntrlPointsBasin.size() / 3) - 1, coordinatesBasin, normalsBasin, indicesBasin, verticesBasin, 2.0);


    read_file("tree_top.txt", cntrlPointsTreeTop);
    treeTopVAO = hollowBezier(cntrlPointsTreeTop.data(), ((unsigned int)cntrlPointsTreeTop.size() / 3) - 1, coordinatesTreeTop, normalsTreeTop, indicesTreeTop, verticesTreeTop, 1.0);

    
    generateTree(treeVertices, treeIndices, glm::vec3(0,0,0), glm::vec3(0,1,0), 2, 5, 5);
    fractalVAO = treeSetUp();


    //floor texture
   /* string diffuseMapPath = "floor.jpeg";
    string specularMapPath = "floor.jpeg";*/
    string diffuseMapPath = "floor2.jpg";
    string specularMapPath = "floor2.jpg";
    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 20.5f, 10.0f);

    //area texture
    string diffuseMapPath1 = "grass3.jpg";
    string specularMapPath1 = "grass3.jpg";
    unsigned int diffMap1 = loadTexture(diffuseMapPath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap1 = loadTexture(specularMapPath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube1 = Cube(diffMap1, specMap1, 32.0f, 0.0f, 0.0f, 200.5f, 200.0f);

    //basin texture
    string diffuseMapPath2= "basin.jpg";
    string specularMapPath2 = "basin.jpg";
    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    //sky texture
    string diffuseMapPath3 = "sky.jpg";
    string specularMapPath3 = "sky.jpg";
    unsigned int diffMap3 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube3 = Cube(diffMap3, specMap3, 32.0f, 0.0f, 0.0f, 20, 20);


    //sphere texture
    string diffuseMapPath8 = "football.jpg";
    string specularMapPath8 = "football.jpg";
    unsigned int diffMap8 = loadTexture(diffuseMapPath8.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap8 = loadTexture(specularMapPath8.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    //sphere texture
    string diffuseMapPath9 = "cone.jpg";
    string specularMapPath9 = "cone.jpg";
    unsigned int diffMap9 = loadTexture(diffuseMapPath9.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap9 = loadTexture(specularMapPath9.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    //main road
    string main_road = "main_road.jpg";
    unsigned int mroad = loadTexture(main_road.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube road_m = Cube(mroad, mroad, 32.0f, 0.0f, 0.0f, 30.0f, 1.0f);

    string tv = "tv.png";
    unsigned int tvv = loadTexture(tv.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube vtv = Cube(tvv, tvv, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string tr = "tree2.jpg";
    string tw = "tree_w.jpg";
    unsigned int trr = loadTexture(tr.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int twood = loadTexture(tw.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //Cylinder2 tree_w = Cylinder2(twood, twood, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    //Sphere2 stree = Sphere2(1.5f, 36, 18, glm::vec3(0.4, 0.4, 0.4), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.3, 0.3, 0.3), 10.0f, trr, trr, 2.0f, 2.0f, 3.0f, 3.0f);


    string toy2 = "machine.jpg";
    string blck = "black.jpg";
    unsigned int tys2 = loadTexture(toy2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int blk = loadTexture(blck.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube blacktl = Cube(blk, blk, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube toys2 = Cube(tys2, tys2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string ovn = "oven.jpg";
    unsigned int ov = loadTexture(ovn.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube stove = Cube(ov, ov, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string tile_path = "wtiles.jpg";
    unsigned int tile_p = loadTexture(tile_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube tile = Cube(tile_p, tile_p, 32.0f, 0.0f, 0.0f, 10.0f, 10.0f);


    string wall_path = "wall3.png";
    unsigned int wall_p = loadTexture(wall_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall = Cube(wall_p, wall_p, 32.0f, 0.0f, 0.0f, 10.0f, 10.0f);

    string flash_path = "tiles.jpg";
    unsigned int flash_p = loadTexture(flash_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube flash = Cube(flash_p, flash_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string steel_path = "steel.jpg";
    unsigned int steel = loadTexture(steel_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    

    string tire_path = "carw.jpg";
    string ring_path = "carw.png";
    unsigned int tire = loadTexture(tire_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int ring = loadTexture(ring_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


    string sofa_path = "sofa_seat2.jpg";
    string sofa_path2 = "bed_head.png";
    string sofa_path3 = "sofa_seat.jpg";
    unsigned int sofa_p = loadTexture(sofa_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sofa_p2 = loadTexture(sofa_path2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int sofa_p3 = loadTexture(sofa_path3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Cube sofa2 = Cube(sofa_p2, sofa_p2, 32.0f, 0.0f, 0.0f, 3.0f, 3.0f);
    Cube sofa = Cube(sofa_p, sofa_p, 32.0f, 0.0f, 0.0f, 3.0f, 1.0f);
    Cube sofa3 = Cube(sofa_p3, sofa_p3, 32.0f, 0.0f, 0.0f, 3.0f, 3.0f);

    string chair_path = "chair.jpg";
    unsigned int chair_p = loadTexture(chair_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube chair = Cube(chair_p, chair_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string table_path = "chair.jpg";
    unsigned int table_p = loadTexture(table_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube table = Cube(table_p, table_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string oven_path = "oven2.jpg";
    string ovenkeypad_path = "oven_keypad.jpg";
    string led_path = "led.jpg";
    unsigned int oven_p = loadTexture(oven_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int ovenkeypad_p = loadTexture(ovenkeypad_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int led_p = loadTexture(led_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube oven_keypad = Cube(ovenkeypad_p, oven_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube oven = Cube(oven_p, oven_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube led = Cube(led_p, led_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string fridge_keypad_path = "fridge_keypad.jpg";
    unsigned int fridge_keypad_p = loadTexture(fridge_keypad_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube fridge_keypad = Cube(fridge_keypad_p, fridge_keypad_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string bdf = "bed_head.png";
    unsigned int fbd = loadTexture(bdf.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bdfram = Cube(fbd, fbd, 32.0f, 0.0f, 0.0f, 3.0f, 1.0f);

    string bdbo = "bed_head.png";
    unsigned int bobd = loadTexture(bdbo.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bdbody = Cube(bobd, bobd, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string bdfm = "foam_bed.jpg";
    unsigned int fmbd = loadTexture(bdfm.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bdfoam = Cube(fmbd, fmbd, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string blank = "blank.jpg";
    unsigned int blan = loadTexture(blank.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube blankt = Cube(blan, blan, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string door_path = "door4.png";
    unsigned int door_p = loadTexture(door_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bathroom_door = Cube(door_p, door_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string rvright_path = "rv_right.png";
    unsigned int rvright_p = loadTexture(rvright_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube rvright = Cube(rvright_p, rvright_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string watr = "water.jpg";
    unsigned int wtr = loadTexture(watr.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube water = Cube(wtr, wtr, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string rvfront_path = "rv_front2.png";
    unsigned int rvfront_p = loadTexture(rvfront_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube rvfront = Cube(rvfront_p, rvfront_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string busdoor_path = "bus_door.png";
    unsigned int busdoor_p = loadTexture(busdoor_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube busdoor = Cube(busdoor_p, busdoor_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string speedometer_path = "speedometer.jpg";
    unsigned int speedometer_p = loadTexture(speedometer_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube speedometer = Cube(speedometer_p, speedometer_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string painting_path = "paintings.png";
    unsigned int painting_p = loadTexture(painting_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube painting = Cube(painting_p, painting_p, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);




    r = 0.0f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //ourShader.use();
        


        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        pointlight4.setUpPointLight(lightingShader);
        pointlight5.setUpPointLight(lightingShader);
        pointlight6.setUpPointLight(lightingShader);
        pointlight7.setUpPointLight(lightingShader);
        pointlight8.setUpPointLight(lightingShader);
        pointlight9.setUpPointLight(lightingShader);
        pointlight10.setUpPointLight(lightingShader);
        pointlight11.setUpPointLight(lightingShader);
        pointlight12.setUpPointLight(lightingShader);
        pointlight13.setUpPointLight(lightingShader);


        //spot light 1
        spotlight1.setUpspotLight(lightingShader);



        lightingShader.setVec3("directionalLight.direction", 0.0f, -24.0f, 0.0f);
        lightingShader.setVec3("directionalLight.ambient", 0.2, 0.2, 0.2);
        lightingShader.setVec3("directionalLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setBool("directionLightOn", directionLightOn);

        //lightingShader.setBool("dlighton", dl);

        lightingShader.setVec3("spotlight.position", 20.1, 7, 0.1);
        lightingShader.setVec3("spotLight.direction", 5.0f, 5.0f, 5.0f);
        lightingShader.setVec3("spotLight.ambient", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("spotLight.specular", 0.2f, 0.2f, 0.2f);
        lightingShader.setFloat("spotLight.k_c", 1.0f);
        lightingShader.setFloat("spotLight.k_l", 0.09);
        lightingShader.setFloat("spotLight.k_q", 0.032);
        //lightingShader.setFloat("cos_theta", glm::cos(glm::radians(5.5f)));
        lightingShader.setFloat("spotLight.inner_circle", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outer_circle", glm::cos(glm::radians(25.0f)));
        lightingShader.setBool("spotLightOn", spotLightOn);

        //handle for changes in directional light directly from shedder
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
            if (directionLightOn) {
                lightingShader.setBool("ambientLight", !directionalAmbient);
                directionalAmbient = !directionalAmbient;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
            if (directionLightOn) {
                lightingShader.setBool("diffuseLight", !directionalDiffuse);
                directionalDiffuse = !directionalDiffuse;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
            if (directionLightOn) {
                lightingShader.setBool("specularLight", !directionalSpecular);
                directionalSpecular = !directionalSpecular;
            }
        }

        // Update wheel rotation
        if (rotateWheel) {
            // Update the wheel rotation angle based on the direction
            wheelRotationAngle += rotationSpeed * rotationDirection;

            // Keep the angle within [0, 360)
            if (wheelRotationAngle >= 360.0f) {
                wheelRotationAngle -= 360.0f;
            }
            else if (wheelRotationAngle < 0.0f) {
                wheelRotationAngle += 360.0f;
            }
        }



        //bus door
        // Update Bus Door Position
        if (isBusDoorMovingForward) {
            busDoorPositionX += busDoorSpeed * deltaTime; // Move forward (positive X)
        }
        if (isBusDoorMovingBackward) {
            busDoorPositionX -= busDoorSpeed * deltaTime; // Move backward (negative X)
        }



        //door rotation
        // Handle forward rotation if it's enabled
        if (bathdoor_isRotatingForward) {
            // Increment the rotation angle for forward rotation
            bathdoor_rotationAngle += bathdoor_rotationSpeed * deltaTime;

            // Cap the rotation angle to 90 degrees
            if (bathdoor_rotationAngle >= 90.0f) {
                bathdoor_rotationAngle = 90.0f;
                bathdoor_isRotatingForward = false; // Stop forward rotation at 90 degrees
            }
        }

        // Handle reverse rotation if it's enabled
        if (bathdoor_isRotatingReverse) {
            // Increment the rotation angle for reverse rotation (going back to 0 degrees)
            bathdoor_rotationAngle -= bathdoor_rotationSpeed * deltaTime;

            // Cap the rotation angle to 0 degrees
            if (bathdoor_rotationAngle <= 0.0f) {
                bathdoor_rotationAngle = 0.0f;
                bathdoor_isRotatingReverse = false; // Stop reverse rotation at 0 degrees
            }
        }




        /*glm::vec4 pivot = glm::vec4(glm::vec3(19.0f, 3.3f, 10.0f), 1.0f);

        glm::vec3 translateToPivot = glm::translate(identityMatrix, glm::vec3(-pivot));
        translateFromPivot = glm::translate(identityMatrix, glm::vec3(pivot));

        float currentTime = static_cast<float>(glfwGetTime());
        if (currentTime - lastTime > 0.1)
        {
            if (isswingOn)
                swingAngle += swingSpeed;
            if (abs(swingAngle) >= swingMaxAngle)
            {
                swingSpeed *= -1;
                lastTime = static_cast<float>(glfwGetTime());
            }
        }
        if (!isswingOn)
        {
            if (swingAngle < 0)
            {
                swingAngle += abs(swingSpeed);
            }
            if (swingAngle > 0)
            {
                swingAngle -= abs(swingSpeed);
            }
        }

        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(swingAngle), glm::vec3(0.0f, 0.0f, 1.0f));
*/


        // Scene transition
        if (isMovingForward) {
            translationOffsetX += movementSpeed * deltaTime; // Move forward (positive X)
        }
        if (isMovingBackward) {
            translationOffsetX -= movementSpeed * deltaTime; // Move backward (negative X)
        }


        //bed translation
        // Continuous upward movement
        if (bedIsMovingUp) {
            bedYTranslation += bedTranslationSpeed * deltaTime;
            if (bedYTranslation >= bedMaxYTranslation) {
                bedYTranslation = bedMaxYTranslation; // Cap at maximum Y
                bedIsMovingUp = false;               // Stop upward movement
            }
        }

        // Continuous downward movement
        if (bedIsMovingDown) {
            bedYTranslation -= bedTranslationSpeed * deltaTime;
            if (bedYTranslation <= bedMinYTranslation) {
                bedYTranslation = bedMinYTranslation; // Cap at minimum Y
                bedIsMovingDown = false;              // Stop downward movement
            }
        }





        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(basic_camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        //ourShader.setMat4("projection", projection);
        lightingShader.setMat4("projection", projection);
        //constantShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = basic_camera.createViewMatrix();
        //ourShader.setMat4("view", view);
        //constantShader.setMat4("view", view);

        glm::mat4 view;

        if (birdEyeView) {
            // Set camera position directly above the scene
            glm::vec3 up(0.0f, 1.0f, 0.0f); // Ensure the up vector points backward
            view = glm::lookAt(birdEyePosition, birdEyeTarget, up);
        }
        else {
            view = camera.GetViewMatrix();
        }

        //ourShader.setMat4("view", view);

        lightingShader.setMat4("view", view);
        

        glm::mat4 identityMatrix = glm::mat4(1.0f);
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, RotateTranslateMatrix, InvRotateTranslateMatrix;
        glm::vec3 color;

        // Draw a sphere next to the sofa
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 3.65f, 5.0f)); // Adjust the offset as needed
        //model = translateMatrix * scaleMatrix;
        //drawSphere(VAO_S, lightingShader, glm::vec3(1.0f, 0.0f, 0.0f), model, indices_s);

        //sofa
        //drawSofa(VAO, lightingShader, identityMatrix);

        //first wall section 1
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.1f, 7.2f, 10.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(1.0, 0.98f, 0.97f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(1.0, 0.98f, 0.97f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //first wall section 2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 7.2f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.1f, 2.8f, 10.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.88, 0.75f, 0.64f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.88, 0.75f, 0.64f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //second wall section 1
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(20.5f, 7.2f, -0.1f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        lightEffect(VAO, lightingShader, model, glm::vec3(1.0, 0.98f, 0.97f));


        //second wall section 1 after door
        translateMatrix = glm::translate(identityMatrix, glm::vec3(24.0f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(7.0f, 7.2f, -0.1f));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        lightEffect(VAO, lightingShader, model, glm::vec3(1.0, 0.98f, 0.97f));
        

        //second wall section 2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 7.2f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(20.5f, 2.8f, -0.1f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.88, 0.75f, 0.64f));

        //second wall section 2 after door
        translateMatrix = glm::translate(identityMatrix, glm::vec3(24.0f, 7.2f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(7.0f, 2.8f, -0.1f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.88, 0.75f, 0.64f));
        
        //roof
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1f, 10.0f, -0.1f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(31.0f, 0.1f, 10.5f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.678, 0.678, 0.788));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.678, 0.678, 0.788, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


        //floor
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.0f, -0.1f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(30.5f, -0.1f, 10.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.85, 0.65f, 0.56f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.85, 0.65f, 0.56f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //upper box 1
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 3.7f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 2.0f, 2.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //upper boxes
        float z = 2.01f;
        float z_thin = 2.0f;
        for (int i = 0; i < 5; i++) {
            //thin separator
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.51f, 3.7f, z_thin));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 2.0f, 0.02f));
            model = translateMatrix * scaleMatrix;
            lightEffect(VAO, lightingShader, model, glm::vec3(0.0, 0.0f, 0.0f));
            /*ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
            z_thin += 1.0f;


            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 3.7f, z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 2.0f, 0.98f));
            model = translateMatrix * scaleMatrix;
            lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
            /*ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
            z += 1.0f;
        }
        
        
        //white surface 1
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 2.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 10.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(1.0, 0.98f, 0.97f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(1.0, 0.98f, 0.97f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


        //lamp base
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.30f, 2.2f, 0.30f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.1f, 0.2f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.0, 0.0f, 0.0f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


        //lamp stick
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.35f, 2.21f, 0.35f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.6f, 0.1f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.0, 0.0f, 0.0f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
        
        //lamp
        //drawCylinder(ourShader, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), cylinder_VAO, identityMatrix, 1.5f, 2.81f, 0.5f, 0, 0, 0, 0.4f, 0.5f, 0.4f);

        

       


        //white surface 2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 2.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(7.0f, 0.2f, 1.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(1.0, 0.98f, 0.97f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(1.0, 0.98f, 0.97f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


        //lower box 1
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 2.0f, 1.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


        //lower boxes (2-4)
        float z_lower = 2.01f;
        float z_thin_lower = 2.0f;
        for (int i = 0; i < 3; i++) {
            //thin separator
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.01f, 0.0f, z_thin_lower));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 2.0f, 0.02f));
            model = translateMatrix * scaleMatrix;
            lightEffect(VAO, lightingShader, model, glm::vec3(0.0, 0.0f, 0.0f));
            /*ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
            z_thin_lower += 1.0f;


            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, z_lower));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 2.0f, 0.98f));
            model = translateMatrix * scaleMatrix;
            lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
            /*ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
            z_lower += 1.0f;
        }

        
        //store bottom
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.4f, 2.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.64, 0.58f, 0.49f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.64, 0.58f, 0.49f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //store top
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 1.6f, 5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.4f, 2.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.64, 0.58f, 0.49f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.64, 0.58f, 0.49f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //store mid
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.4f, 5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.2f, 2.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.0, 0.0f, 0.0f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //store handle
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.01f, 1.2f, 5.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.05f, 1.65f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(1.0, 1.0f, 1.0f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(1.0, 1.0f, 1.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


        //lower boxes (5-7)
        float z_lower_left = 7.02f;
        float z_thin_lower_left = 7.0f;
        for (int i = 0; i < 3; i++) {
            //thin separator
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.01f, 0.0f, z_thin_lower_left));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 2.0f, 0.02f));
            model = translateMatrix * scaleMatrix;
            lightEffect(VAO, lightingShader, model, glm::vec3(0.0, 0.0f, 0.0f));
            /*ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
            z_thin_lower_left += 1.0f;


            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, z_lower_left));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 2.0f, 0.98f));
            model = translateMatrix * scaleMatrix;
            lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
            /*ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
            z_lower_left += 1.0f;
        }



        //lower boxes right (1-6)
        float x_lower_right = 1.12f;
        float x_thin_lower_right = 1.1f;
        for (int i = 0; i < 6; i++) {
            //thin separator
            translateMatrix = glm::translate(identityMatrix, glm::vec3(x_thin_lower_right, 0.0f,1.01f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 2.0f, 0.01f));
            model = translateMatrix * scaleMatrix;
            lightEffect(VAO, lightingShader, model, glm::vec3(0.0, 0.0f, 0.0f));
            /*ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
            x_thin_lower_right += 1.0f;


            translateMatrix = glm::translate(identityMatrix, glm::vec3(x_lower_right, 0.0f, 0.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.98f, 2.0f, 1.0f));
            model = translateMatrix * scaleMatrix;
            lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
            /*ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
            x_lower_right += 1.0f;
        }

        //upper box right 1
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 3.7f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


        //upper box right 1 white front
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 3.9f, 1.02f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.9f, 1.6f, 0.02f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.85, 0.65f, 0.56f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.85, 0.65f, 0.56f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/



        //window mid left
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 3.7f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 2.0f, 0.2f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //window mid 
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f, 3.7f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 2.0f, 0.2f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


        //window mid right
        translateMatrix = glm::translate(identityMatrix, glm::vec3(6.05f, 3.7f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 2.0f, 0.2f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //window top left 
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 5.7f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.7f, 0.15f, 0.2f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //window top right 
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f, 5.7f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.7f, 0.15f, 0.2f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //window bottom left 
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 3.7f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.7f, 0.15f, 0.2f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //window bottom right 
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f, 3.7f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.7f, 0.15f, 0.2f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.97, 0.48f, 0.21f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.97, 0.48f, 0.21f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


        //fridge back
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.1f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 5.0f, 1.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.65, 0.57f, 0.47f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.65, 0.57f, 0.47f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


        //fridge front left
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.1f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.2f, 5.0f, 0.2f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.65, 0.57f, 0.47f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.65, 0.57f, 0.47f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //fridge separator line
        translateMatrix = glm::translate(identityMatrix, glm::vec3(8.3f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 5.0f, 0.2f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.0, 0.0f, 0.0f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //fridge front right
        translateMatrix = glm::translate(identityMatrix, glm::vec3(8.32f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.78f, 5.0f, 0.2f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.65, 0.57f, 0.47f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.65, 0.57f, 0.47f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


        
        //lightEffect(fractalVAO, lightingShader, model, glm::vec3(0.65, 0.57f, 0.47f));

        /*translateMatrix = glm::translate(identityMatrix, glm::vec3(22, 0.0f, 15.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        glBindVertexArray(fractalVAO);
        glDrawElements(GL_LINES, treeIndices.size(), GL_UNSIGNED_INT, 0);*/

        //fridge left handle
        translateMatrix = glm::translate(identityMatrix, glm::vec3(8.0f, 1.9f, 1.21f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.5f, 0.1f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.0, 0.0f, 0.0f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //fridge right handle
        translateMatrix = glm::translate(identityMatrix, glm::vec3(8.5f, 1.9f, 1.21f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.5f, 0.1f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.0, 0.0f, 0.0f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        

        //sink
        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 2.21f, 0.05f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.01f, 0.9f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.0, 0.0f, 0.0f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //sink lines
        float sink_z = 0.1f;
        for (int i = 0; i < 10; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.55f, 2.22f, sink_z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 0.01f, 0.02f));
            model = translateMatrix * scaleMatrix;
            lightEffect(VAO, lightingShader, model, glm::vec3(1.0, 1.0f, 1.0f));
            /*ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(1.0, 1.0f, 1.0f, 1.0f));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
            sink_z += 0.1;
        }
        


        

        //tap 1
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 2.21f, 0.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 1.0f, 0.05f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.5, 0.5f, 0.5f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.5, 0.5f, 0.5f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //tap 2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 3.21f, 0.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.5f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.5, 0.5f, 0.5f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.5, 0.5f, 0.5f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

        //tap 3
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 3.26f, 0.7f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, -0.2f, 0.05f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.5, 0.5f, 0.5f));
        /*ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.5, 0.5f, 0.5f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/



        

        
        

        //draw fan with rotations
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 3.0f, 7.0f));
        drawFan(VAO, lightingShader, identityMatrix);



        
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        //point lights set up
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        pointlight5.setUpPointLight(lightingShaderWithTexture);
        pointlight6.setUpPointLight(lightingShaderWithTexture);
        pointlight7.setUpPointLight(lightingShaderWithTexture);
        pointlight8.setUpPointLight(lightingShaderWithTexture);
        pointlight9.setUpPointLight(lightingShaderWithTexture);
        pointlight10.setUpPointLight(lightingShaderWithTexture);
        pointlight11.setUpPointLight(lightingShaderWithTexture);
        pointlight12.setUpPointLight(lightingShaderWithTexture);
        pointlight13.setUpPointLight(lightingShaderWithTexture);
        

        //spot light 1
        spotlight1.setUpspotLight(lightingShaderWithTexture);

        //directional light set up
        lightingShaderWithTexture.setVec3("directionalLight.direction", 0.0f, -24.0f, 0.0f);
        lightingShaderWithTexture.setVec3("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
        lightingShaderWithTexture.setVec3("directionalLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShaderWithTexture.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShaderWithTexture.setBool("directionLightOn", directionLightOn);

        //spot light set up
        lightingShaderWithTexture.setVec3("spotLight.position", 20.1f, 7.0f, 0.1f);
        lightingShaderWithTexture.setVec3("spotLight.direction", 5.0f, 5.0f, 5.0f);
        lightingShaderWithTexture.setVec3("spotLight.ambient", 0.5f, 0.5f, 0.5f);
        lightingShaderWithTexture.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShaderWithTexture.setVec3("spotLight.specular", 0.2f, 0.2f, 0.2f);
        lightingShaderWithTexture.setFloat("spotLight.k_c", 1.0f);
        lightingShaderWithTexture.setFloat("spotLight.k_l", 0.09);
        lightingShaderWithTexture.setFloat("spotLight.k_q", 0.032);
        //lightingShaderWithTexture.setFloat("spotLight.cos_theta", glm::cos(glm::radians(30.0f)));
        lightingShaderWithTexture.setFloat("spotLight.inner_circle", glm::cos(glm::radians(12.5f)));
        lightingShaderWithTexture.setFloat("spotLight.outer_circle", glm::cos(glm::radians(25.0f)));
        lightingShaderWithTexture.setBool("spotLightOn", spotLightOn);

        
        //floor
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.0f, -0.1f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(20.5f, 0.1f, 10.0f));
        model = translateMatrix * scaleMatrix;
        //lightEffect(VAO, lightingShader, model, glm::vec3(0.85, 0.65f, 0.56f));
        cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        


        //sky
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-500.1f, 30.0f, -500.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1000.0f, 0.1f, 1000.0f));
        model = translateMatrix * scaleMatrix;
        //lightEffect(VAO, lightingShader, model, glm::vec3(0.85, 0.65f, 0.56f));
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);


        


        //television
        translateMatrix = glm::translate(identityMatrix, glm::vec3(24.5, 5.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.5f, 2.5f, 0.1f));
        model = translateMatrix * scaleMatrix;
        //lightEffect(VAO, lightingShader, model, glm::vec3(0.85, 0.65f, 0.56f));
        vtv.drawCubeWithTexture(lightingShaderWithTexture, model);


        //paintings
        translateMatrix = glm::translate(identityMatrix, glm::vec3(13, 5.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.5f, 4.5f, 0.1f));
        model = translateMatrix * scaleMatrix;
        //lightEffect(VAO, lightingShader, model, glm::vec3(0.85, 0.65f, 0.56f));
        painting.drawCubeWithTexture(lightingShaderWithTexture, model);

        


        //bathtub using bezier curve
        translateMatrix = glm::translate(identityMatrix, glm::vec3(11.5f, 2.0f, 9.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.8, 2.0, 1.2));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", 32.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffMap2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specMap2);
        glBindVertexArray(sphereVAO);
        lightingShaderWithTexture.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, (unsigned int)indicesSphere.size(), GL_UNSIGNED_INT, (void*)0);


        //comode pan using bezier curve
        translateMatrix = glm::translate(identityMatrix, glm::vec3(15.5f+ 1.0f, 2.0f, 8.0f-0.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 1.0, 1.0));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", 32.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffMap2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specMap2);
        glBindVertexArray(sphereVAO);
        lightingShaderWithTexture.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, (unsigned int)indicesSphere.size(), GL_UNSIGNED_INT, (void*)0);

        //comode hole
        translateMatrix = glm::translate(identityMatrix, glm::vec3(17.3, 1.0f, 7.7));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.01f, 2.0f));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(0.494f, 0.514f, 0.541f);
        Cylinder_base c4 = Cylinder_base(0.05f, 36, 10, color, color, color, 32.0f);
        c4.drawSphereTexture(ourShader, model, steel, steel);

        //comode base
        translateMatrix = glm::translate(identityMatrix, glm::vec3(17.3, 0.1f, 7.7));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 1.0f, 2.0f));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(0.494f, 0.514f, 0.541f);
        Cylinder_base c5 = Cylinder_base(0.05f, 36, 10, color, color, color, 32.0f);
        c5.drawSphereTexture(ourShader, model, steel, steel);

        //comode flash
        translateMatrix = glm::translate(identityMatrix, glm::vec3(16.0f+0.5f, 2.0f, 9.0f - 0.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.4f, 1.0f, 0.5f));
        model = translateMatrix * scaleMatrix;
        flash.drawCubeWithTexture(lightingShaderWithTexture, model);

        //flash pipe
        ourShader.setInt("material.diffuse", 0);
        ourShader.setInt("material.specular", 1);
        ourShader.setFloat("material.shininess", 32.0f);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(16.7f+ 0.5f, 0.0f, 9.2f - 0.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(0.494f, 0.514f, 0.541f);
        Cylinder c3 = Cylinder(0.05f, 36, 10, color, color, color, 32.0f);
        c3.drawSphereTexture(ourShader, model, steel, steel);

        //flash button
        translateMatrix = glm::translate(identityMatrix, glm::vec3(17.3, 3.01f, 8.7));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.01f, 2.0f));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(0.494f, 0.514f, 0.541f);
        Cylinder_base c8 = Cylinder_base(0.05f, 36, 10, color, color, color, 32.0f);
        c8.drawSphereTexture(ourShader, model, steel, steel);


        //basin using bezier curve
        translateMatrix = glm::translate(identityMatrix, glm::vec3(13.0f, 2.0f, 9.0f - 0.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0, 1.5, 1.0));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotateZMatrix * scaleMatrix;
        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", 32.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffMap2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specMap2);
        glBindVertexArray(basinVAO);
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, (unsigned int)indicesSphere.size(), GL_UNSIGNED_INT, (void*)0);


        //basin hole
        translateMatrix = glm::translate(identityMatrix, glm::vec3(17.3-2.7, 1.5f, 8.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.01f, 2.0f));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(0.494f, 0.514f, 0.541f);
        Cylinder_base c6 = Cylinder_base(0.05f, 36, 10, color, color, color, 32.0f);
        c6.drawSphereTexture(ourShader, model, steel, steel);

        //basin pipe
        translateMatrix = glm::translate(identityMatrix, glm::vec3(17.3 - 2.7, 0.1f, 8.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(0.494f, 0.514f, 0.541f);
        Cylinder_base c7 = Cylinder_base(0.05f, 36, 10, color, color, color, 32.0f);
        c7.drawSphereTexture(ourShader, model, steel, steel);
        

        //draw sofa
        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 0.5f, 0.0f));
        drawSofas(sofa2, lightingShaderWithTexture, translateMatrix);
        
        //oven structure
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 2.2f, 3.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 0.8f, 2.0f));
        model = translateMatrix * scaleMatrix;
        lightEffect(VAO, lightingShader, model, glm::vec3(0.1, 0.1f, 0.1f));
        

        //oven black left
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.81f, 2.25f, 3.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.7f, 1.4f));
        model = translateMatrix * scaleMatrix;
        oven.drawCubeWithTexture(lightingShaderWithTexture, model);

        //oven screen
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.81f, 2.75f, 3.10f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.15f, 0.3f));
        model = translateMatrix * scaleMatrix;
        led.drawCubeWithTexture(lightingShaderWithTexture, model);

        //oven keypad
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.81f, 2.40f, 3.10f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.3f, 0.3f));
        model = translateMatrix * scaleMatrix;
        oven_keypad.drawCubeWithTexture(lightingShaderWithTexture, model);











        // Draw trees with updated translation along X-axis
        for (int i = 0; i < 400; i += 20) {
            glm::mat4 translateMatrix = glm::translate(identityMatrix, glm::vec3(-8.0f+10 + i + translationOffsetX, -1.0f, 15.0f));
            drawTree(trr, twood, lightingShaderWithTexture, translateMatrix);
        }

        //lamp post
        for (int i = 0; i < 200; i += 20) {
            glm::mat4 translateMatrix = glm::translate(identityMatrix, glm::vec3(-8.0f + 10 + i + translationOffsetX, -1.0f, -5.0f));
            drawLamppost(blacktl, blacktl, lightingShaderWithTexture, translateMatrix);
        }

        //generate fractal tree
        for (int i = 0; i < 400; i += 20) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-8.0f + i + translationOffsetX, -2.0f, 15.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
            model = translateMatrix * scaleMatrix;
            lightingShader.setMat4("model", model);
            glBindVertexArray(fractalVAO);
            glDrawElements(GL_LINES, treeIndices.size(), GL_UNSIGNED_INT, 0);
        }

        // Draw area with updated translation along X-axis
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-200.1f + translationOffsetX, -2.0f, -100.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(500.0f, 0.01f, 200.0f));
        model = translateMatrix * scaleMatrix;
        cube1.drawCubeWithTexture(lightingShaderWithTexture, model);

        // Draw main road with updated translation along X-axis
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-200.0f + translationOffsetX, -2.0f, -2.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(500.0f, 0.1f, 15.0f));
        model = translateMatrix * scaleMatrix;
        road_m.drawCubeWithTexture(lightingShaderWithTexture, model);








        if (watr_tap) {
            if (tap < -4.0) {
                tap = -4.0;
            }
            tap -= 0.1;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 3.2f, 0.7f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05, 0.5 * tap, 0.05f));
            tapwater(water, lightingShaderWithTexture, translateMatrix * scaleMatrix);
            //0.05f, -0.5f, 0.05f
        }
        else {
            if (tap > 1.0) {
                tap = 1.0;
            }
            tap += .1;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 3.2f, 0.7f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05, 0.05, 0.05f));
            tapwater(water, lightingShaderWithTexture, translateMatrix * scaleMatrix);
        }







        //washing machine
        translateMatrix = glm::translate(identityMatrix, glm::vec3(18.0, 0.0, 0.0));
        washingmachine(blacktl, toys2, lightingShaderWithTexture, translateMatrix);


        //electric stove
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 2.21f, 0.05f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.4f, 0.01f, 0.7f));
        model = translateMatrix * scaleMatrix;
        stove.drawCubeWithTexture(lightingShaderWithTexture, model);


        //bathroom floor
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 0.0f, 5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(9.0f, 0.2f, 5.0f));
        model = translateMatrix * scaleMatrix;
        tile.drawCubeWithTexture(lightingShaderWithTexture, model);

        //bathroom wall1 inner
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 0.0f, 5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.5f, 5.0f));
        model = translateMatrix * scaleMatrix;
        tile.drawCubeWithTexture(lightingShaderWithTexture, model);

        //bathroom wall1 outer
        translateMatrix = glm::translate(identityMatrix, glm::vec3(9.99f, 0.0f, 5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 6.5f, 5.0f));
        model = translateMatrix * scaleMatrix;
        wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //bathroom wall2 inner
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f + 9.0f, 0.0f, 5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.5f, 5.0f));
        model = translateMatrix * scaleMatrix;
        tile.drawCubeWithTexture(lightingShaderWithTexture, model);

        //bathroom wall2 outer
        translateMatrix = glm::translate(identityMatrix, glm::vec3(19.2f, 0.0f, 5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 6.5f, 5.0f));
        model = translateMatrix * scaleMatrix;
        wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //bathroom wall3 inner
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 0.0f, 5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.2f, 6.5f, 0.2f));
        model = translateMatrix * scaleMatrix;
        tile.drawCubeWithTexture(lightingShaderWithTexture, model);

        //bathroom wall3 outer
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 0.0f, 5.0f-0.01));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.2f, 6.5f, 0.01f));
        model = translateMatrix * scaleMatrix;
        wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //bathroom door
        
        translateMatrix = glm::translate(identityMatrix, glm::vec3(16.2f, 0.0f, 5.0f - 0.01));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 6.5f, 0.2f));
        // Rotation around Z-axis (adjust based on your door's axis)
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(bathdoor_rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        model =  translateMatrix * rotateYMatrix  * scaleMatrix;
        bathroom_door.drawCubeWithTexture(lightingShaderWithTexture, model);


        


        //bathroom wall4 inner
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 0.0f, 10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(9.2f, 6.5f, 0.1f));
        model = translateMatrix * scaleMatrix;
        tile.drawCubeWithTexture(lightingShaderWithTexture, model);

        //bathroom top
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 6.5f, 5.0f - 0.01));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(9.2f, 0.2f, 5.1f));
        model = translateMatrix * scaleMatrix;
        wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right wall inner
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(31.0f, 10.0f, 0.01f));
        model = translateMatrix * scaleMatrix;
        wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right wall outer
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 10.01f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(31.0f, 10.0f, 0.4f));
        model = translateMatrix * scaleMatrix;
        rvright.drawCubeWithTexture(lightingShaderWithTexture, model);


       
        //left wall outer back
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -0.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(20.5f, 10.0f, 0.4f));
        model = translateMatrix * scaleMatrix;
        rvright.drawCubeWithTexture(lightingShaderWithTexture, model);


        //left wall outer top
        translateMatrix = glm::translate(identityMatrix, glm::vec3(20.5f, 7.2f, -0.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.5f, 2.8f, 0.4f));
        model = translateMatrix * scaleMatrix;
        rvright.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left wall outer front
        translateMatrix = glm::translate(identityMatrix, glm::vec3(24.0f, 0.0f, -0.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(7.0f, 10.0f, 0.4f));
        model = translateMatrix * scaleMatrix;
        rvright.drawCubeWithTexture(lightingShaderWithTexture, model);


        //front wall outer
        translateMatrix = glm::translate(identityMatrix, glm::vec3(30.60f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 10.0f, 10.0f));
        model = translateMatrix * scaleMatrix;
        rvfront.drawCubeWithTexture(lightingShaderWithTexture, model);

        ////front wall inner
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(30.6f, 0.0f, 0.0f));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 10.0f, 10.0f));
        //model = translateMatrix * scaleMatrix;
        //wall.drawCubeWithTexture(lightingShaderWithTexture, model);


        //back wall outer
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 10.0f, 10.5f));
        model = translateMatrix * scaleMatrix;
        rvright.drawCubeWithTexture(lightingShaderWithTexture, model);


        //bus control
        translateMatrix = glm::translate(identityMatrix, glm::vec3(29.0f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.6f, 2.0f, 10.0f));
        model = translateMatrix * scaleMatrix;
        speedometer.drawCubeWithTexture(lightingShaderWithTexture, model);


        // Clamp Bus Door Position to within [20.5, 25.5]
        busDoorPositionX = glm::clamp(busDoorPositionX, 20.5f, 25.5f);

        // Render Bus Door
        translateMatrix = glm::translate(identityMatrix, glm::vec3(busDoorPositionX, 0.0f, -0.55f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.50f, 7.2f, 0.2f));
        model = translateMatrix * scaleMatrix;
        busdoor.drawCubeWithTexture(lightingShaderWithTexture, model);





        ////stair
        //for (int i = 0,j=0; i < 3 && j<3; i++, j++) {
        //    translateMatrix = glm::translate(identityMatrix, glm::vec3(20.5f, -0.5f-i*.5, -1.0f-j*1));
        //    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.6f, 0.5f, 1.0f));
        //    model = translateMatrix * scaleMatrix;
        //    bdfoam.drawCubeWithTexture(lightingShaderWithTexture, model);
        //}
        

        //seat right
        glm::vec3 busPosition = glm::vec3(27, 1, -2);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.78, .05, 11.2) + busPosition);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 0.4f, 1.8));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        bdfoam.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.78, .05, 9.7) + busPosition);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 2.6f, 1.5));
        model = translateMatrix * scaleMatrix;
        bdfoam.drawCubeWithTexture(lightingShaderWithTexture, model);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1, -1.0, 10.5) + busPosition);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(0.494f, 0.514f, 0.541f);
        Cone cone1 = Cone(0.4f, 1.0f, 36, color, color, color, 32.0f);
        cone1.drawConeTexture(lightingShaderWithTexture, model, rvfront_p, rvfront_p);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1, 0.0, 10.5) + busPosition);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        color = glm::vec3(0.494f, 0.514f, 0.541f);
        Cone cone2 = Cone(0.4f, 1.0f, 36, color, color, color, 32.0f);
        cone2.drawConeTexture(lightingShaderWithTexture, model, rvfront_p, rvfront_p);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.15, 2.21, 9.9) + busPosition);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, 0.5, 0.5));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotateZMatrix * rotateYMatrix * scaleMatrix;
        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", 32.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rvfront_p);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, rvfront_p);
        glBindVertexArray(sphereVAO);
        lightingShaderWithTexture.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, (unsigned int)indicesCone.size(), GL_UNSIGNED_INT, (void*)0);



        //seat left
        busPosition = glm::vec3(27, 1, -8);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.78, .05, 11.2) + busPosition);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 0.4f, 1.8));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        bdfoam.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.78, .05, 9.7) + busPosition);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 2.6f, 1.5));
        model = translateMatrix * scaleMatrix;
        bdfoam.drawCubeWithTexture(lightingShaderWithTexture, model);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1, -1.0, 10.5) + busPosition);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(0.494f, 0.514f, 0.541f);
        Cone cone11 = Cone(0.4f, 1.0f, 36, color, color, color, 32.0f);
        cone11.drawConeTexture(lightingShaderWithTexture, model, rvfront_p, rvfront_p);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1, 0.0, 10.5) + busPosition);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        color = glm::vec3(0.494f, 0.514f, 0.541f);
        Cone cone21 = Cone(0.4f, 1.0f, 36, color, color, color, 32.0f);
        cone21.drawConeTexture(lightingShaderWithTexture, model, rvfront_p, rvfront_p);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.15, 2.21, 9.9) + busPosition);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, 4.5, 4.5));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotateZMatrix * rotateYMatrix * scaleMatrix;
        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", 32.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rvfront_p);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, rvfront_p);
        glBindVertexArray(sphereVAO);
        lightingShaderWithTexture.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, (unsigned int)indicesCone.size(), GL_UNSIGNED_INT, (void*)0);



        // right wheel3
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 0.0f, 10.0f));
        drawWheel(tire, ring, lightingShaderWithTexture, translateMatrix, wheelRotationAngle);

        // right wheel2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 0.0f, 10.0f));
        drawWheel(tire, ring, lightingShaderWithTexture, translateMatrix, wheelRotationAngle);

        // right wheel2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(25.0f, 0.0f, 10.0f));
        drawWheel(tire, ring, lightingShaderWithTexture, translateMatrix, wheelRotationAngle);


        // left wheel3
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 0.0f, -0.2f));
        drawWheel(tire, ring, lightingShaderWithTexture, translateMatrix, wheelRotationAngle);

        // left wheel2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 0.0f, -0.2f));
        drawWheel(tire, ring, lightingShaderWithTexture, translateMatrix, wheelRotationAngle);

        // left wheel2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(25.0f, 0.0f, -0.2f));
        drawWheel(tire, ring, lightingShaderWithTexture, translateMatrix, wheelRotationAngle);


        //football
        /*translateMatrix = glm::translate(identityMatrix, glm::vec3(9.0f, 0.45f, 15.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(0.494f, 0.514f, 0.541f);
        Sphere s = Sphere(0.4f, 36, 36, color, color, color, 32.0f);
        s.drawSphereTexture(lightingShaderWithTexture, model, diffMap8, specMap8);*/

        //cone
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 7.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translateMatrix * rotateXMatrix * scaleMatrix;
        color = glm::vec3(0.494f, 0.514f, 0.541f);
        Cone cone = Cone(0.4f, 1.0f, 36, color, color, color, 32.0f);
        cone.drawConeTexture(lightingShaderWithTexture, model, diffMap9, specMap9);

        //draw table
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 2.0f, 5.0f));
        drawTable(table, lightingShaderWithTexture, translateMatrix);

        //draw chair 1
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f, 1.5f, 7.5f));
        drawChair(chair, lightingShaderWithTexture, translateMatrix);

        //draw chair 2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(6.3f, 1.5f, 7.5f));
        drawChair(chair, lightingShaderWithTexture, translateMatrix);

        //draw chair 3
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.7f, 1.5f, 4.5f));
        model =  translateMatrix * rotateYMatrix;
        drawChair(chair, lightingShaderWithTexture, model);

        //draw chair 4
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.4f, 1.5f, 4.5f));
        model = translateMatrix * rotateYMatrix;
        drawChair(chair, lightingShaderWithTexture, model);

        //fridge password
        translateMatrix = glm::translate(identityMatrix, glm::vec3(9.0f, 2.5f, 1.21f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.6f, 0.01f));
        model = translateMatrix * scaleMatrix;
        fridge_keypad.drawCubeWithTexture(lightingShaderWithTexture, model);
        

        //bed
        translateMatrix = glm::translate(identityMatrix, glm::vec3(25, bedYTranslation, 3.0));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.2, 1.0f, 1.2f));
        bed(bdfram, bdbody, bdfoam, blankt, lightingShaderWithTexture, translateMatrix* rotateYMatrix* scaleMatrix);


        ourShader.use();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 13; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}




unsigned int treeSetUp()
{
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (unsigned int)treeVertices.size() * sizeof(float), treeVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)treeIndices.size() * sizeof(float), treeIndices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    //sphereIndicesSize = indices.size();

    return VAO;
}

// Function to generate fractal tree branches with normals in 3D
void generateTree(std::vector<float>& vertices, std::vector<unsigned int>& indices, glm::vec3 start, glm::vec3 direction, float length, int depth, int maxDepth) {
    if (depth == 0 || length < 0.01f) return;

    // Calculate the end point of the current branch
    glm::vec3 end = start + direction * length;
    glm::vec3 normal = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 0.0f, 1.0f)));

    // Add vertices and normals for the branch
    unsigned int startIndex = vertices.size() / 6;
    vertices.push_back(start.x);
    vertices.push_back(start.y);
    vertices.push_back(start.z);
    vertices.push_back(normal.x);
    vertices.push_back(normal.y);
    vertices.push_back(normal.z);

    vertices.push_back(end.x);
    vertices.push_back(end.y);
    vertices.push_back(end.z);
    vertices.push_back(normal.x);
    vertices.push_back(normal.y);
    vertices.push_back(normal.z);

    // Add indices for the branch
    indices.push_back(startIndex);
    indices.push_back(startIndex + 1);

    // Recursive branching (left, right, and up branches)
    float angleOffset = glm::radians(30.0f + (rand() % 10 - 5)); // Randomize angle slightly
    int numBranches = 3; // Three branches: left, right, and up

    for (int i = 0; i < numBranches; ++i) {
        glm::vec3 newDirection = direction; // Start with the current direction

        // Apply rotation based on the branch index
        if (i == 0) {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleOffset, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around x-axis
            newDirection = glm::vec3(rotationMatrix * glm::vec4(direction, 0.0f));
        }
        else if (i == 1) {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleOffset, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around y-axis
            newDirection = glm::vec3(rotationMatrix * glm::vec4(direction, 0.0f));
        }
        else {
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleOffset, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around z-axis
            newDirection = glm::vec3(rotationMatrix * glm::vec4(direction, 0.0f));
        }

        // Random length factor to ensure it doesn't go to zero
        float lengthFactor = 0.6f + static_cast<float>(rand()) / RAND_MAX * 0.2f; // Random length factor
        generateTree(vertices, indices, end, newDirection, length * lengthFactor, depth - 1, maxDepth);
    }
}


void tapwater(Cube& cube, Shader& lightingShader, glm::mat4 als)
{

    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.05f, 0.1f));
   // rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = als * translateMatrix;
    cube.drawCubeWithTexture(lightingShader, model);

}

void bed(Cube& frame, Cube& body, Cube& foam, Cube blank, Shader& lightingShader, glm::mat4 als)
{


    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.5, 1.0f, 3.7f));
    model = als * translateMatrix * scaleMatrix;
    body.drawCubeWithTexture(lightingShader, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 1.0, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3, 2.0f, 3.7f));
    model = als * translateMatrix * scaleMatrix;
    frame.drawCubeWithTexture(lightingShader, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3, 1.0, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.2, 0.8f, 3.7f));
    model = als * translateMatrix * scaleMatrix;
    foam.drawCubeWithTexture(lightingShader, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.4, 1.8, 0.2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.2, 0.25f, 1.5f));
    model = als * translateMatrix * scaleMatrix;
    foam.drawCubeWithTexture(lightingShader, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.4, 1.8, 2.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.2, 0.25f, 1.5f));
    model = als * translateMatrix * scaleMatrix;
    foam.drawCubeWithTexture(lightingShader, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0, 1.8, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 0.1f, 3.7f));
    model = als * translateMatrix * scaleMatrix;
    blank.drawCubeWithTexture(lightingShader, model);

    


}


void washingmachine(Cube& cube, Cube& cube2, Shader& lightingShader, glm::mat4 als)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //translateMatrix = glm::translate(identityMatrix, glm::vec3(- 15.5, 2.2, 12.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5f, 2.8f, 2.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = als * rotateYMatrix * scaleMatrix;
    cube.drawCubeWithTexture(lightingShader, model);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 2.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5f, 2.8f, 0.1f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = als * translateMatrix * rotateYMatrix * scaleMatrix;
    cube2.drawCubeWithTexture(lightingShader, model);


}



void drawFan(unsigned int VAO, Shader ourShader, glm::mat4 matrix) {
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, RotateTranslateMatrix, InvRotateTranslateMatrix;

    if (fanOn) {
        //fan rod
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.15f, 10.0f, 4.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.5f, 0.2f));
        model = matrix * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //fan middle
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.85f, 8.5f, 4.6f));
        RotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(-0.4f, 0.0f, -0.4f));
        InvRotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(0.4f, 0.0f, 0.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, -0.2f, 0.8f));
        model = matrix * translateMatrix * InvRotateTranslateMatrix * rotateYMatrix * RotateTranslateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //fan propelars left
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.85f, 8.5f, 4.8f));
        RotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(-0.4f, 0.0f, -0.2f));
        InvRotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(0.4f, 0.0f, 0.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.5f, -0.2f, 0.4f));
        model = matrix * translateMatrix * InvRotateTranslateMatrix * rotateYMatrix * RotateTranslateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //fan propelars right
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.65f, 8.5f, 4.8f));
        RotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(0.4f, 0.0f, -0.2f));
        InvRotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(-0.4f, 0.0f, 0.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, -0.2f, 0.4f));
        model = matrix * translateMatrix * InvRotateTranslateMatrix * rotateYMatrix * RotateTranslateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //fan propelars up
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.05f, 8.5f, 4.6f));
        RotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, 0.0f, -0.4f));
        InvRotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(0.2f, 0.0f, 0.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, -0.2f, -1.5f));
        model = matrix * translateMatrix * InvRotateTranslateMatrix * rotateYMatrix * RotateTranslateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //fan propelars down
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.05f, 8.5f, 5.4f));
        RotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, 0.0f, 0.4f));
        InvRotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(0.2f, 0.0f, -0.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, -0.2f, 1.5f));
        model = matrix * translateMatrix * InvRotateTranslateMatrix * rotateYMatrix * RotateTranslateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        r += 0.75f;
    }

    else {
        //fan rod
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.15f, 10.0f, 4.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.5f, 0.2f));
        model = matrix * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //fan middle
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.85f, 8.5f, 4.6f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, -0.2f, 0.8f));
        model = matrix * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //fan propelars left
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.85f, 8.5f, 4.8f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.5f, -0.2f, 0.4f));
        model = matrix * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //fan propelars right
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.65f, 8.5f, 4.8f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, -0.2f, 0.4f));
        model = matrix * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //fan propelars up
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.05f, 8.5f, 4.6f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, -0.2f, -1.5f));
        model = matrix * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //fan propelars down
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.05f, 8.5f, 5.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, -0.2f, 1.5f));
        model = matrix * translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}

void drawWheel(unsigned int tire, unsigned int  ring, Shader ourShader, glm::mat4 matrix, float rotationAngle) {
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 rotateZMatrix, rotateXMatrix, scaleMatrix, model;

    // Common rotation along Z-axis for both ring and tire
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    // Ring
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(28.0f, 1.0f, 28.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = matrix * rotateZMatrix * rotateXMatrix * scaleMatrix; // Apply Z-axis rotation to the ring
    glm::vec3 ringColor = glm::vec3(0.0f, 0.514f, 0.0f);
    Cylinder_base ringCylinder(0.05f, 36, 10, ringColor, ringColor, ringColor, 32.0f);
    ringCylinder.drawSphereTexture(ourShader, model, ring, ring);

    // Tire
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(40.0f, 1.0f, 40.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = matrix * rotateZMatrix * rotateXMatrix * scaleMatrix; // Apply Z-axis rotation to the tire
    glm::vec3 tireColor = glm::vec3(0.494f, 0.0f, 0.0f);
    Cylinder_base tireCylinder(0.05f, 36, 10, tireColor, tireColor, tireColor, 32.0f);
    tireCylinder.drawSphereTexture(ourShader, model, tire, tire);
}

void drawLamppost(Cube& ver, Cube& hor, Shader ourShader, glm::mat4 matrix) {
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    
    
    ////vertical
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -2.2f, 0.0f));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 3.0f, 2.0f));
    //model = matrix * translateMatrix * scaleMatrix;
    //glm::vec3 color = glm::vec3(0.494f, 0.514f, 0.541f);
    //Cylinder c = Cylinder(0.05f, 36, 10, color, color, color, 32.0f);
    //c.drawSphereTexture(ourShader, model, twood, twood);


    //vertical
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -2.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 10.2f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    ver.drawCubeWithTexture(ourShader, model);

    //horizontal
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 8.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, .2f, 2.2f));
    model = matrix * translateMatrix * scaleMatrix;
    hor.drawCubeWithTexture(ourShader, model);

}


void drawTree(unsigned int trr, unsigned int twood, Shader ourShader, glm::mat4 matrix) {
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    //tree mid using bezier curve
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.4f, 1.4f, 1.0f));
    //model = matrix * scaleMatrix;
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(-8.0f, 1.5f, 20.0f));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.4f, 1.4f, 1.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = matrix * rotateZMatrix * scaleMatrix;
    ourShader.setInt("material.diffuse", 0);
    ourShader.setInt("material.specular", 1);
    ourShader.setFloat("material.shininess", 32.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, trr);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, trr);
    glBindVertexArray(treeTopVAO);
    ourShader.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, (unsigned int)indicesTreeTop.size(), GL_UNSIGNED_INT, (void*)0);

    //tree wood
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -2.2f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 3.0f, 2.0f));
    model = matrix * translateMatrix * scaleMatrix;
    glm::vec3 color = glm::vec3(0.494f, 0.514f, 0.541f);
    Cylinder c = Cylinder(0.05f, 36, 10, color, color, color, 32.0f);
    c.drawSphereTexture(ourShader, model, twood, twood);

}






void drawChair(Cube& chair, Shader& ourShader, glm::mat4 matrix) {
    //chair sit
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.2f, 0.2f, 1.2f));
    model = matrix * scaleMatrix;
    chair.drawCubeWithTexture(ourShader, model);

    //chair left back leg
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.5f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    chair.drawCubeWithTexture(ourShader, model);


    //chair right back leg
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.5f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    chair.drawCubeWithTexture(ourShader, model);

    //chair left front leg
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.5f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    chair.drawCubeWithTexture(ourShader, model);


    //chair right front leg
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.5f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    chair.drawCubeWithTexture(ourShader, model);

    //chair left hand
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.8f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    chair.drawCubeWithTexture(ourShader, model);


    //chair right hand
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.8f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    chair.drawCubeWithTexture(ourShader, model);

    //chair top hand
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 1.8f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.2f, 0.2f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    chair.drawCubeWithTexture(ourShader, model);

    //chair mid hand
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.8f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.2f, 0.2f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    chair.drawCubeWithTexture(ourShader, model);

    
}
void drawTable(Cube& table, Shader& ourShader, glm::mat4 matrix) {
    //table top
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 0.2f, 2.0f));
    model = matrix * scaleMatrix;
    table.drawCubeWithTexture(ourShader, model);


    //table front leg left
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 1.8f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    table.drawCubeWithTexture(ourShader, model);

    //table front leg right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.8f, 0.0f, 1.8f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    table.drawCubeWithTexture(ourShader, model);

    //table back leg left
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    table.drawCubeWithTexture(ourShader, model);

    //table back leg right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.8f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -2.0f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    table.drawCubeWithTexture(ourShader, model);

 
}

void drawSofas(Cube& cube2, Shader& ourShader, glm::mat4 matrix) {
    //sofa down
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 0.5f, 2.0f));
    model = matrix * scaleMatrix;
    cube2.drawCubeWithTexture(ourShader, model);


    //sofa back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 1.0f, 0.5f));
    model = matrix * translateMatrix * scaleMatrix;
    cube2.drawCubeWithTexture(ourShader, model);

    //sofa hand 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.4f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 1.0f, 2.0f));
    model = matrix * translateMatrix * scaleMatrix;
    cube2.drawCubeWithTexture(ourShader, model);

    //sofa hand 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, 1.0f, 2.0f));
    model = matrix * translateMatrix * scaleMatrix;
    cube2.drawCubeWithTexture(ourShader, model);

    //sofa back leg 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.3f, -0.5f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    cube2.drawCubeWithTexture(ourShader, model);

    //sofa back leg 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.1f, -0.5f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    cube2.drawCubeWithTexture(ourShader, model);

    //sofa front leg 1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.3f, -0.5f, 1.7f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    cube2.drawCubeWithTexture(ourShader, model);

    //sofa front leg 2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.1f, -0.5f, 1.7f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.5f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    cube2.drawCubeWithTexture(ourShader, model);

    


}

void drawCylinder(unsigned int& VAO_C, Shader& lightingShader, glm::vec3 color, glm::mat4 model, std::vector<unsigned int>& indices)
{
    lightingShader.use();

    // setting up materialistic property
    lightingShader.setVec3("material.ambient", color);
    lightingShader.setVec3("material.diffuse", color);
    lightingShader.setVec3("material.specular", color);
    lightingShader.setFloat("material.shininess", 32.0f);
    float emissiveIntensity = 0.05f; // Adjust this value to increase or decrease the intensity
    glm::vec3 emissiveColor = glm::vec3(1.0f, 1.0f, 0.0f) * emissiveIntensity;

    //lightingShader.setVec3("material.emissive", emissiveColor);


    lightingShader.setMat4("model", model);

    glBindVertexArray(VAO_C);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void drawSphere(unsigned int& VAO_S, Shader& lightingShader, glm::vec3 color, glm::mat4 model, std::vector<unsigned int>& indices)
{
    lightingShader.use();

    // Setting up materialistic property
    lightingShader.setVec3("material.ambient", color);
    lightingShader.setVec3("material.diffuse", color);
    lightingShader.setVec3("material.specular", color);
    lightingShader.setFloat("material.shininess", 32.0f);
    float emissiveIntensity = 0.05f; // Adjust this value to increase or decrease the intensity
    glm::vec3 emissiveColor = glm::vec3(1.0f, 0.0f, 0.0f) * emissiveIntensity;

    // lightingShader.setVec3("material.emissive", emissiveColor);

    lightingShader.setMat4("model", model);

    glBindVertexArray(VAO_S);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void generateCylinder(float radius, float height, int segments, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    float angleStep = 2.0f * M_PI / segments;

    // Generate vertices and normals
    for (int i = 0; i <= segments; ++i) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Bottom circle vertex
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(z);

        // Bottom circle normal (pointing down the y-axis)
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);
        vertices.push_back(0.0f);

        // Top circle vertex
        vertices.push_back(x);
        vertices.push_back(height);
        vertices.push_back(z);

        // Top circle normal (pointing up the y-axis)
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);

        // Side vertex (bottom)
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(z);

        // Side normal
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(z);

        // Side vertex (top)
        vertices.push_back(x);
        vertices.push_back(height);
        vertices.push_back(z);

        // Side normal
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(z);
    }

    // Generate indices for the sides
    for (int i = 0; i < segments; ++i) {
        int bottom1 = 4 * i + 2;
        int top1 = bottom1 + 1;
        int bottom2 = 4 * (i + 1) + 2;
        int top2 = bottom2 + 1;

        // Side triangles
        indices.push_back(bottom1);
        indices.push_back(top1);
        indices.push_back(bottom2);

        indices.push_back(top1);
        indices.push_back(top2);
        indices.push_back(bottom2);
    }

    // Center vertex for the top circle
    int topCenterIndex = vertices.size() / 6;
    vertices.push_back(0.0f);
    vertices.push_back(height);
    vertices.push_back(0.0f);

    // Top center normal (pointing up the y-axis)
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    // Generate indices for the top circle
    for (int i = 0; i < segments; ++i) {
        int top1 = 4 * i + 1;
        int top2 = 4 * ((i + 1) % segments) + 1;

        indices.push_back(topCenterIndex);
        indices.push_back(top1);
        indices.push_back(top2);
    }

    // Center vertex for the bottom circle
    int bottomCenterIndex = vertices.size() / 6;
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // Bottom center normal (pointing down the y-axis)
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    // Generate indices for the bottom circle
    for (int i = 0; i < segments; ++i) {
        int bottom1 = 4 * i;
        int bottom2 = 4 * ((i + 1) % segments);

        indices.push_back(bottomCenterIndex);
        indices.push_back(bottom2);
        indices.push_back(bottom1);
    }
}




void generateCone(float radius, float height, int segments, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    float angleStep = 2.0f * M_PI / segments;

    // Generate vertices and normals for the base circle
    for (int i = 0; i < segments; ++i) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Base circle vertex
        vertices.push_back(x);
        vertices.push_back(0.0f); // y = 0 for the base
        vertices.push_back(z);

        // Base circle normal (pointing downwards)
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);
        vertices.push_back(0.0f);
    }

    // Add the tip vertex of the cone
    int tipIndex = vertices.size() / 6; // Index of the tip
    vertices.push_back(0.0f);
    vertices.push_back(height); // Tip is at y = height
    vertices.push_back(0.0f);

    // Tip normal (pointing upwards)
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    // Add the center vertex of the base circle
    int baseCenterIndex = vertices.size() / 6; // Index of the base center
    vertices.push_back(0.0f);
    vertices.push_back(0.0f); // Center is at y = 0
    vertices.push_back(0.0f);

    // Base center normal (pointing downwards)
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    // Generate indices and normals for the sides
    for (int i = 0; i < segments; ++i) {
        int nextIndex = (i + 1) % segments;

        // Side triangle
        indices.push_back(i);
        indices.push_back(nextIndex);
        indices.push_back(tipIndex);

        // Calculate normals for the side vertices
        glm::vec3 v1(vertices[6 * i], vertices[6 * i + 1], vertices[6 * i + 2]);
        glm::vec3 v2(vertices[6 * nextIndex], vertices[6 * nextIndex + 1], vertices[6 * nextIndex + 2]);
        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, glm::vec3(0.0f, height, 0.0f) - v1));

        // Add normals for the side vertices
        vertices.push_back(v1.x);
        vertices.push_back(v1.y);
        vertices.push_back(v1.z);
        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);

        vertices.push_back(v2.x);
        vertices.push_back(v2.y);
        vertices.push_back(v2.z);
        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);
    }

    // Generate indices for the base circle
    for (int i = 0; i < segments; ++i) {
        int nextIndex = (i + 1) % segments;

        indices.push_back(baseCenterIndex);
        indices.push_back(i);
        indices.push_back(nextIndex);
    }
}




void generateSphere(float radius, int sectorCount, int stackCount, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal, lenginv is the inverse of the radius


    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }

    // generate indices
    int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}



void drawFurnace(unsigned int VAO, Shader ourShader, glm::mat4 matrix) {
    
    //z1
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 0.01f, 0.3f));
    model = matrix * scaleMatrix;
    lightEffect(VAO, ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
    /*ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


    //z2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3f, 0.0f,0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 0.01f, 0.3f));
    model = matrix * translateMatrix * scaleMatrix;
    lightEffect(VAO, ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
    /*ourShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

    //x1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.01f, 0.02f));
    model = matrix * translateMatrix * scaleMatrix;
    lightEffect(VAO, ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
    /*ourShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

    //x2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.35f, 0.01f, 0.02f));
    model = matrix * translateMatrix * scaleMatrix;
    lightEffect(VAO, ourShader, model, glm::vec3(1.0f, 1.0f, 1.0f));
    /*ourShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

    


}


void lightEffect(unsigned int VAO, Shader lightShader, glm::mat4 model, glm::vec3 color)
{
    lightShader.use();
    lightShader.setVec3("material.ambient", color);
    lightShader.setVec3("material.diffuse", color);
    lightShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightShader.setFloat("material.shininess", 32.0f);

    lightShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        if (fanOn) {
            fanOn = false;
        }
        else {
            fanOn = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        birdEyeView = !birdEyeView;
    }

    if (birdEyeView) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            birdEyePosition.z -= birdEyeSpeed * deltaTime; // Move forward along Z
            birdEyeTarget.z -= birdEyeSpeed * deltaTime;
            if (birdEyePosition.z <= 4.0) {
                birdEyePosition.z = 4.0;
            }
            if (birdEyeTarget.z <= 0.0) {
                birdEyeTarget.z = 0.0;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            birdEyePosition.z += birdEyeSpeed * deltaTime; // Move backward along Z
            birdEyeTarget.z += birdEyeSpeed * deltaTime;
            if (birdEyePosition.z >= 10.0) {
                birdEyePosition.z = 10.0;
            }
            if (birdEyeTarget.z >= 6.0) {
                birdEyeTarget.z = 6.0;
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if (!birdEyeView) {
            theta += 0.01f; // Increment angle to rotate
            // Calculate new eye position using spherical coordinates
            camera.Position.x = lookAtX + radius * sin(theta);
            camera.Position.y = lookAtY;
            camera.Position.z = lookAtZ + radius * cos(theta);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        directionLightOn = !directionLightOn;
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        spotLightOn = !spotLightOn;
    }

    
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        if (point1)
        {
            point1 = false;
            point3 = false;
            point4 = false;
            point5 = false;
            point6 = false;
            point7 = false;
            point8 = false;
            point9 = false;
            point10 = false;
            point11 = false;
            point12 = false;
            point13 = false;
            pointlight1.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointlight5.turnOff();
            pointlight6.turnOff();
            pointlight7.turnOff();
            pointlight8.turnOff();
            pointlight9.turnOff();
            pointlight10.turnOff();
            pointlight11.turnOff();
            pointlight12.turnOff();
            pointlight13.turnOff();
        }
        else
        {
            point1 = true;
            pointlight1.turnOn();
            point3 = true;
            pointlight3.turnOn();
            point4 = true;
            pointlight4.turnOn();
            point5 = true;
            pointlight5.turnOn();
            point6 = true;
            pointlight6.turnOn();
            point7 = true;
            pointlight7.turnOn();
            point8 = true;
            pointlight8.turnOn();
            point9 = true;
            pointlight9.turnOn();
            point10 = true;
            pointlight10.turnOn();
            point11 = true;
            pointlight11.turnOn();
            point12 = true;
            pointlight12.turnOn();
            point13 = true;
            pointlight13.turnOn();
        }

    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        if (point2)
        {
            point2 = false;
            pointlight2.turnOff();
        }
        else
        {
            point2 = true;
            pointlight2.turnOn();
        }

    }
   

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            ambientToggle = false;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            ambientToggle = true;
        }

    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            //d_def_on = 0.0f;

            diffuseToggle = false;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();

            //d_def_on = 1.0f;
            diffuseToggle = true;
        }

    }

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            //d_def_on = 0.0f;

            specularToggle = false;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();

            //d_def_on = 1.0f;
            specularToggle = true;
        }

    }

    //if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
    //    rotateWheel = !rotateWheel; // Toggle rotation state
    //}


    // Handle key press to move objects forward along X-axis (L key)
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !isKeyLPressed) {
        isKeyLPressed = true; // Mark the L key as pressed
        if (!isMovingForward and !rotateWheel and rotationDirection != 1) {
            // Start forward movement
            isMovingForward = true;
            isMovingBackward = false; // Ensure backward movement is stopped
            rotationDirection = 1;    // Forward direction
            rotateWheel = true;
        }
        else {
            // Stop forward movement
            isMovingForward = false;
            rotateWheel = false;         // Stop rotating the wheel
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) {
        isKeyLPressed = false; // Reset when the L key is released
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !isKeyKPressed) {
        isKeyKPressed = true; // Mark the K key as pressed
        if (!isMovingBackward and !rotateWheel and rotationDirection != -1) {
            // Start backward movement
            isMovingBackward = true;
            isMovingForward = false; // Ensure forward movement is stopped
            rotationDirection = -1;  // Backward direction
            rotateWheel = true;          // Start rotating the wheel
        }
        else {
            // Stop backward movement
            isMovingBackward = false;
            rotateWheel = false;         // Stop rotating the wheel
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) {
        isKeyKPressed = false; // Reset when the K key is released
    }



    // Handle key press to toggle forward rotation (M key)
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        bathdoor_isRotatingForward = true;
        bathdoor_isRotatingReverse = false; // Stop reverse rotation if M is pressed

    }

    // Handle key press to toggle reverse rotation (N key)
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        bathdoor_isRotatingReverse = true;
        bathdoor_isRotatingForward = false; // Stop forward rotation if N is pressed
    }

    // Start upward movement when O is pressed
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        bedIsMovingUp = true;
        bedIsMovingDown = false; // Stop downward movement if O is pressed
    }

    // Start downward movement when P is pressed
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        bedIsMovingDown = true;
        bedIsMovingUp = false; // Stop upward movement if P is pressed
    }


    //tap water
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        if (watr_tap) {
            watr_tap = false;
        }
        else {
            watr_tap = true;
        }
    }



    // Handle Forward Movement (H key)
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !isKeyHPressed) {
        isKeyHPressed = true;
        isBusDoorMovingForward = !isBusDoorMovingForward; // Toggle forward movement
        if (isBusDoorMovingForward) {
            isBusDoorMovingBackward = false; // Stop backward movement
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE) {
        isKeyHPressed = false; // Reset when the H key is released
    }

    // Handle Backward Movement (J key)
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && !isKeyJPressed) {
        isKeyJPressed = true;
        isBusDoorMovingBackward = !isBusDoorMovingBackward; // Toggle backward movement
        if (isBusDoorMovingBackward) {
            isBusDoorMovingForward = false; // Stop forward movement
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE) {
        isKeyJPressed = false; // Reset when the J key is released
    }



}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    float aspectRatio = 4.0f / 3.0f;
    int viewWidth, viewHeight;

    if (width / (float)height > aspectRatio) {
        // Window is too wide, fit height and adjust width
        viewHeight = height;
        viewWidth = (int)(height * aspectRatio);
    }
    else {
        // Window is too tall, fit width and adjust height
        viewWidth = width;
        viewHeight = (int)(width / aspectRatio);
    }

    // Center the viewport
    int xOffset = (width - viewWidth) / 2;
    int yOffset = (height - viewHeight) / 2;

    glViewport(xOffset, yOffset, viewWidth, viewHeight);
    //glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void drawSofa(unsigned int VAO, Shader lightingShader, glm::mat4 matrix) {
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, RotateTranslateMatrix, InvRotateTranslateMatrix;
    glm::vec3 color;

    //top
    translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 0.1f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 1.0f, 2.0f));
    model = matrix * translateMatrix * scaleMatrix;
    color = glm::vec3(0.882f, 0.710f, 0.604f);
    drawCube(VAO, lightingShader, model, color);

    //back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.0f, -1.0f, -4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 2.5f, 0.5f));
    model = matrix * translateMatrix * scaleMatrix;
    color = glm::vec3(0.647f, 0.408f, 0.294f);
    drawCube(VAO, lightingShader, model, color);

    //left
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.0f, -1.0f, -6.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.2f, 1.5f, 2.5f));
    model = matrix * translateMatrix * scaleMatrix;
    color = glm::vec3(0.647f, 0.408f, 0.294f);
    drawCube(VAO, lightingShader, model, color);

    //right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, -1.0f, -6.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.5f, 2.5f));
    model = matrix * translateMatrix * scaleMatrix;
    color = glm::vec3(0.647f, 0.408f, 0.294f);
    drawCube(VAO, lightingShader, model, color);

    //top
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, -1.0f, -6.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 1.0f, -3.0f));
    model = matrix * translateMatrix * scaleMatrix;
    color = glm::vec3(0.882f, 0.710f, 0.604f);
    drawCube(VAO, lightingShader, model, color);

    //back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, -1.0f, -6.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 2.5f, -3.0f));
    model = matrix * translateMatrix * scaleMatrix;
    color = glm::vec3(0.647f, 0.408f, 0.294f);
    drawCube(VAO, lightingShader, model, color);

    //left
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, -1.0f, -6.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5f, 1.5f, 0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    color = glm::vec3(0.647f, 0.408f, 0.294f);
    drawCube(VAO, lightingShader, model, color);

    //right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, -1.0f, -9.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5f, 1.5f, -0.2f));
    model = matrix * translateMatrix * scaleMatrix;
    color = glm::vec3(0.647f, 0.408f, 0.294f);
    drawCube(VAO, lightingShader, model, color);
}


void drawCube(unsigned int& VAO, Shader& lightingShader, glm::mat4 model, glm::vec3 color)
{
    //use the shadder
    lightingShader.use();

    //define lighting properties
    lightingShader.setVec3("material.ambient", color);
    lightingShader.setVec3("material.diffuse", color);
    lightingShader.setVec3("material.specular", color);
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector<float>& coordinates, vector<float>& normals, vector<int>& indices, vector<float>& vertices, float div = 1.0)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal

    const float dtheta = (2 * pi / ntheta) / div;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];
    vector<float> textureCoords;

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            // Calculate (x, y, z) coordinates using parametric equations
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = -(x - 0) * lengthInv;
            ny = -(y - y) * lengthInv;
            nz = -(z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // Texture coordinates (u, v)
            float u = float(j) / float(ntheta); // Around the circle
            float v = t;     // Along the curve
            textureCoords.push_back(u);
            textureCoords.push_back(v);

            // Increment theta for next iteration
            theta += dtheta;
        }
    }

    // Generate index list of triangles
    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // Create two triangles for each segment
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    // Store the vertex data (positions, normals, texture coordinates)
    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);

        // Add texture coordinates
        vertices.push_back(textureCoords[i / 3 * 2]);
        vertices.push_back(textureCoords[i / 3 * 2 + 1]);
    }

    // Create VAO, VBO, and EBO for OpenGL rendering
    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // Create VBO to copy vertex data to GPU
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // For vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // Target
        (unsigned int)vertices.size() * sizeof(float), // Size in bytes
        vertices.data(),   // Pointer to vertex data
        GL_STATIC_DRAW);                   // Usage

    // Create EBO to copy index data to GPU
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // For index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // Target
        (unsigned int)indices.size() * sizeof(unsigned int),             // Size in bytes
        indices.data(),               // Pointer to index data
        GL_STATIC_DRAW);                   // Usage

    // Enable vertex attribute arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Set attribute pointers
    int stride = 32;     // Should be 32 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);            // Vertex position
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3)); // Normal
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float))); // Texture Coord

    // Unbind VAO, VBO, and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}

void read_file(string file_name, vector<float>& vec)
{
    ifstream file(file_name);
    float number;

    while (file >> number)
        vec.push_back(number);

    file.close();
}