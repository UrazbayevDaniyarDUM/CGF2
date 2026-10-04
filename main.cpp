#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <vector>

const int W = 500, H = 500;
int mainWin, subWin, win2;
bool animating = true;
int lastTime = 0;
float sqAngle = 0, triAngle = 0, t = 0; 

int sqMode = 0;                         
const float SQ[4][3] = { {0,0,0},{1,1,1},{1,0,0},{0,1,0} };
const float BG[5][3] = { {0,0,0},{0.9f,0.8f,0.2f},{0.2f,0.7f,0.3f},{0.6f,0.3f,0.7f},{0.3f,0.3f,0.3f} };
float subBg[3] = { 0.2f, 0.4f, 0.8f };
float objColor[3] = { 1, 0, 0 };             

struct Bubble { float x, y, r, g, b, phase; };  
std::vector<Bubble> bubbles;

void circle(float cx, float cy, float rad) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 64; i++) {
        float a = 2 * 3.14159265f * i / 64;
        glVertex2f(cx + rad * cosf(a), cy + rad * sinf(a));
    }
    glEnd();
}

void drawSquare(float h) {
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        if (sqMode == 0) { float c = (i % 2) ? 1.f : 0.f; glColor3f(c, c, c); }
        else glColor3fv(SQ[sqMode]);
        glVertex2f((i == 0 || i == 3) ? -h : h, (i < 2) ? -h : h);
    }
    glEnd();
}

// ------display
void displayMain() {
    glClearColor(0.85f, 0.85f, 0.85f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION); glLoadIdentity(); glOrtho(0, W, 0, H, -1, 1);
    glMatrixMode(GL_MODELVIEW);  glLoadIdentity();

    glPushMatrix();
    glTranslatef(200, 250, 0);
    glRotatef(sqAngle, 0, 0, 1);          
    drawSquare(60);
    glPopMatrix();

    for (auto& b : bubbles) {
        glColor3f(b.r, b.g, b.b);
        circle(b.x, b.y, 15 * (1 + 0.4f * sinf(t + b.phase)));
    }
    glutSwapBuffers();
}

void displaySub() {
    glClearColor(subBg[0], subBg[1], subBg[2], 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();   
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLE_FAN);                      
    glVertex2f(0, 0);
    for (int i = 0; i <= 64; i++) {
        float a = 2 * 3.14159265f * i / 64;
        glVertex2f(0.7f * cosf(a), 0.45f * sinf(a));
    }
    glEnd();
    glutSwapBuffers();
}

void displayWin2() {
    glClearColor(0.1f, 0.1f, 0.1f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glColor3fv(objColor);

    glPushMatrix();
    glTranslatef(-0.5f, 0, 0);
    glRotatef(triAngle, 0, 0, 1);           
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.3f, -0.2f); glVertex2f(0.3f, -0.2f); glVertex2f(0, 0.4f);
    glEnd();
    glPopMatrix();

    circle(0.5f, 0, 0.25f * (1 + 0.3f * sinf(t)));  
    glutSwapBuffers();
}

//animation
void idle() {
    int now = glutGet(GLUT_ELAPSED_TIME);
    float dt = (now - lastTime) / 1000.f;
    lastTime = now;
    sqAngle += 60 * dt;
    triAngle -= 90 * dt;
    t += 3 * dt;
    glutPostWindowRedisplay(mainWin);
    glutPostWindowRedisplay(win2);
}

void startAnim() {
    animating = true;
    lastTime = glutGet(GLUT_ELAPSED_TIME);
    glutIdleFunc(idle);
}

//menu
void mainMenu(int id) { if (id == 1) { animating = false; glutIdleFunc(NULL); } else startAnim(); }
void squareMenu(int id) { sqMode = id; glutPostWindowRedisplay(mainWin); }
void subMenu(int id) { for (int i = 0; i < 3; i++) subBg[i] = BG[id][i]; glutPostWindowRedisplay(subWin); }

//input
void keyboard(unsigned char k, int, int) {
    float* c = objColor;
    switch (tolower(k)) {
    case 'r': c[0] = 1;    c[1] = 0;    c[2] = 0;    break;
    case 'g': c[0] = 0;    c[1] = 1;    c[2] = 0;    break;
    case 'b': c[0] = 0;    c[1] = 0;    c[2] = 1;    break;
    case 'y': c[0] = 1;    c[1] = 1;    c[2] = 0;    break;
    case 'o': c[0] = 1;    c[1] = 0.5f; c[2] = 0;    break;
    case 'p': c[0] = 0.6f; c[1] = 0.2f; c[2] = 0.8f; break;
    case 'w': c[0] = 1;    c[1] = 1;    c[2] = 1;    break;
    }
    glutPostWindowRedisplay(win2);
}

void mouseMain(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        auto rnd = [] { return rand() / (float)RAND_MAX; };
        bubbles.push_back({ (float)x, (float)(H - y), rnd(), rnd(), rnd(), rnd() * 6.28f });
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);      

    glutInitWindowSize(W, H); glutInitWindowPosition(100, 100);
    mainWin = glutCreateWindow("Main Window");          
    glutDisplayFunc(displayMain);
    glutMouseFunc(mouseMain);

    int sq = glutCreateMenu(squareMenu);                
    glutAddMenuEntry("White", 1); glutAddMenuEntry("Red", 2); glutAddMenuEntry("Green", 3);
    glutCreateMenu(mainMenu);
    glutAddMenuEntry("Stop Animation", 1);
    glutAddMenuEntry("Start Animation", 2);
    glutAddSubMenu("Square Colors", sq);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    subWin = glutCreateSubWindow(mainWin, 330, 10, 160, 160);   
    glutDisplayFunc(displaySub);
    glutCreateMenu(subMenu);                            
    glutAddMenuEntry("Yellow", 1); glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Purple", 3); glutAddMenuEntry("Gray", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutInitWindowSize(400, 400); glutInitWindowPosition(650, 100);
    win2 = glutCreateWindow("window 2");                
    glutDisplayFunc(displayWin2);
    glutKeyboardFunc(keyboard);

    startAnim();                                        
    glutMainLoop();
}