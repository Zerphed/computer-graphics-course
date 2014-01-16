import processing.opengl.*;

final int WINDOW_W = 860;
final int WINDOW_H = 640;

final int NTRIANGLES = 3;
ArrayList triangles;

public class Triangle {
 
  private int x1, y1, x2, y2, x3, y3;
  private int r, g, b;
  
  private int cx, cy;
  private float cradius;
  
  private int bcx, bcy;
  private float bcradius;
  
  boolean intersect;
  
  Triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    this.x1 = x1;
    this.y1 = y1;
    this.x2 = x2;
    this.y2 = y2;
    this.x3 = x3;
    this.y3 = y3;
    
    this.r = (int)random(255);
    this.g = (int)random(255);
    this.b = (int)random(255);
    
    this.circumcircle();
    this.boundingcircle();
    this.intersect = false;
  }
  
  void setIntersect(boolean val) {
    this.intersect = val;
  }
  
  void draw() {
    
    // Draw the triangle
    if (this.intersect)
      fill(255, 255, 255, 50);
    else
      fill(r, g, b);
    triangle(x1, y1, x2, y2, x3, y3);
    
    
    // Draw the bounding circle
    ellipseMode(CENTER);
    stroke(4);
    noFill();
    //ellipse(this.cx, this.cy, 2*this.cradius, 2*this.cradius);
    ellipse(this.bcx, this.bcy, 2*this.bcradius, 2*this.bcradius);
    
    // Draw the circumcenter label
    fill(255, 255, 255);
    text("O (" + this.bcx + ", " + this.bcy + ")", this.bcx, this.bcy);
    
    // Draw the corner labels
    PFont font;
    font = loadFont("Arial-BoldMT-20.vlw");
    textFont(font, 15);
    fill(255, 255, 255);
    text("A (" + this.x1 + ", " + this.y1 + ")", this.x1, this.y1);
    text("B (" + this.x2 + ", " + this.y2 + ")", this.x2, this.y2);
    text("C (" + this.x3 + ", " + this.y3 + ")", this.x3, this.y3);
    
  }
  
  // Well this is one possibility
  private void circumcircle() {
    
    // B - A
    float a = this.x2 - this.x1;
    float b = this.y2 - this.y1;
    
    // C - A
    float c = this.x3 - this.x1;
    float d = this.y3 - this.y1;
    
    float e = a * (this.x2 + this.x1) * 0.5 + b * (this.y2 + this.y1) * 0.5;
    float f = c * (this.x3 + this.x1) * 0.5 + d * (this.y3 + this.y1) * 0.5;
    float det = a*d - b*c;    

    int cx = (int)((d*e - b*f)/det);
    int cy = (int)((-c*e + a*f)/det);
    
    float r = sqrt((this.x1-cx)*(this.x1-cx) + (this.y1-cy)*(this.y1-cy));
    
    // Set the private values
    this.cx = cx;
    this.cy = cy;
    this.cradius = r;
  }
  
  // Let's see if this does a prettier job than its predecessor above
  private void boundingcircle() {
    if (pointInTriangle(new PVector(this.cx, this.cy))) {
      this.bcx = this.cx;
      this.bcy = this.cy;
      this.bcradius = this.cradius;
      return;
    }
    
    LineSegment ab = new LineSegment(this.x1, this.y1, this.x2, this.y2);
    LineSegment bc = new LineSegment(this.x2, this.y2, this.x3, this.y3);
    LineSegment ca = new LineSegment(this.x3, this.y3, this.x1, this.y1);
    
    LineSegment[] edges = new LineSegment[]{ab, bc, ca};
    
    // Find the longest edge
    LineSegment longest = ab;
    for (int i = 1; i < 3; i++)
      if (edges[i].length() > longest.length())
        longest = edges[i];
    
    // The radius of the bounding circle if half of the line segment
    this.bcradius = longest.length() / 2;
    this.bcx = (int)(abs(longest.x2() - longest.x1()) / 2) + min(longest.x1(), longest.x2()); // I am not proud of this...
    this.bcy = (int)(abs(longest.y2() - longest.y1()) / 2) + min(longest.y1(), longest.y2()); // I am not proud of this...
  }
  
  /* Idea:
   * 1. Draw circumcircles around the triangles
   * 2. Check whether the circles of the Triangles intersect
   *    If yes go to step 3 and 4, else stop.
   * 3. Check whether any edges of the triangles overlap if 
   *    not goto step 4
   * 4. Check whether any vertices are inside other triangles.
   */
  
  // This method checks whether there is an intersection
  // @param idx of this triangle
  boolean intersect(int idx) {
    if (this.intersect) return true;

    // This triangle's circumcircle's circumcenter
    PVector tbc = new PVector(this.bcx, this.bcy);
    
    boolean cintersect = false;
    
    for (int i = 0; i < NTRIANGLES; i++) {
      Triangle current = ((Triangle)triangles.get(i));
      
      // Check whether the current triangle is this one
      if (idx == i) continue;
      
      // Other triangle's circumcircle's circumcenter
      PVector obc = new PVector(current.bcx, current.bcy);
      
      // If the distance between circumcenters is smaller than
      // the combined radiuses there is a possible intersect, if not
      // there can be no intersect
      if (tbc.dist(obc) < this.bcradius + current.bcradius) {
        cintersect = true;
        break;
      }
    }
    
    if (!cintersect) return false;
    
    // Intersection is possible, check for any intersections between individual
    // triangle edges
    
    // Push all the edges of other triangles into an array of edges
    ArrayList o_edges = new ArrayList();
    ArrayList t_edges = new ArrayList();
    
    for (int i = 0; i < NTRIANGLES; i++) {
      Triangle current = ((Triangle)triangles.get(i));
      if (i == idx) continue;
      
      // Add the edges to the array
      o_edges.add(new LineSegment(current.x1, current.y1, current.x2, current.y2));
      o_edges.add(new LineSegment(current.x2, current.y2, current.x3, current.y3));
      o_edges.add(new LineSegment(current.x3, current.y3, current.x1, current.y1));
    }
    
    t_edges.add(new LineSegment(this.x1, this.y1, this.x2, this.y2));
    t_edges.add(new LineSegment(this.x2, this.y2, this.x3, this.y3));
    t_edges.add(new LineSegment(this.x3, this.y3, this.x1, this.y1));
    
    for (int i = 0; i < t_edges.size(); i++) {
      for (int j = 0; j < o_edges.size(); j++) {
        if (((LineSegment)t_edges.get(i)).intersect((LineSegment)o_edges.get(j))) return true;
      }
    }
    
    // So still no cigar, continue by checking whether the triangle is inside the other:
    // "If the point was on the same side of AB as C and is also on the same side of BC as A 
    // and on the same side of CA as B, then it is in the triangle."
    boolean ret = false;
    for (int i = 0; i < NTRIANGLES; i++) {
      Triangle current = ((Triangle)triangles.get(i));
      if (i == idx) continue;
      
      PVector p1 = new PVector(current.x1, current.y1);
      PVector p2 = new PVector(current.x2, current.y2);
      PVector p3 = new PVector(current.x3, current.y3);
      
      if (pointInTriangle(p1) || pointInTriangle(p2) || pointInTriangle(p3)) {
        current.setIntersect(true);
        ret = true;
      }
    }
    
    return ret;
  }
  
  // @param: point coordinates p
  private boolean pointInTriangle(PVector p) {
    // Point on same side of AB as C
    boolean ab = pointsOnSameSide(p, new PVector(this.x1, this.y1), new PVector(this.x2, this.y2), new PVector(this.x3, this.y3));
    // Point on same side of BC as A
    boolean bc = pointsOnSameSide(p, new PVector(this.x2, this.y2), new PVector(this.x1, this.y1), new PVector(this.x3, this.y3));
    // Point on same side of CA as B
    boolean ca = pointsOnSameSide(p, new PVector(this.x3, this.y3), new PVector(this.x1, this.y1), new PVector(this.x2, this.y2));
    
    if (ab && bc && ca) return true;
    return false;
  }
  
  // @param: point 1 coordinates: p1
  //         point 2 coordinates: p2
  //         line coordinates...: a, b
  private boolean pointsOnSameSide(PVector p1, PVector p2, PVector a, PVector b) {
    PVector cp1 = PVector.sub(b, a).cross(PVector.sub(p1, a));
    PVector cp2 = PVector.sub(b, a).cross(PVector.sub(p2, a));
    if (cp1.dot(cp2) >= 0) return true;
    return false;
  }
  
}


