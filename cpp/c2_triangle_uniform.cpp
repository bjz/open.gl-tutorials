// Link statically with GLEW
#define GLEW_STATIC

// Headers
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <iostream>

// Shader sources
const char* vertexSource =
    "#version 150\n"
    "in vec2 position;"
    "void main() {"
    "   gl_Position = vec4( position, 0.0, 1.0 );"
    "}";
const char* fragmentSource =
    "#version 150\n"
    "out vec4 outColor;"
    "uniform vec3 triangleColor;"
    "void main() {"
    "   outColor = vec4( triangleColor, 1.0 );"
    "}";

int main()
{
    sf::Window window( sf::VideoMode( 800, 600, 32 ), "OpenGL", sf::Style::Titlebar | sf::Style::Close );
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers( 1, &vbo );

    float vertices[] = {
        0.0f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexSource, NULL );
    glCompileShader( vertexShader );

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentSource, NULL );
    glCompileShader( fragmentShader );

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader( shaderProgram, vertexShader );
    glAttachShader( shaderProgram, fragmentShader );
    glBindFragDataLocation( shaderProgram, 0, "outColor" );
    glLinkProgram( shaderProgram );
    glUseProgram( shaderProgram );

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation( shaderProgram, "position" );
    glEnableVertexAttribArray( posAttrib );
    glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0 );

    // Get the location of the color uniform
    GLint uniColor = glGetUniformLocation( shaderProgram, "triangleColor" );

    while ( window.IsOpened() )
    {
        sf::Event windowEvent;
        while ( window.GetEvent( windowEvent ) )
        {
            switch ( windowEvent.Type )
            {
            case sf::Event::Closed:
                window.Close();
                break;
            }
        }

        // Set the color of the triangle
        float time = (float)clock() / (float)CLOCKS_PER_SEC;
        glUniform3f( uniColor, ( sin( time * 4.0f ) + 1.0f ) / 2.0f, 0.0f, 0.0f );

        // Clear the screen to black
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Draw a triangle from the 3 vertices
        glDrawArrays( GL_TRIANGLES, 0, 3 );

        // Swap buffers
        window.Display();
    }

    glDeleteProgram( shaderProgram );
    glDeleteShader( fragmentShader );
    glDeleteShader( vertexShader );

    glDeleteBuffers( 1, &vbo );

    glDeleteVertexArrays( 1, &vao );
}