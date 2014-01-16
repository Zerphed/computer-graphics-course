/* The program supports the following controls:
 * --------------------------------------------
 *
 * Left mouse: switch between active light sources
 * Mouse wheel: switch the size of the individual checker tiles
 *
 * Keyboard c: change the color of the active light source
 * Keyboard +: increase the 'brightness' of the active light source
 * Keyboard -: decrease the 'brightness' of the active light source
 */

import java.awt.event.MouseWheelListener;
import java.awt.event.MouseWheelEvent;
 
final int WINDOW_W = 640;
final int WINDOW_H = 480;
int RECT_SIZE = 40;

PImage tex1, tex2;

final int SPHERE_RADIUS = 28;
boolean active = true;

void draw_checkerboard3D() {
  int offset = 0;
  
  for (int i = 0; i * RECT_SIZE < WINDOW_H; i++) {
    for (int j = 0; j * RECT_SIZE < WINDOW_W; j++) {
      pushMatrix();
        int[] c = (((j + offset) % 2) == 0) ? new int[] {0, 0, 0} : new int[] {111, 111, 111};
        ambient(c[0], c[1], c[2]);
        translate(j * RECT_SIZE, i * RECT_SIZE, 0);
        box(RECT_SIZE, RECT_SIZE, 20);
      popMatrix();
    }
    offset = (offset + 1) % 2;
  }
}

void draw_sphere() {
  pushMatrix();
    noStroke();
    translate(width/2, height/2, SPHERE_RADIUS * 2 + 1);
    specular(61, 183, 180);
    shininess(1.00);
    sphere(SPHERE_RADIUS);
  popMatrix();
}

void draw_cube() {
  pushMatrix();
    noStroke();
    translate(width/2 - 60, height/2 + 60, 21);
    ambient(165, 45, 49);
    box(40, 40, 80);
  popMatrix();
}

void draw_textured_cube(PImage tex, int tx, int ty, int tz) {
  pushMatrix();
  translate(tx, ty, tz);
  scale(40);
  beginShape(QUADS);
  texture(tex);
  
  // +Z "front" face
  vertex(-1, -1,  1, 0, 0);
  vertex( 1, -1,  1, 1, 0);
  vertex( 1,  1,  1, 1, 1);
  vertex(-1,  1,  1, 0, 1);

  // -Z "back" face
  vertex( 1, -1, -1, 0, 0);
  vertex(-1, -1, -1, 1, 0);
  vertex(-1,  1, -1, 1, 1);
  vertex( 1,  1, -1, 0, 1);

  // +Y "bottom" face
  vertex(-1,  1,  1, 0, 0);
  vertex( 1,  1,  1, 1, 0);
  vertex( 1,  1, -1, 1, 1);
  vertex(-1,  1, -1, 0, 1);

  // -Y "top" face
  vertex(-1, -1, -1, 0, 0);
  vertex( 1, -1, -1, 1, 0);
  vertex( 1, -1,  1, 1, 1);
  vertex(-1, -1,  1, 0, 1);

  // +X "right" face
  vertex( 1, -1,  1, 0, 0);
  vertex( 1, -1, -1, 1, 0);
  vertex( 1,  1, -1, 1, 1);
  vertex( 1,  1,  1, 0, 1);

  // -X "left" face
  vertex(-1, -1, -1, 0, 0);
  vertex(-1, -1,  1, 1, 0);
  vertex(-1,  1,  1, 1, 1);
  vertex(-1,  1, -1, 0, 1);

  endShape();
  popMatrix();
}

// Set some values to allow the moving of lights
int px = 35, py = 40, pz = 36;
int pr = 165, pg = 45, pb = 49;

int sz = 10;
int sr = 51, sg = 102, sb = 126;
float dirY = 0 , dirX = 0;

void draw_lights() {
  if (active) {
    // Map the mouse direction for the spot light
    dirY = (mouseY / float(height) - 0.5) * 2;
    dirX = (mouseX / float(width) - 0.5) * 2;
  }
  else {
    px = mouseX;
    py = mouseY;
  }
  
  // Add an ambient light to the system: ambientLight(v1, v2, v3, x, y, z)
  ambientLight(102, 102, 102);
  
  // Add a point light to the system: pointLight(v1, v2, v3, x, y, z)
  pointLight(pr, pg, pb, px, py, pz);
  
  // Add a spot light to the system: spotLight(v1, v2, v3, x, y, z, nx, ny, nz, angle, concentration)
  spotLight(sr, sg, sb, width/2, height/2, 400, dirX, dirY, -1, PI/12, sz);  
}

void mouseClicked() { 
  active = !active;
}

void setup() {
  size(WINDOW_W, WINDOW_H, P3D);
  
  frame.addMouseWheelListener (
    new MouseWheelListener() {
      public void mouseWheelMoved (MouseWheelEvent e) {
          mouseWheel(e.getWheelRotation());
      }
    }
  );
  tex1 = loadImage("daisy.jpg");
  tex2 = loadImage("cow.jpg");
  textureMode(NORMALIZED);
}

void mouseWheel(int delta) {
  if (delta > 0)
  {
    if (RECT_SIZE < 40)
      RECT_SIZE *= 2;
  }
  else
    RECT_SIZE /= 2;
}

void keyPressed() {
  if (key == 'c')
    if (active) {
      sr = (int)random(255);
      sg = (int)random(255);
      sb = (int)random(255);
    }
    else {
      pr = (int)random(255);
      pg = (int)random(255);
      pb = (int)random(255);
    }
  if (key == '-')
  {
    if (active)
      sz++;
    else
      pz--;
  }
  if (key == '+')
  {
    if (active)
        sz--;
    else
        pz++;
  }
}

void draw() {
  background(204);
  
  beginCamera();
    // camera(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
    camera(width/2.0, WINDOW_H + 90, (height/2.0) - 30 / tan(PI*60.0 / 360.0), width/2.0, height/2.0, 0, 0, 10, 0);
  endCamera();
  
  draw_lights();
  draw_checkerboard3D();
  draw_sphere();
  draw_cube();
  draw_textured_cube(tex1, width/2 + 70, height/2 + 90, 60);
  draw_textured_cube(tex2, width/2 - 90, height/2 - 90, 60);
  
  
  //println("Camera: " + width/2.0 + ", " + WINDOW_H + 90 + ", " + ((height/2.0) - mouseY / tan(PI*60.0 / 360.0)) + ", " + width/2.0 + ", " + height/2.0 + ", " + 0 + ", " + 0 + ", " + 10 + ", " + 0);
  //println("Mouse: " + mouseX + ", y: " + mouseY);
}