class LineSegment {
  
  // LineSegment has a start (x1, y1) and an end point (x2, y2)
  private int x1, y1, x2, y2;
  
  LineSegment(int x1, int y1, int x2, int y2) {
    this.x1 = x1;
    this.y1 = y1;
    this.x2 = x2;
    this.y2 = y2;
  }
  
  int x1() {
    return this.x1;
  }
  
  int x2() {
    return this.x2;
  }
  
  int y1() {
    return this.y1;
  }
  
  int y2() {
    return this.y2;
  }
  
  boolean intersect(LineSegment other) {

    PVector r = new PVector(this.x1, this.y1);
    PVector s = new PVector(this.x2, this.y2);
    PVector p = new PVector(other.x1, other.y1);
    PVector q = new PVector(other.x2, other.y2);
    
    float divisor = (q.x - p.x)*(r.y - s.y) - (r.x - s.x)*(q.y - p.y);
    float t = ((r.x - p.x)*(r.y - s.y) - (r.x - s.x)*(r.y - p.y)) / divisor;
    float h = ((q.x - p.x)*(r.y - p.y) - (r.x - p.x)*(q.y - p.y)) / divisor;
    
    if ((t >= 0 && t <= 1) && (h >= 0 && h <= 1)) return true;
    return false;
  }
  
