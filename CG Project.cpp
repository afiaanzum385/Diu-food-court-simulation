/*
 * 2D Food Court Simulation
 * Built for Code::Blocks with GLUT
 *
 * IMPLEMENTED ALGORITHMS:
 * - DDA Line Algorithm
 * - Bresenham's Line Algorithm
 * - Midpoint Circle Algorithm
 *
*/

#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

// ============================================================
// CONSTANTS
// ============================================================
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 650;

// Animation variables (for moving objects)
// Mohona's Cars
float car1X = 150;      // Car 1 position
float car2X = 550;      // Car 2 position

// Shanta's People
float person1X = 300;   // Person 1 position
float person2X = 650;   // Person 2 position

// Afia's Clouds
float cloud1X = 400;    // Cloud 1 position
float cloud2X = 600;    // Cloud 2 position
float cloud3X = 200;    // Cloud 3 position

bool startScreen = true;  // Show start screen or main scene

// ============================================================
// ALGORITHM 1: DDA LINE DRAWING
// ============================================================
void drawDDALine(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;

    // Find how many steps we need
    float steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    if (steps == 0) {
        glVertex2f(x1, y1);
        return;
    }

    // Calculate increment values
    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = x1;
    float y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++) {
        glVertex2f(round(x), round(y));
        x += xInc;
        y += yInc;
    }
    glEnd();
}

