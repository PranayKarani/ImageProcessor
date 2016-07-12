#include <basic_include>
#include <Effects>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

/* Declarations */

// effects
Brightness brightness;
Tint tint;
int grayScale = -1;
int inverse = -1;
int vibrance = -1;
int saturation = -1;
int desaturation = -1;
int vignette = -1;
float v_radius = 0.25;
float v_radius_r = 0.1;

float *targetValue;
float *targetRate;

bool save = false;


GLint pX = 0, pY = 0, mr = 20;
float z = 1, zr = 0.2f;

void setTitle(string text){
    glfwSetWindowTitle(_window, text.c_str());
}

int main() {

    /* Open window and setup OpenGL stuff */

    basic_initializations(700, 780, "ImageProcessor");
    // register input
    glfwSetKeyCallback(_window, key_callback);


    /* Setup texture */

    // loadfrom disk
    Texture texture(input_path"30.jpg", 0, GL_RGB);
    float n_width, n_height;
    // portrait or landscape?
    if (texture.width > texture.height) {// lanscape
        n_width = 0.75f * (float) texture.width / (float) texture.width;
        n_height = 0.75f * ((float) texture.height / (float) texture.width);
    } else {//portrait
        n_width = 0.75f * (float) texture.width / (float) texture.height;
        n_height = 0.75f * ((float) texture.height / (float) texture.height);
    }

    cout << "n_width: " << n_width << "  n_height: " << n_height << endl;

    /* Setting up vertices for rectangle (2 triangles cleverlt positioned) that will
     * hold our texture, whose coordinates are also provides*/
    GLfloat vertices[] = {
            //0
            +n_width, +n_height, 0.0f, // triangle vertices
            1.0f, 1.0f, // texture vertcies
            //1
            -n_width, +n_height, 0.0f,
            0.0f, 1.0f,
            //2
            -n_width, -n_height, 0.0f,
            0.0f, 0.0f,
            //3
            +n_width, -n_height, 0.0f,
            1.0f, 0.0f
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *) (0 * sizeof(float)));


    /* Setting up indices */
    GLuint indices[] = {
            0, 1, 2,// upper triangle -> |/
            2, 3, 0//  lower triangle -> /|
    };
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* Setting up shaders */

    GLuint SId = loadShaders("v", "f");

    // place from where vertex shader will read our texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void *) (3 * sizeof(GLfloat)));


    while (!glfwWindowShouldClose(_window)) {

        /* !!! MAIN LOOP !!! */

        // clear screen and update viewport
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(pX, pY, WIDTH * z, HEIGHT * z);


        // feed data to shaders
        glUniform2f(glGetUniformLocation(SId, "res"), WIDTH, HEIGHT);
        glUniform1i(glGetUniformLocation(SId, "texture0"), texture.texUnit);
        glUniform1f(glGetUniformLocation(SId, "brightness"), brightness.value);
        glUniform1f(glGetUniformLocation(SId, "tint_r"), tint.color.r);
        glUniform1f(glGetUniformLocation(SId, "tint_g"), tint.color.g);
        glUniform1f(glGetUniformLocation(SId, "tint_b"), tint.color.b);
        glUniform1i(glGetUniformLocation(SId, "grayscale"), grayScale);
        glUniform1i(glGetUniformLocation(SId, "inverse"), inverse);
        glUniform1i(glGetUniformLocation(SId, "vibrance"), vibrance);
        glUniform1i(glGetUniformLocation(SId, "saturation"), saturation);
        glUniform1i(glGetUniformLocation(SId, "desaturation"), desaturation);
        glUniform1i(glGetUniformLocation(SId, "vignette"), vignette);
        glUniform1f(glGetUniformLocation(SId, "v_radius"), v_radius);

        glUniform1i(glGetUniformLocation(SId, "makeInverse"), -1);
        glUseProgram(SId);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // if want to save image...
        if (save) {

            // inverse the image first
            glUniform1i(glGetUniformLocation(SId, "makeInverse"), 1);// :D :D :D
            glUseProgram(SId);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // read that image and ...
            int arraySize = texture.width * texture.height * 3;
            unsigned char buf[arraySize];
            glReadPixels(0, 0, texture.width, texture.height, GL_RGB, GL_UNSIGNED_BYTE, &buf);
            // ...store it somewhere warm :)
            texture.saveImage(buf);

            save = false;

        }

        glfwSwapBuffers(_window);
        glfwPollEvents();// check for input events

    }


    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {

        switch (key) {

            // view port movement + zooming

            case GLFW_KEY_UP:
                pY -= mr;
                break;
            case GLFW_KEY_DOWN:
                pY += mr;
                break;
            case GLFW_KEY_RIGHT:
                pX -= mr;
                break;
            case GLFW_KEY_LEFT:
                pX += mr;
                break;
            case GLFW_KEY_PAGE_UP:
                z += zr;
                break;
            case GLFW_KEY_PAGE_DOWN:
                z -= zr;
                break;

            case GLFW_KEY_S:
                save = true;
                break;


                // effect selection
                // 1. brightness
                // 2. greyscale
                // 3. invert
                // 4. vibrance
                // 5. saturation
                // 6. desaturation
                // 7. vignette

            case GLFW_KEY_1:
                targetValue = &brightness.value;
                targetRate = &brightness.rate;
                setTitle("Brightness");
                break;
            case GLFW_KEY_2:
                if (grayScale > 0) {
                    grayScale = -1;
                } else {
                    grayScale = 1;
                }
                targetValue = targetRate = NULL;
                setTitle("Grayscale");
                break;
            case GLFW_KEY_3:
                if (inverse > 0) {
                    inverse = -1;
                } else {
                    inverse = 1;
                }
                targetValue = targetRate = NULL;
                setTitle("Invert");
                break;
            case GLFW_KEY_4:
                if (vibrance > 0) {
                    vibrance = -1;
                } else {
                    vibrance = 1;
                }
                targetValue = targetRate = NULL;
                setTitle("VIbrance");
                break;
            case GLFW_KEY_5:
                if (saturation > 0) {
                    saturation = -1;
                } else {
                    saturation = 1;
                }
                targetValue = targetRate = NULL;
                setTitle("Saturation");
                break;
            case GLFW_KEY_6:
                if (desaturation > 0) {
                    desaturation = -1;
                } else {
                    desaturation = 1;
                }
                targetValue = targetRate = NULL;
                setTitle("Desaturation");
                break;
            case GLFW_KEY_7:

                vignette = 1;
                targetValue = &v_radius;
                targetRate = &v_radius_r;
                setTitle("Vignette");
                break;



                // special rgb control

            case GLFW_KEY_R:
                targetValue = &tint.color.r;
                targetRate = &tint.rate;
                break;
            case GLFW_KEY_G:
                targetValue = &tint.color.g;
                targetRate = &tint.rate;
                break;
            case GLFW_KEY_B:
                targetValue = &tint.color.b;
                targetRate = &tint.rate;
                break;




                // reset
            case GLFW_KEY_X:
                targetValue = targetRate = NULL;
                brightness.value = 1;
                grayScale = -1;
                inverse = -1;
                vibrance = -1;
                saturation = -1;
                desaturation = -1;
                vignette = -1;
                tint.color.set(1.0f, 1.0f, 1.0f, 1.0f);
                setTitle("reset!");
                break;



                // value control
            case GLFW_KEY_HOME:
                if (targetValue != NULL && targetRate != NULL) {
                    if (*targetValue < 2) {
                        *targetValue += *targetRate;
                        cout << "Value: " << *targetValue << ",  rate: " << *targetRate << endl;
                    }
                }
                break;
            case GLFW_KEY_END:
                if (targetValue != NULL && targetRate != NULL) {
                    if (*targetValue > 0) {
                        *targetValue -= *targetRate;
                        cout << "Value: " << *targetValue << ",  rate: " << *targetRate << endl;
                    }
                }
                break;



            default:
                targetValue = targetRate = NULL;
                setTitle("Default");
                break;
        }

    }

}