  float length() {
    return sqrt(pow(this.x2 - this.x1, 2) + pow(this.y2 - this.y1, 2));
  }
}

void setup() {
  size(WINDOW_W, WINDOW_H, OPENGL);
  smooth();
  
  triangles = new ArrayList();

  /* Situation with other triangle inside the other. */
  
  triangles.add(new Triangle(WINDOW_W/2, 10, 10, WINDOW_H-10, WINDOW_W-10, WINDOW_H-10));  
  //for (int i = 0; i < NTRIANGLES-1; i++)
  triangles.add(new Triangle(295, 520, 695, 567, 400, 202));
  triangles.add(new Triangle(350, 250, 240, 400, 350, 230));
  
  
  /*
  for (int i = 0; i < NTRIANGLES; i++)
    triangles.add(new Triangle((int)random(WINDOW_W-20), (int)random(WINDOW_H-20), (int)random(WINDOW_W-20), (int)random(WINDOW_H-20), (int)random(WINDOW_W-20), (int)random(WINDOW_H-20)));
  */
  for (int i = 0; i < NTRIANGLES; i++) {
    Triangle current = ((Triangle)triangles.get(i));
    boolean ret = current.intersect(i);
    if (ret) {
      current.setIntersect(ret);
      continue;
    }
  }
  
}

void mouseClicked() {
 
  triangles.clear();
  for (int i = 0; i < NTRIANGLES; i++)
    triangles.add(new Triangle((int)random(WINDOW_W-20), (int)random(WINDOW_H-20), (int)random(WINDOW_W-20), (int)random(WINDOW_H-20), (int)random(WINDOW_W-20), (int)random(WINDOW_H-20)));
    
  for (int i = 0; i < NTRIANGLES; i++) {
    Triangle current = ((Triangle)triangles.get(i));
    boolean ret = current.intersect(i);
    if (ret) {
      current.setIntersect(ret);
      continue;
    }
  }
}

void draw() {
  background(60);
  
  for (int i = 0; i < NTRIANGLES; i++)
    ((Triangle)triangles.get(i)).draw();
    
}
