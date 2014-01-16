import processing.opengl.*;

int SCREEN_W = 800;
int SCREEN_H = 600;

public abstract class Geometry {
  
  abstract void draw();
  abstract boolean hit(float x, float y);
}

class Transform {

  private Geometry g;
  private float[][] m;
  
  public Transform(float[][] m, Geometry g) {
    this.m = m; // 4x4 matrix
    this.g = g;
  }

  public void draw() {
    pushMatrix();
    
    applyMatrix(this.m[0][0], this.m[0][1], this.m[0][2], this.m[0][3],
                this.m[1][0], this.m[1][1], this.m[1][2], this.m[1][3],
                this.m[2][0], this.m[2][1], this.m[2][2], this.m[2][3],
                this.m[3][0], this.m[3][1], this.m[3][2], this.m[3][3]
                );

    g.draw();
    popMatrix();
  }
  
}


public class Triangle extends Geometry {

  private float x1, y1, x2, y2, x3, y3;
  
  public Triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    this.x1 = x1;
    this.y1 = y1;
    this.x2 = x2;
    this.y2 = y2;
    this.x3 = x3;
    this.y3 = y3;
  }
  
  void draw() {
    triangle(this.x1, this.y1, this.x2, this.y2, this.x3, this.y3);
  }
  
  // INCOMPLETE
  boolean hit(float x, float y) {
    return true;
  }
}


public class Rectangle extends Geometry {

  private float x, y, width, height;
  
  public Rectangle(float x, float y, float width, float height) {
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
  }
  
  void draw() {
    rect(this.x, this.y, this.width, this.height);
  }
  
  // INCOMPLETE
  boolean hit(float x, float y) {
    return true;
  }
}

void setup() {
  size(SCREEN_W, SCREEN_H, OPENGL);
  background(100);
  smooth();
}

void draw() {
  background(100);
  // Rectangle with a transform matrix
  fill(204, 102, 0);
  Rectangle r = new Rectangle(200, 200, 90, 90);
  float[][] m_r = new float[][]{{cos(millis()/250),-sin(millis()/500),0,100},{1,cos(millis()/500),0,200},{0,0,1,0},{0,0,0,1}};
  Transform tf_r = new Transform(m_r, r);
  
  tf_r.draw();
  r.draw();
  
  // Triangle with a transform matrix
  fill(255, 255, 255);
  Triangle t = new Triangle(30, 75, 58, 20, 86, 75);
  float[][] m_t = new float[][]{{cos(millis()/500),0,0,100},{0,1,0,300},{0,0,1,0},{0,0,0,1}};
  Transform tf_t = new Transform(m_t, t);
  
  tf_t.draw();
  t.draw();
}
