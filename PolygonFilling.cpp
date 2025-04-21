#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
typedef struct pixel
{
    GLubyte r, g, b;
} pixel;
pixel f_color, b_color;
// Function to perform boundary fill
void boundary_fill(int x, int y)
{
    pixel c;
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &c); // Get the current pixel color
    // If the color is not the background color and not the fill color, fill the color
    if ((c.r != b_color.r || c.g != b_color.g || c.b != b_color.b) &&
        (c.r != f_color.r || c.g != f_color.g || c.b != f_color.b))
    {

        glColor3ub(f_color.r, f_color.g, f_color.b); // Set the fill color
        glBegin(GL_POINTS);
        glVertex2i(x, y); // Draw a pixel at (x, y)
        glEnd();
        glFlush();
        // Recursive calls to fill neighboring pixels
        boundary_fill(x + 1, y); // Right pixel
        boundary_fill(x - 1, y); // Left pixel
        boundary_fill(x, y + 1); // Upper pixel
        boundary_fill(x, y - 1); // Lower pixel
    }
}
// Function to draw a triangle
void draw_triangle(int x, int y)
{
    glBegin(GL_LINE_LOOP); // Draw triangle
    glVertex2f(0, 0);
    glVertex2f(0, 50);
    glVertex2f(100, 50);
    glVertex2f(100, 400);
    glVertex2f(300, 400);
    glVertex2f(300, 350);
    glVertex2f(150, 350);
    glVertex2f(150, 0);
    glEnd();
    glFlush();

    glBegin(GL_LINE_LOOP); // Draw triangle
    glVertex2f(0, 400);
    glVertex2f(50, 400);
    glVertex2f(50, 250);
    glVertex2f(300, 250);
    glVertex2f(300, 0);
    glVertex2f(250, 0);
    glVertex2f(250, 200);
    glVertex2f(0, 200);
    glEnd();
    glFlush();
}
// Function to set up the window and initial configurations
void Init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0); // Set background color (white)
    glClear(GL_COLOR_BUFFER_BIT);     // Clear the buffer
    glColor3f(0.0, 0.0, 0.0);         // Set drawing color (black)
    gluOrtho2D(0, 500, 0, 500);       // Set the coordinate system
}
// Function to handle mouse clicks
void mouse_click(int btn, int state, int x, int y)
{
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Fill the triangle with the selected color
        draw_triangle(x, y);

        // Fill the triangle from a point inside it (e.g., center of the triangle)
        int fill_x = 120; // Approximate center of the triangle
        int fill_y = 50;  // Approximate center of the triangle
        f_color.r = 255;
        boundary_fill(125, 25); // Start filling from the center of the triangle
        f_color.b = 240;
        f_color.g = 22;
        boundary_fill(125, 225);
        f_color.r = 70;
        boundary_fill(125, 375);
        f_color.g = 200;
        f_color.b = 50;
        boundary_fill(25, 225);
        f_color.r = 100;
        f_color.b = 100;
        f_color.g = 100;
        boundary_fill(275, 225);
    }
}
void draw()
{
}
// Main function
int main(int argc, char **argv)
{
    // Set border color (black)
    b_color.r = b_color.g = b_color.b = 0;
    // Set fill color (red)
    f_color.r = 20;
    f_color.g = 0;
    f_color.b = 0;
    glutInit(&argc, argv);                       // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode
    glutInitWindowPosition(0, 0);                // Window position
    glutInitWindowSize(500, 500);                // Window size
    glutCreateWindow("Color Fill Triangle");     // Window title
    Init();                                      // Initialize the drawing environment
    glutDisplayFunc(draw);                       // Draw the triangle when the window is displayed
    glutMouseFunc(mouse_click);                  // Handle mouse clicks
    glutMainLoop();                              // Start the GLUT main loop
    return 0;
}