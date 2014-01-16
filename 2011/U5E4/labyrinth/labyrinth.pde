import processing.opengl.*;

final int WINDOW_W = 640;
final int WINDOW_H = 480;

final int CUBE_SIZE = 1;
final int SCALE_FACTOR = 40;

final int WORLD_W = 10;
final int WORLD_H = 10;


class Cube {
  private float x, y, z;

  Cube(float x, float y, float z) {
    this.x = x;
    this.y = y;
    this.z = z;
  }
  
  void draw() {
    pushMatrix();
    
    translate(this.x, this.y, this.z);
    scale(SCALE_FACTOR);
    box(CUBE_SIZE);
    
    popMatrix();
  }
}

class World {
  private int[][] worldmap;
  
  World() {
    this.worldmap = new int[WORLD_W][WORLD_H];
  }
}

class Player {
  
  private float x, y;
  
  Player(float x, float y) {
    this.x = x;
    this.y = y;
  }
  
  void move() {
    
  }
}

Cube c1, c2;

void setup() {
  size(WINDOW_W, WINDOW_H, OPENGL);
  c1 = new Cube(SCALE_FACTOR, SCALE_FACTOR, 0);
  c2 = new Cube(SCALE_FACTOR, SCALE_FACTOR*2, 0);
}

void draw() {
  background(150);
  
  try {
    c1.draw();
    c2.draw();
  } 
  catch (Throwable t) {
    exit(); 
  }
}