// ============================================================
// ALGORITHM 2: BRESENHAM'S LINE ALGORITHM
// ============================================================
void drawBresenhamLine(float x1, float y1, float x2, float y2) {
    int ix1 = round(x1);
    int iy1 = round(y1);
    int ix2 = round(x2);
    int iy2 = round(y2);

    int dx = abs(ix2 - ix1);
    int dy = abs(iy2 - iy1);

    int sx = (ix1 < ix2) ? 1 : -1;
    int sy = (iy1 < iy2) ? 1 : -1;

    int err = dx - dy;
    int x = ix1;
    int y = iy1;

    glBegin(GL_POINTS);
    while (true) {
        glVertex2f(x, y);
        if (x == ix2 && y == iy2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
    glEnd();
}

// ============================================================
// ALGORITHM 3: MIDPOINT CIRCLE ALGORITHM
// ============================================================
void drawMidpointCircle(float cx, float cy, float radius) {
    int r = round(radius);
    int x = 0;
    int y = r;
    int d = 1 - r;

    glBegin(GL_POINTS);
    while (x <= y) {
        // Draw 8 symmetric points of the circle
        glVertex2f(cx + x, cy + y);
        glVertex2f(cx - x, cy + y);
        glVertex2f(cx + x, cy - y);
        glVertex2f(cx - x, cy - y);
        glVertex2f(cx + y, cy + x);
        glVertex2f(cx - y, cy + x);
        glVertex2f(cx + y, cy - x);
        glVertex2f(cx - y, cy - x);

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
    glEnd();
}

// ============================================================
// FILLED CIRCLE (using the Midpoint Circle Algorithm)
// ============================================================
void drawFilledCircle(float cx, float cy, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    int rInt = round(radius);

    // Draw horizontal lines to fill the circle
    for (int y = -rInt; y <= rInt; y++) {
        int xLimit = round(sqrt(rInt * rInt - y * y));
        glBegin(GL_POINTS);
        for (int x = -xLimit; x <= xLimit; x++) {
            glVertex2f(cx + x, cy + y);
        }
        glEnd();
    }
}

// Simple wrapper for drawing filled circle
void drawCircle(float cx, float cy, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    drawFilledCircle(cx, cy, radius, r, g, b);
}

// ============================================================
// DRAWING RECTANGLES (Simple and Easy)
// ============================================================

// Draw a filled rectangle
void drawRect(float x, float y, float w, float h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

// Draw rectangle border using Bresenham's algorithm
void drawRectBorder(float x, float y, float w, float h, float r, float g, float b) {
    glColor3f(r, g, b);
    glLineWidth(2.0f);
    drawBresenhamLine(x, y, x + w, y);
    drawBresenhamLine(x + w, y, x + w, y + h);
    drawBresenhamLine(x + w, y + h, x, y + h);
    drawBresenhamLine(x, y + h, x, y);
}

// ============================================================
// TEXT DRAWING FUNCTIONS (Simple)
// ============================================================

void drawSmallText(float x, float y, const char* text, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void drawMediumText(float x, float y, const char* text, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void drawLargeText(float x, float y, const char* text, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}

// ============================================================
// ============================================================
// MOHONA'S FUNCTIONS (Serial 1): Cars and Buildings
// ============================================================
// ============================================================

// Draw a car (Mohona's animation)
void drawCar(float x, float y, float r, float g, float b) {
    // Car body
    drawRect(x, y + 8, 60, 18, r, g, b);
    drawRectBorder(x, y + 8, 60, 18, 0.2f, 0.2f, 0.2f);

    // Car roof
    drawRect(x + 12, y + 20, 36, 12, r * 0.8f, g * 0.8f, b * 0.8f);
    drawRectBorder(x + 12, y + 20, 36, 12, 0.2f, 0.2f, 0.2f);

    // Windows
    drawRect(x + 14, y + 22, 14, 8, 0.6f, 0.8f, 1.0f);
    drawRect(x + 32, y + 22, 14, 8, 0.6f, 0.8f, 1.0f);

    // Headlights
    drawCircle(x + 4, y + 15, 3, 1.0f, 1.0f, 0.5f);
    drawCircle(x + 56, y + 15, 3, 1.0f, 0.2f, 0.2f);

    // Wheels
    drawCircle(x + 12, y + 8, 8, 0.1f, 0.1f, 0.1f);
    drawCircle(x + 48, y + 8, 8, 0.1f, 0.1f, 0.1f);
    // Wheel rims
    drawCircle(x + 12, y + 8, 4, 0.5f, 0.5f, 0.5f);
    drawCircle(x + 48, y + 8, 4, 0.5f, 0.5f, 0.5f);
}

// Draw a tall building (Mohona)
void drawBuilding(float x, float y, float w, float h, float r, float g, float b, int floors) {
    // Main building
    drawRect(x, y, w, h, r, g, b);
    drawRectBorder(x, y, w, h, 0.1f, 0.1f, 0.1f);

    // Windows
    float windowW = 12;
    float windowH = 16;
    float gap = 8;
    float floorHeight = h / floors;

    for (int floor = 0; floor < floors; floor++) {
        float floorY = y + floor * floorHeight + 12;
        for (int windowCol = 0; windowCol < 4; windowCol++) {
            float windowX = x + 15 + windowCol * (windowW + gap);
            if (windowX + windowW < x + w - 15) {
                drawRect(windowX, floorY, windowW, windowH, 0.6f, 0.8f, 1.0f);
                drawRectBorder(windowX, floorY, windowW, windowH, 0.2f, 0.2f, 0.3f);
                // Window reflection
                drawCircle(windowX + 3, floorY + windowH - 4, 2, 1.0f, 1.0f, 1.0f);
            }
        }
    }
}

// Draw a short building (Mohona)
void drawShortBuilding(float x, float y, float w, float h, float r, float g, float b, const char* name) {
    drawRect(x, y, w, h, r, g, b);
    drawRectBorder(x, y, w, h, 0.1f, 0.1f, 0.1f);

    float windowW = 12;
    float windowH = 14;
    float gap = 6;
    float rows = 2;
    float floorHeight = h / rows;

    for (int row = 0; row < rows; row++) {
        float rowY = y + row * floorHeight + 10;
        for (int col = 0; col < 3; col++) {
            float windowX = x + 12 + col * (windowW + gap);
            if (windowX + windowW < x + w - 12) {
                drawRect(windowX, rowY, windowW, windowH, 0.6f, 0.8f, 1.0f);
                drawRectBorder(windowX, rowY, windowW, windowH, 0.2f, 0.2f, 0.3f);
            }
        }
    }

    if (name) {
        drawSmallText(x + w/2 - 15, y + h - 22, name, 1.0f, 1.0f, 1.0f);
    }
}

// Draw a garage building (Mohona)
void drawGarageBuilding(float x, float y, float w, float h, float r, float g, float b) {
    drawRect(x, y, w, h, r, g, b);
    drawRectBorder(x, y, w, h, 0.1f, 0.1f, 0.1f);

    // Garage door
    float doorX = x + 4;
    float doorY = y + 3;
    float doorW = w - 8;
    float doorH = h - 18;
    drawRect(doorX, doorY, doorW, doorH, 0.25f, 0.25f, 0.25f);
    drawRectBorder(doorX, doorY, doorW, doorH, 0.1f, 0.1f, 0.1f);

    // Garage door panels
    glColor3f(0.2f, 0.2f, 0.2f);
    glLineWidth(1);
    for (float lineY = doorY + 6; lineY < doorY + doorH - 4; lineY += 8) {
        drawDDALine(doorX + 2, lineY, doorX + doorW - 2, lineY);
    }

    // Small windows on top
    float winW = 10;
    float winH = 7;
    drawRect(x + 3, y + h - 12, winW, winH, 0.5f, 0.8f, 1.0f);
    drawRectBorder(x + 3, y + h - 12, winW, winH, 0.2f, 0.2f, 0.3f);
    drawRect(x + w - winW - 3, y + h - 12, winW, winH, 0.5f, 0.8f, 1.0f);
    drawRectBorder(x + w - winW - 3, y + h - 12, winW, winH, 0.2f, 0.2f, 0.3f);
}

// ============================================================
// ============================================================
// SHANTA'S FUNCTIONS (Serial 2): People, Chairs, Tables, Ground
// ============================================================
// ============================================================

// Draw a person (Shanta)
void drawPerson(float x, float y) {
    // Head - using circle
    drawCircle(x, y + 25, 6, 0.9f, 0.7f, 0.5f);

    // Body - using DDA lines
    glColor3f(0.2f, 0.3f, 0.8f);
    glLineWidth(2);
    drawDDALine(x, y + 18, x, y + 5);

    // Arms
    drawDDALine(x, y + 14, x - 8, y + 8);
    drawDDALine(x, y + 14, x + 8, y + 8);

    // Legs
    drawDDALine(x, y + 5, x - 6, y);
    drawDDALine(x, y + 5, x + 6, y);
}

// Draw a walking person (Shanta's animation)
void drawPersonWalking(float x, float y, bool leftDirection) {
    // Head - bigger for visibility
    drawCircle(x, y + 27, 8, 0.9f, 0.7f, 0.5f);

    // Body
    glColor3f(0.2f, 0.3f, 0.8f);
    glLineWidth(2);
    drawDDALine(x, y + 18, x, y + 5);

    // Arms (walking motion)
    if (leftDirection) {
        drawDDALine(x, y + 14, x - 8, y + 10);
        drawDDALine(x, y + 12, x + 8, y + 6);
    } else {
        drawDDALine(x, y + 14, x + 8, y + 10);
        drawDDALine(x, y + 12, x - 8, y + 6);
    }

    // Legs (walking motion)
    if (leftDirection) {
        drawDDALine(x, y + 5, x - 10, y);
        drawDDALine(x, y + 5, x + 4, y - 2);
    } else {
        drawDDALine(x, y + 5, x + 10, y);
        drawDDALine(x, y + 5, x - 4, y - 2);
    }
}

// Draw a chair (Shanta)
void drawChair(float x, float y) {
    drawRect(x, y + 12, 20, 4, 0.4f, 0.2f, 0.1f);      // Seat
    drawRect(x + 16, y + 12, 3, 16, 0.4f, 0.2f, 0.1f);  // Back
    drawRect(x + 2, y, 3, 12, 0.4f, 0.2f, 0.1f);        // Left leg
    drawRect(x + 16, y, 3, 12, 0.4f, 0.2f, 0.1f);       // Right leg
}

// Draw a table (Shanta)
void drawTable(float x, float y) {
    drawRect(x, y + 16, 50, 4, 0.6f, 0.4f, 0.2f);       // Table top
    drawRect(x + 4, y, 3, 16, 0.6f, 0.4f, 0.2f);        // Left leg
    drawRect(x + 43, y, 3, 16, 0.6f, 0.4f, 0.2f);       // Right leg
}

// Draw ground and road (Shanta)
void drawGroundAndRoad() {
    // Sky border line
    glColor3f(0.2f, 0.2f, 0.2f);
    glLineWidth(2);
    drawDDALine(0, 105, WINDOW_WIDTH, 105);

    // Ground (soil color)
    drawRect(0, 0, WINDOW_WIDTH, 105, 0.55f, 0.35f, 0.15f);

    // Road (darker)
    drawRect(0, 0, WINDOW_WIDTH, 65, 0.35f, 0.25f, 0.15f);

    // Road markings (dashed white lines)
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2);
    for (int i = 0; i < WINDOW_WIDTH; i += 40) {
        drawDDALine(i, 32, i + 20, 32);
    }

    // Road edge lines
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.5);
    drawDDALine(0, 8, WINDOW_WIDTH, 8);
    drawDDALine(0, 57, WINDOW_WIDTH, 57);
}

// ============================================================
// ============================================================
// AFIA'S FUNCTIONS (Serial 3): Trees, Sky, Stalls
// ============================================================
// ============================================================

// Draw a small tree (Afia)
void drawSmallTree(float x, float y) {
    drawRect(x - 2, y, 4, 12, 0.5f, 0.3f, 0.1f);        // Trunk
    drawCircle(x, y + 15, 15, 0.1f, 0.7f, 0.1f);         // Leaves
}

// Draw a big tree (Afia)
void drawBigTree(float x, float y) {
    // Trunk
    drawRect(x - 5, y, 10, 110, 0.45f, 0.28f, 0.12f);

    // Leaves - using multiple circles
    float leafData[][3] = {
        {0, 130, 28}, {-20, 118, 24}, {20, 118, 24},
        {-12, 145, 22}, {12, 145, 22}, {0, 160, 20},
        {-28, 138, 20}, {28, 138, 20}
    };

    for (int i = 0; i < 8; i++) {
        if (i % 2 == 0)
            drawCircle(x + leafData[i][0], y + leafData[i][1], leafData[i][2], 0.08f, 0.55f, 0.08f);
        else
            drawCircle(x + leafData[i][0], y + leafData[i][1], leafData[i][2], 0.12f, 0.65f, 0.12f);
    }
}

// Draw tree top (Afia)
void drawTreeTop(float x, float y, float r) {
    for (int i = 0; i < 5; i++) {
        float ox = (i - 2) * 18;
        float oy = (i % 2) * 12;
        if (i % 2 == 0)
            drawCircle(x + ox, y + oy, r, 0.08f, 0.55f, 0.08f);
        else
            drawCircle(x + ox, y + oy, r, 0.12f, 0.65f, 0.12f);
    }
}

// Draw a cloud (Afia's animation) - ORIGINAL VERSION
void drawCloud(float cx, float cy) {
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(cx - 30, cy, 16, 1.0f, 1.0f, 1.0f);
    drawCircle(cx - 12, cy + 8, 18, 1.0f, 1.0f, 1.0f);
    drawCircle(cx + 8, cy + 10, 20, 1.0f, 1.0f, 1.0f);
    drawCircle(cx + 28, cy + 6, 17, 1.0f, 1.0f, 1.0f);
    drawCircle(cx + 44, cy - 2, 14, 1.0f, 1.0f, 1.0f);
}

// Draw the sky background (Afia)
void drawBackground() {
    // Sky
    drawRect(0, 105, WINDOW_WIDTH, WINDOW_HEIGHT - 105, 0.6f, 0.8f, 1.0f);

    // Sun
    drawCircle(200, 560, 35, 1.0f, 0.9f, 0.2f);

    // Sun rays
    glColor3f(1.0f, 0.95f, 0.5f);
    glLineWidth(2);
    for (int i = 0; i < 12; i++) {
        float angle = i * 30.0f * 3.14159f / 180.0f;
        float r1 = 42, r2 = 55;
        float x1 = 200 + r1 * cos(angle);
        float y1 = 560 + r1 * sin(angle);
        float x2 = 200 + r2 * cos(angle);
        float y2 = 560 + r2 * sin(angle);
        drawDDALine(x1, y1, x2, y2);
    }

    // Clouds (Afia's animation)
    drawCloud(cloud1X, 520);
    drawCloud(cloud2X, 560);
    drawCloud(cloud3X, 480);
}

// Draw a food stall (Afia) - FIXED TEXT POSITION
void drawStall(float x, float y, float w, float h, const char* name,
               float r1, float g1, float b1, float r2, float g2, float b2) {
    // Shadow
    drawRect(x + 3, y - 3, w, 4, 0.45f, 0.45f, 0.45f);

    // Main body
    drawRect(x, y, w, h, 0.95f, 0.93f, 0.86f);
    drawRectBorder(x, y, w, h, 0.25f, 0.25f, 0.25f);

    // Sign board
    float signH = 26;
    drawRect(x, y + h - signH, w, signH, r2, g2, b2);
    drawRectBorder(x, y + h - signH, w, signH, 0.2f, 0.2f, 0.2f);

    // Sign board text - CENTERED properly
    // Calculate text length to center it
    int textLen = strlen(name);
    float textWidth = textLen * 7; // Approximate width per character for 12px font
    float textX = x + (w - textWidth) / 2;
    drawSmallText(textX, y + h - signH + 8, name, 1.0f, 1.0f, 1.0f);

    // Canopy
    drawRect(x - 2, y + h - signH - 6, w + 4, 6, r2, g2, b2);

    // Glass window
    drawRect(x + 8, y + 30, w - 16, h - 65, 0.70f, 0.88f, 1.0f);
    drawRectBorder(x + 8, y + 30, w - 16, h - 65, 0.35f, 0.35f, 0.35f);

    // Counter
    drawRect(x + 5, y + 10, w - 10, 18, 0.55f, 0.35f, 0.18f);
    drawRectBorder(x + 5, y + 10, w - 10, 18, 0.2f, 0.2f, 0.2f);
}

// ============================================================
// ANIMATION UPDATE (All members' animations combined)
// ============================================================
void update(int value) {
    // ===== MOHONA'S ANIMATION: Cars =====
    car1X += 4.5;  // Car 1 moves right
    if (car1X > WINDOW_WIDTH + 60) car1X = -60;

    car2X -= 4.0;  // Car 2 moves left
    if (car2X < -60) car2X = WINDOW_WIDTH + 60;

    // ===== SHANTA'S ANIMATION: People =====
    person1X += 1.2;  // Person 1 walks right
    if (person1X > WINDOW_WIDTH + 30) person1X = -30;

    person2X -= 1.2;  // Person 2 walks left
    if (person2X < -30) person2X = WINDOW_WIDTH + 30;

    // ===== AFIA'S ANIMATION: Clouds =====
    cloud1X += 0.4;  // Cloud 1 moves right
    if (cloud1X > WINDOW_WIDTH + 60) cloud1X = -60;

    cloud2X += 0.3;  // Cloud 2 moves right (slower)
    if (cloud2X > WINDOW_WIDTH + 60) cloud2X = -60;

    cloud3X += 0.5;  // Cloud 3 moves right (faster)
    if (cloud3X > WINDOW_WIDTH + 60) cloud3X = -60;

    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

// ============================================================
// MAIN DISPLAY FUNCTION
// ============================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Show start screen
    if (startScreen) {
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawLargeText(180, 520, "2D FOOD COURT SIMULATION", 1, 1, 1);
        drawMediumText(280, 450, "Group Members (Serial Order)", 1, 1, 0);

        drawSmallText(250, 400, "1. Nahida Akter Mohona    ID: 232-15-894 ", 1, 1, 1);
        drawSmallText(250, 375, "2. Sadia Islam Shanta     ID: 232-15-082 ", 1, 1, 1);
        drawSmallText(250, 350, "3. Afia Anzum             ID: 232-15-385 ", 1, 1, 1);

        drawMediumText(220, 200, "Press ENTER to Start", 1, 1, 1);

        glutSwapBuffers();
        return;
    }

    // ============================================================
    // DRAW THE SCENE (Serial Order: Mohona, Shanta, Afia)
    // ============================================================

    // 1. Background (Afia's)
    drawBackground();

    // 2. Ground and Road (Shanta's)
    drawGroundAndRoad();

    // 3. Pavement (Shanta's)
    drawRect(0, 105, WINDOW_WIDTH, 115, 0.75f, 0.75f, 0.75f);

    glColor3f(0.55f, 0.55f, 0.55f);
    glLineWidth(2);
    drawDDALine(0, 220, WINDOW_WIDTH, 220);

    // Pavement tiles
    glColor3f(0.62f, 0.62f, 0.62f);
    glLineWidth(1);
    for (int y = 120; y <= 210; y += 15) {
        drawDDALine(0, y, WINDOW_WIDTH, y);
    }
    for (int x = 0; x <= WINDOW_WIDTH; x += 25) {
        drawDDALine(x, 105, x, 220);
    }

    // 4. DIU Building (Mohona's)
    drawBuilding(20, 190, 120, 370, 0.8f, 0.8f, 0.85f, 7);
    drawLargeText(35, 540, "DIU", 0.0f, 0.2f, 0.6f);

    // 5. Trees in front of DIU (Afia's)
    drawBigTree(18, 100);
    drawBigTree(52, 105);
    drawBigTree(86, 100);

    // 6. Short buildings (Mohona's)
    drawShortBuilding(140, 220, 80, 100, 0.4f, 0.6f, 0.8f, "CSE");
    drawShortBuilding(140, 320, 80, 100, 0.8f, 0.5f, 0.3f, "EEE");

    // 7. Food Court Area (Afia's stalls)
    float fcX = 220, fcY = 205, fcW = 380, fcH = 155;
    drawRect(fcX, fcY, fcW, fcH, 0.95f, 0.92f, 0.85f);
    drawRectBorder(fcX, fcY, fcW, fcH, 0.2f, 0.2f, 0.2f);
    drawMediumText(fcX + 115, fcY + fcH - 25, "FOOD COURT", 0.0f, 0.2f, 0.6f);

    // Stalls (Afia's)
    float stallW = 82, stallH = 115, gap = 10;
    float startX = fcX + 10, startY = fcY + 12;

    drawStall(startX, startY, stallW, stallH, "PIZZA", 0.9f, 0.3f, 0.2f, 0.8f, 0.1f, 0.1f);
    drawStall(startX + stallW + gap, startY, stallW, stallH, "BURGER", 0.9f, 0.6f, 0.1f, 0.8f, 0.5f, 0.0f);
    drawStall(startX + 2 * (stallW + gap), startY, stallW, stallH, "NOODLES", 0.9f, 0.8f, 0.2f, 0.7f, 0.6f, 0.0f);
    drawStall(startX + 3 * (stallW + gap), startY, stallW, stallH, "DRINKS", 0.3f, 0.6f, 0.9f, 0.1f, 0.4f, 0.8f);

    // 8. Tree tops behind garages (Afia's)
    drawTreeTop(675, 290, 34);
    drawTreeTop(725, 287, 33);
    drawTreeTop(775, 292, 34);
    drawTreeTop(825, 286, 33);
    drawTreeTop(880, 289, 34);

    // 9. Garage buildings (Mohona's)
    float rightX = fcX + fcW;
    float smallW = 40, smallH = 75, smallY = fcY + 5;

    drawGarageBuilding(rightX, smallY, smallW, smallH, 0.8f, 0.7f, 0.6f);
    drawGarageBuilding(rightX + smallW + 4, smallY, smallW, smallH, 0.7f, 0.8f, 0.7f);
    drawGarageBuilding(rightX + 2*(smallW + 4), smallY, smallW, smallH, 0.8f, 0.7f, 0.8f);
    drawGarageBuilding(rightX + 3*(smallW + 4), smallY, smallW, smallH, 0.7f, 0.8f, 0.8f);
    drawGarageBuilding(rightX + 4*(smallW + 4), smallY, smallW, smallH, 0.9f, 0.8f, 0.6f);
    drawGarageBuilding(rightX + 5*(smallW + 4), smallY, smallW, smallH, 0.8f, 0.6f, 0.7f);
    drawGarageBuilding(rightX + 6*(smallW + 4), smallY, smallW, smallH, 0.6f, 0.8f, 0.6f);

    // 10. Tables, Chairs and People (Shanta's)
    float tablePositions[4][2] = {{230, 150}, {320, 150}, {410, 150}, {500, 150}};
    for (int i = 0; i < 4; i++) {
        float tx = tablePositions[i][0];
        float ty = tablePositions[i][1];
        drawTable(tx, ty);
        drawChair(tx - 12, ty);
        drawChair(tx + 38, ty);
        drawPerson(tx - 8, ty + 4);
        drawPerson(tx + 42, ty + 4);
    }

    // 11. Roadside trees (Afia's)
    for (int i = 0; i < 61; i++) {
        drawSmallTree(i * 15, 97);
        drawSmallTree(-4 + i * 15, 84);
    }

    // 12. Walking people (Shanta's animation)
    drawPersonWalking(person1X, 60, false);  // Walks right
    drawPersonWalking(person2X, 60, true);   // Walks left

    // 13. Cars (Mohona's animation)
    drawCar(car1X, 38, 0.9f, 0.2f, 0.2f);    // Red car going right
    drawCar(car2X, 10, 0.2f, 0.3f, 0.8f);    // Blue car going left

    glutSwapBuffers();
}

// ============================================================
// KEYBOARD FUNCTIONS
// ============================================================
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 13:  // ENTER key
        startScreen = false;
        glutPostRedisplay();
        break;
    case 27:  // ESC key
        exit(0);
        break;
    }
}

// ============================================================
// RESHAPE FUNCTION
// ============================================================
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ============================================================
// INITIALIZATION
// ============================================================
void init() {
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

// ============================================================
// MAIN FUNCTION (With serial order)
// ============================================================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("2D Food Court Simulation");

    cout << "=============================================\n";
    cout << "       2D Food Court Simulation\n";
    cout << "=============================================\n\n";

    cout << "TEAM MEMBERS:\n";
    cout << "---------------------------------------------\n";
    cout << "1. Nahida Akter Mohona       ID: 232-15-894\n";
    cout << "2. Sadia Islam Shanta        ID: 232-15-082\n";
    cout << "3. Afia Anzum                ID: 232-15-385\n";


    cout << "Controls:\n";
    cout << "ENTER - Start Simulation\n";
    cout << "ESC   - Exit\n\n";

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(30, update, 0);

    glutMainLoop();
    return 0;
}
