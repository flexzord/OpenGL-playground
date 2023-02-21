//
//  main.cpp
//  opengl2
//
//  Created by Aleksei Nesterov on 8/22/22.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include<iostream>
#include <string>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 lightPos(-1.5f, 0.5f, 2.0f);

bool isOn = false;
bool keyPressed = false;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    // configure global opengl state
    // -----------------------------
    //glEnable(GL_DEPTH_TEST);
    // build and compile our shader zprogram
    // ------------------------------------
    Shader my_shader("v_shader", "f_shader");
    Shader cube_shader("cube_v_shader", "cube_f_shader");
    Shader backpack_shader("backpack_v", "backpack_f");
    Model my_model("backpack/backpack.obj");
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    float vertices[] = {
           // positions          // texture coords
            0.5f,  0.5f, 0.0f,   20.0f, 20.0f, // top right
            0.5f, -0.5f, 0.0f,   20.0f, 0.0f, // bottom right
           -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
           -0.5f,  0.5f, 0.0f,   0.0f, 20.0f  // top left
       };
       unsigned int indices[] = {
           0, 1, 3, // first triangle
           1, 2, 3  // second triangle
       };
    
    float cube_vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    
    
    
    unsigned int VBO, cube_VBO, VAO, cube_VAO, EBO;
    glGenVertexArrays(1,&VAO);
    glGenVertexArrays(1, &cube_VAO);
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &cube_VBO);
    
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(cube_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    
    
    
    unsigned int texture = loadTexture("grass.jpg");
    unsigned int cube_texture = loadTexture("container2.png");
    unsigned int spec_texture = loadTexture("container2_specular.png");
    
    my_shader.use();
    my_shader.setInt("texture_diffuse1", 0);
    cube_shader.use();
    cube_shader.setInt("material.diffuse", 1);
    cube_shader.setInt("material.specular", 2);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // render loop
    // -----------
    
    glm::vec3 cubePositions[] = {
          glm::vec3(  2.0f,  -0.5f,  0.0f),
          glm::vec3(  2.0f,  -0.5f, -1.0f),
          glm::vec3(  2.0f,  -0.5f, -2.0f),
          glm::vec3(  2.0f,  -0.5f, -3.0f),
          glm::vec3(  2.0f,  -0.5f, -4.0f),
          glm::vec3( -2.0f,  -0.5f,  0.0f),
          glm::vec3( -2.0f,  -0.5f, -1.0f),
          glm::vec3( -2.0f,  -0.5f, -2.0f),
          glm::vec3( -2.0f,  -0.5f, -3.0f),
          glm::vec3( -2.0f,  -0.5f, -4.0f),

          glm::vec3(  0.0f, -0.5f, -4.0f),
          glm::vec3(  1.0f, -0.5f, -4.0f),
          glm::vec3( -1.0f, -0.5f, -4.0f),
      };
    
    glm::vec3 pointLightPositions[] = {
           glm::vec3( 0.7f,  0.2f,  2.0f),
           glm::vec3( 2.3f, -3.3f, -4.0f),
           glm::vec3(-4.0f,  2.0f, -12.0f),
           glm::vec3( 0.0f,  0.0f, -3.0f)
       };


    
    while (!glfwWindowShouldClose(window))
    {
        
        float current_frame = static_cast<float>(glfwGetTime());
        deltaTime = current_frame - lastFrame;
        lastFrame = current_frame;
        processInput(window);
        
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        my_shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(200.0f, 200.0f, 0.0f));
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        
        my_shader.setMat4("model", model);
        my_shader.setMat4("view", view);
        my_shader.setMat4("projection", projection);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
       
        
        cube_shader.use();
        cube_shader.setVec3("viewPos", camera.Position);
        cube_shader.setFloat("material.shininess", 32.0f);
        // directional light
        cube_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        cube_shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        cube_shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        cube_shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        cube_shader.setVec3("pointLights[0].position", pointLightPositions[0]);
        cube_shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        cube_shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        cube_shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        cube_shader.setFloat("pointLights[0].constant", 1.0f);
        cube_shader.setFloat("pointLights[0].linear", 0.09f);
        cube_shader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        cube_shader.setVec3("pointLights[1].position", pointLightPositions[1]);
        cube_shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        cube_shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        cube_shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        cube_shader.setFloat("pointLights[1].constant", 1.0f);
        cube_shader.setFloat("pointLights[1].linear", 0.09f);
        cube_shader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        cube_shader.setVec3("pointLights[2].position", pointLightPositions[2]);
        cube_shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        cube_shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        cube_shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        cube_shader.setFloat("pointLights[2].constant", 1.0f);
        cube_shader.setFloat("pointLights[2].linear", 0.09f);
        cube_shader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        cube_shader.setVec3("pointLights[3].position", pointLightPositions[3]);
        cube_shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        cube_shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        cube_shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        cube_shader.setFloat("pointLights[3].constant", 1.0f);
        cube_shader.setFloat("pointLights[3].linear", 0.09f);
        cube_shader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        
        if(isOn==false){
            cube_shader.setVec3("spotLight.position", 0.0f, -20.0f, 0.0f);
            cube_shader.setVec3("spotLight.direction", camera.Front);
            cube_shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            cube_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            cube_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            cube_shader.setFloat("spotLight.constant", 1.0f);
            cube_shader.setFloat("spotLight.linear", 0.09f);
            cube_shader.setFloat("spotLight.quadratic", 0.032f);
            cube_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            cube_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
            cube_shader.setMat4("view", view);
            cube_shader.setMat4("projection", projection);
        }
        if(isOn==true){
            cube_shader.setVec3("spotLight.position", camera.Position);
            cube_shader.setVec3("spotLight.direction", camera.Front);
            cube_shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            cube_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            cube_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            cube_shader.setFloat("spotLight.constant", 1.0f);
            cube_shader.setFloat("spotLight.linear", 0.09f);
            cube_shader.setFloat("spotLight.quadratic", 0.032f);
            cube_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            cube_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
            cube_shader.setMat4("view", view);
            cube_shader.setMat4("projection", projection);
        }
       
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cube_texture);
        
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, spec_texture);
        
        glBindVertexArray(cube_VAO);
        for(int i=0; i<13; ++i){
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            cube_shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        
        backpack_shader.use();
        backpack_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        backpack_shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        backpack_shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        backpack_shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        backpack_shader.setVec3("pointLights[0].position", pointLightPositions[0]);
        backpack_shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        backpack_shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        backpack_shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        backpack_shader.setFloat("pointLights[0].constant", 1.0f);
        backpack_shader.setFloat("pointLights[0].linear", 0.09f);
        backpack_shader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        backpack_shader.setVec3("pointLights[1].position", pointLightPositions[1]);
        backpack_shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        backpack_shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        backpack_shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        backpack_shader.setFloat("pointLights[1].constant", 1.0f);
        backpack_shader.setFloat("pointLights[1].linear", 0.09f);
        backpack_shader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        backpack_shader.setVec3("pointLights[2].position", pointLightPositions[2]);
        backpack_shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        backpack_shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        backpack_shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        backpack_shader.setFloat("pointLights[2].constant", 1.0f);
        backpack_shader.setFloat("pointLights[2].linear", 0.09f);
        backpack_shader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        backpack_shader.setVec3("pointLights[3].position", pointLightPositions[3]);
        backpack_shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        backpack_shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        backpack_shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        backpack_shader.setFloat("pointLights[3].constant", 1.0f);
        backpack_shader.setFloat("pointLights[3].linear", 0.09f);
        backpack_shader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        
        if(isOn==false){
            backpack_shader.setVec3("spotLight.position", 0.0f, -20.0f, 0.0f);
            backpack_shader.setVec3("spotLight.direction", camera.Front);
            backpack_shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            backpack_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            backpack_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            backpack_shader.setFloat("spotLight.constant", 1.0f);
            backpack_shader.setFloat("spotLight.linear", 0.09f);
            backpack_shader.setFloat("spotLight.quadratic", 0.032f);
            backpack_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            backpack_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
            
        }
        if(isOn==true){
            backpack_shader.setVec3("spotLight.position", camera.Position);
            backpack_shader.setVec3("spotLight.direction", camera.Front);
            backpack_shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            backpack_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            backpack_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            backpack_shader.setFloat("spotLight.constant", 1.0f);
            backpack_shader.setFloat("spotLight.linear", 0.09f);
            backpack_shader.setFloat("spotLight.quadratic", 0.032f);
            backpack_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            backpack_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
            
        }
        backpack_shader.setVec3("viewPos", camera.Position);
        backpack_shader.setFloat("material.shininess", 32.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        backpack_shader.setMat4("model", model);
        backpack_shader.setMat4("view", view);
        backpack_shader.setMat4("projection", projection);
        my_model.Draw(backpack_shader);
        
        
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&VAO);
    glDeleteVertexArrays(1,&cube_VAO);
  
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &cube_VBO);
 
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS&& !keyPressed){
        isOn=!isOn;
        keyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
            keyPressed = false;
        }
    
    
        
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
