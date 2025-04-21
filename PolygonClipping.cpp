#include <GL/glut.h>
#include <iostream>
#include <vector>

enum Edge {
    LEFT = 0,
    RIGHT = 1,
    BOTTOM = 2,
    TOP = 3
};

class Point {
public:
    float x, y;
    
    Point(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

class PolygonClipper {
private:
    float xmin, ymin, xmax, ymax;
    std::vector<Point> polygon;
    std::vector<Point> clipped;
    bool clippedFlag;

public:
    PolygonClipper() : 
        xmin(-100), ymin(-100), xmax(100), ymax(100), clippedFlag(false) {}

    void addPoint(float x, float y) {
        polygon.emplace_back(x, y);
    }

    void drawPolygon(const std::vector<Point>& poly) {
        glBegin(GL_LINE_LOOP);
        for (const auto& p : poly) {
            glVertex2f(p.x, p.y);
        }
        glEnd();
    }

    Point intersect(const Point& a, const Point& b, Edge edge) {
        Point p;
        float m = (b.x - a.x != 0) ? (b.y - a.y) / (b.x - a.x) : 0;
        
        switch (edge) {
            case LEFT:
                p.x = xmin;
                p.y = a.y + m * (xmin - a.x);
                break;
            case RIGHT:
                p.x = xmax;
                p.y = a.y + m * (xmax - a.x);
                break;
            case BOTTOM:
                p.y = ymin;
                p.x = (m != 0) ? a.x + (ymin - a.y) / m : a.x;
                break;
            case TOP:
                p.y = ymax;
                p.x = (m != 0) ? a.x + (ymax - a.y) / m : a.x;
                break;
        }
        
        return p;
    }

    bool inside(const Point& p, Edge edge) {
        switch (edge) {
            case LEFT:   return p.x >= xmin;
            case RIGHT:  return p.x <= xmax;
            case BOTTOM: return p.y >= ymin;
            case TOP:    return p.y <= ymax;
        }
        return true;
    }

    void clipEdge(Edge edge) {
        std::vector<Point> input = clipped;
        clipped.clear();
        
        for (size_t i = 0; i < input.size(); i++) {
            Point curr = input[i];
            Point prev = input[(i + input.size() - 1) % input.size()];
            bool currIn = inside(curr, edge);
            bool prevIn = inside(prev, edge);
            
            if (prevIn && currIn) {
                clipped.push_back(curr);
            } else if (prevIn && !currIn) {
                clipped.push_back(intersect(prev, curr, edge));
            } else if (!prevIn && currIn) {
                clipped.push_back(intersect(prev, curr, edge));
                clipped.push_back(curr);
            }
        }
    }

    void suthHodgman() {
        clipped = polygon;
        clipEdge(LEFT);
        clipEdge(RIGHT);
        clipEdge(BOTTOM);
        clipEdge(TOP);
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw clipping window (red outline)
        glColor3f(1, 0, 0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(xmin, ymin);
        glVertex2f(xmax, ymin);
        glVertex2f(xmax, ymax);
        glVertex2f(xmin, ymax);
        glEnd();
        
        if (!clippedFlag) {
            // Draw original polygon (blue outline)
            glColor3f(0, 0, 1);
            drawPolygon(polygon);
        } else {
            // Draw only the clipped polygon (green outline)
            glColor3f(0, 1, 0);
            drawPolygon(clipped);
        }
        
        glFlush();
    }

    void clip() {
        suthHodgman();
        clippedFlag = true;
    }

    bool isClipped() const {
        return clippedFlag;
    }

    void getInput() {
        int n;
        std::cout << "Enter number of vertices of polygon: ";
        std::cin >> n;
        
        for (int i = 0; i < n; i++) {
            float x, y;
            std::cout << "Enter x and y for vertex " << (i + 1) << ": ";
            std::cin >> x >> y;
            addPoint(x, y);
        }
        
        std::cout << "\nPress 'c' in the window to clip the polygon...\n";
    }
};

// Global object (needed for callback functions)
PolygonClipper clipper;

// Callback functions
void display() {
    clipper.display();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'c' || key == 'C') {
        clipper.clip();
        glutPostRedisplay();
    }
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(-320, 320, -240, 240);
}

int main(int argc, char** argv) {
    clipper.getInput();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sutherland–Hodgman Polygon Clipping");
    
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
    return 0;
}
