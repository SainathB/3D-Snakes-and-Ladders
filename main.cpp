#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SOIL/SOIL.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <unistd.h>
using namespace std;

#include "Game.hpp"

#define ESCAPE 27

int window; 

float rtri = 0.0f;
float rquad = 0.0f;
float stepsz = 0.07f;
// float stepsz = 0.07f;
bool cont = false;
Game game;

void init(int Width, int Height) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

void resize(int Width, int Height) {
  if (Height==0)
    Height=1;

  glViewport(0, 0, Width, Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

GLuint ntexdp[100];
GLuint getNumberTexture(int i) {
  if (ntexdp[i] == 0) {
    char name[128];
    sprintf(name, "img/number%d.jpg", i);
    ntexdp[i] = SOIL_load_OGL_texture(
      name,
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
  }
  return ntexdp[i];
}

void render();
void update(int x=1) {
  if (game.gameend) return;
  if (cont) {
    cont = false;
    game.ra = rand() % 6 + 1;
    game.move();
    render();
  }
  glutTimerFunc(100, update, 0);
}

void render_tile(int j, int i, int tile=-1) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, getNumberTexture(i+j*10+1));
  glBegin(GL_QUADS);
    float tx = (4.5-i)*(1/5.f);
    float ty = (4.5-j)*(1/5.f);
    float ts = (1/10.f);
    switch (tile) {
      case TILE_EMPTY:
        glColor3f(0.78, 0.78, 0.78);
        break;
      case TILE_SNKST:
        glColor3f(1, 0.5, 0.5);
        break;
      case TILE_SNKEN:
        glColor3f(1, 0, 0);
        break;
      case TILE_LADST:
        glColor3f(0.5, 1, 0.5);
        break;
      case TILE_LADEN:
        glColor3f(0, 1, 0);
        break;
    }
    glTexCoord2d(0, 0); glVertex3f(tx-ts, j*stepsz, ty+ts);
    glTexCoord2d(1, 0); glVertex3f(tx+ts, j*stepsz, ty+ts);
    glTexCoord2d(1, 1); glVertex3f(tx+ts, j*stepsz, ty-ts);
    glTexCoord2d(0, 1); glVertex3f(tx-ts, j*stepsz, ty-ts);

    glVertex3f(tx-ts, j*stepsz, ty+ts);
    glVertex3f(tx+ts, j*stepsz, ty+ts);
    glVertex3f(tx+ts, (j-1)*stepsz, ty+ts);
    glVertex3f(tx-ts, (j-1)*stepsz, ty+ts);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,GLUquadricObj *quadric) {
  float vx = x2-x1;
  float vy = y2-y1;
  float vz = z2-z1;

  //handle the degenerate case of z1 == z2 with an approximation
  if(vz == 0)
      vz = .0001;

  float v = sqrt( vx*vx + vy*vy + vz*vz );
  float ax = 57.2957795*acos( vz/v );
  if ( vz < 0.0 )
      ax = -ax;
  float rx = -vy*vz;
  float ry = vx*vz;
  glPushMatrix();

  //draw the cylinder body
  glTranslatef( x1,y1,z1 );
  glRotatef(ax, rx, ry, 0.0);
  gluQuadricOrientation(quadric,GLU_OUTSIDE);
  gluCylinder(quadric, radius, radius, v, 64, 1);

  //draw the first cap
  gluQuadricOrientation(quadric,GLU_INSIDE);
  gluDisk( quadric, 0.0, radius, 64, 1);
  glTranslatef( 0,0,v );

  //draw the second cap
  gluQuadricOrientation(quadric,GLU_OUTSIDE);
  gluDisk( quadric, 0.0, radius, 64, 1);
  glPopMatrix();
}

GLUquadric* cylq;
void render_cyl(int j, int i, int q, int p, bool snake) {
  float tx = (4.5-i)*(1/5.f);
  float ty = (4.5-j)*(1/5.f);
  float sx = (4.5-p)*(1/5.f);
  float sy = (4.5-q)*(1/5.f);
  if (snake) glColor3f(0.02, 0.4, 0.02);
  else glColor3f(0.02, 0.02, 0.4);
  renderCylinder(tx, (j+0.1)*stepsz, ty, sx, (q+0.7)*stepsz, sy, 0.02, cylq);
}

void render_player(int j, int i, int r=0, int g=0, int b=0) {
  float tx = (4.5-i)*(1/5.f);
  float ty = (4.5-j)*(1/5.f);
  float ts = (1/12.f);

  glColor3f(0, 0, 0);
  glBegin(GL_QUADS);
    glVertex3f(tx-ts, 0.1f+j*stepsz, ty-ts);
    glVertex3f(tx+ts, 0.1f+j*stepsz, ty-ts);
    glVertex3f(tx+ts, 0.1f+(j-1)*stepsz, ty-ts);
    glVertex3f(tx-ts, 0.1f+(j-1)*stepsz, ty-ts);

    glVertex3f(tx-ts, 0.1f+j*stepsz, ty+ts);
    glVertex3f(tx+ts, 0.1f+j*stepsz, ty+ts);
    glVertex3f(tx+ts, 0.1f+(j-1)*stepsz, ty+ts);
    glVertex3f(tx-ts, 0.1f+(j-1)*stepsz, ty+ts);

    glVertex3f(tx+ts, 0.1f+j*stepsz, ty-ts);
    glVertex3f(tx+ts, 0.1f+j*stepsz, ty+ts);
    glVertex3f(tx+ts, 0.1f+(j-1)*stepsz, ty+ts);
    glVertex3f(tx+ts, 0.1f+(j-1)*stepsz, ty-ts);

    glVertex3f(tx-ts, 0.1f+j*stepsz, ty-ts);
    glVertex3f(tx-ts, 0.1f+j*stepsz, ty+ts);
    glVertex3f(tx-ts, 0.1f+(j-1)*stepsz, ty+ts);
    glVertex3f(tx-ts, 0.1f+(j-1)*stepsz, ty-ts);

    glColor3f(r, g, b);
    glVertex3f(tx-ts, 0.1f+j*stepsz, ty-ts);
    glVertex3f(tx+ts, 0.1f+j*stepsz, ty-ts);
    glVertex3f(tx+ts, 0.1f+j*stepsz, ty+ts);
    glVertex3f(tx-ts, 0.1f+j*stepsz, ty+ts);

    glVertex3f(tx-ts, 0.1f+(j-1)*stepsz, ty+ts);
    glVertex3f(tx+ts, 0.1f+(j-1)*stepsz, ty+ts);
    glVertex3f(tx+ts, 0.1f+(j-1)*stepsz, ty-ts);
    glVertex3f(tx-ts, 0.1f+(j-1)*stepsz, ty-ts);

  glEnd();
}

void render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  // gluLookAt(2, 0, 0, 0, 0, 0, 1, 0, 0);
  gluLookAt(0.5, 2, 2, 0, 0, 0, -0.5, 2, -2);

  glTranslatef(0,0.0f,0.0f);
  
  pair<int, int> p1 = game.getCoordinates(game.p1);
  glColor3f(0, 0, 1);
  render_tile(p1.first, p1.second);
  pair<int, int> p2 = game.getCoordinates(game.p2);
  glColor3f(1, 1, 0);
  render_tile(p2.first, p2.second);
  for (int i = 0; i < 10; ++i)
    for (int j = 0; j < 10; ++j) {
      render_tile(i, j, game.tiles[i][j]);
    }
  for (std::map<int, int>::iterator it = game.snakes.begin(); it != game.snakes.end(); ++it) {
    pair<int, int> p1 = game.getCoordinates(it->first);
    pair<int, int> p2 = game.getCoordinates(it->second);
    render_cyl(p1.first, p1.second, p2.first, p2.second, true);
  }
  for (std::map<int, int>::iterator it = game.ladders.begin(); it != game.ladders.end(); ++it) {
    pair<int, int> p1 = game.getCoordinates(it->first);
    pair<int, int> p2 = game.getCoordinates(it->second);
    render_cyl(p1.first, p1.second, p2.first, p2.second, false);
  }
  render_player(p1.first, p1.second, 0, 0, 1);
  render_player(p2.first, p2.second, 1, 1, 0);

  glutSwapBuffers();
}

void keyPressed(unsigned char key, int x, int y) {
    usleep(100);

    if (key == ESCAPE) { 
      glutDestroyWindow(window); 
      exit(0);                   
    } else if (key == ' ') {
      cont = true;
    } 
}

int main(int argc, char **argv) {
  srand(time(0));
  glutInit(&argc, argv);  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
  window = glutCreateWindow("3D Snakes and Ladders");  
  glutDisplayFunc(&render);  
  glutFullScreen();
  glutReshapeFunc(&resize);
  glutKeyboardFunc(&keyPressed);
  init(640, 480);
  game.snakes[59] = 24;
  game.snakes[84] = 62;
  game.snakes[76] = 47;
  game.snakes[46] = 17;
  game.ladders[19] = 57;
  game.ladders[73] = 95;
  game.ladders[49] = 68;
  game.ladders[15] = 34;
  game.ladders[7] = 29;
  game.ladders[22]=53;
  game.initSnakesLadders();
  cylq = gluNewQuadric();
  update(1);
  glutMainLoop();
  return 1;
}
