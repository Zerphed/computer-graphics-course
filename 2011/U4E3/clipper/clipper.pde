import processing.opengl.*;

final int WINDOW_W = 1280;
final int WINDOW_H = 640;
final int NSPACES  = 4;

PFont font;

Polygon poly;
Halfspace[] spaces;

class Polygon {
  
  private ArrayList vertices;
  
  Polygon(PVector[] v) {
    this.vertices = new ArrayList();
    
    for (int i = 0; i < v.length; i++)
      this.vertices.add(new PVector(v[i].x, v[i].y));
  }
  
    void suthHodgClip(Halfspace hs) {
      if (this.vertices.isEmpty()) return;
    // Initialize a and b as the first vertex
    ArrayList r_vertices=new ArrayList();
    PVector a = new PVector(((PVector)this.vertices.get(0)).x, ((PVector)this.vertices.get(0)).y);
    PVector b = new PVector(((PVector)this.vertices.get(0)).x, ((PVector)this.vertices.get(0)).y);
    
    // Declare the temp values
    PVector v_t, a_t;
    
    // Initialize in as supposed to in the first loop iteration
    boolean in = hs.contains(a), in_t;
    

    //println(hs.p1);
    //println(hs.p2);
    //println(hs.n);
    int i;
    for (i = 0; i < this.vertices.size(); i++) {
      //println("Index now "+i);
      //println((PVector)this.vertices.get(i));
      if (i != 0) {
        in_t = hs.contains((PVector)this.vertices.get(i));
        //println("Point in: " + in_t);
        //println("Previous point: "+in);
        if (in != in_t) {
          v_t = hs.intersection(b, (PVector)this.vertices.get(i));
          this.vertices.add(i, v_t);
          i++;
              //println("Adding "+ v_t+" at "+(i-1));
          if (!in_t) {
              //println("Removing (1) from "+i);
            r_vertices.add(i);
          }
        }
        else{
          if (!in) {
            //println("Removing (2) from "+i);
            r_vertices.add(i);
          }
        }
        
        in = in_t;
      }
      else{
        if (!in) {
        //println("Removing (3) from "+i);
        r_vertices.add(i);
        }
      }
      
      b = (PVector)this.vertices.get(i);

      
    }
    //println("Last line");
    //println(b);
    //println(a);
    in_t = hs.contains(a);
    if (in != in_t) {
      a_t = hs.intersection(b, a);
      this.vertices.add(this.vertices.size(), a_t);
      //println("Adding "+ a_t+" at "+(this.vertices.size()-1));
    }
    //println("Begin delete");
    for ( i = r_vertices.size()-1; i>=0; i--){
      //println("Delete index "+((Number)r_vertices.get(i)).intValue());
      this.vertices.remove(((Number)r_vertices.get(i)).intValue());
    }
  }
  
  void draw() {
    if (vertices.size() == 0)
      return;
    
    noStroke();
    fill(color(204, 102, 0, 110));
    beginShape();

    for (int i = 0; i < vertices.size(); i++)
      vertex(((PVector)vertices.get(i)).x, ((PVector)vertices.get(i)).y);
    
    endShape(CLOSE);
  }
}

// Equation of a halfspace: ax + by + a <= 0
class Halfspace {
  private PVector p1, p2, n;
  float a;
  
  // The window dimensions are known so we can do without the Cyrus-Beck algorithm
  Halfspace(PVector p1, PVector p2) {
    if (p1.x == p2.x && p1.y == p2.y)
      throw new RuntimeException("Error: cannot determine a line between two identical points.");
    
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    this.n = new PVector(-dy, dx);
    
    // We got all the information we need, now calculate the 
    // intersections
    float xmin = 0, ymin = 0, xmax = 0, ymax = 0;
    a = p2.x * p1.y - p1.x * p2.y;
    
    ArrayList intersections = new ArrayList();
    
    // Equation of a line: x * n = a <==> x * n1 + y * n2 = a, where n1 = -dy, n2 = dx
    
    if (dx != 0) {                       // If dx is zero the line is vertical
      xmin = a / dx;                     // Point y when x == 0
      xmax = (a + WINDOW_W * dy) / dx;   // Point y when x == WINDOW_W
      
      if (xmin > 0 && xmin < WINDOW_H)
        intersections.add(new PVector(0-3, xmin));
      if (xmax > 0 && xmax < WINDOW_H)
        intersections.add(new PVector(WINDOW_W+3, xmax));
    }
    
    if (dy != 0) {                      // If dy is zero the line is horizontal
      ymin = -a / dy;                   // Point x when y == 0
      ymax = (WINDOW_H * dx - a) / dy;  // Point x when y == WINDOW_H
      
      if (ymin > 0 && ymin < WINDOW_W)
        intersections.add(new PVector(ymin, 0-3));
      if (ymax > 0 && ymax < WINDOW_W)
        intersections.add(new PVector(ymax, WINDOW_H+3));
    }
    
    if (intersections.size() != 2)
      throw new RuntimeException("Error: invalid number of intersections (" + intersections.size() + ") between the window and line.");
    
    this.p1 = (PVector)intersections.get(0);
    this.p2 = (PVector)intersections.get(1);
    this.n.normalize();
    this.a = this.p1.dot(this.n);
  }
  
  boolean contains(PVector p) {
    // If point p is inside: p (dot) n > a
    return p.dot(this.n) > this.a;
  }
  
  // Calculates the intersection between the halfspace's line and the line-segment pq
  // given as parameter
  PVector intersection(PVector p, PVector q) {
    float divisor = (q.x - p.x) * (this.p1.y - this.p2.y) - (this.p1.x - this.p2.x) * (q.y - p.y);
    float t = ((this.p1.x - p.x) * (this.p1.y - this.p2.y) - (this.p1.x - this.p2.x) * (this.p1.y - p.y)) / divisor;
    float h = ((q.x - p.x) * (this.p1.y - p.y) - (this.p1.x - p.x) * (q.y - p.y)) / divisor;
  
    float x = this.p1.x + h * (this.p2.x - this.p1.x);
    float y = this.p1.y + h * (this.p2.y - this.p1.y);
    
    if ((t >= 0 && t <= 1) && (h >= 0 && h <= 1))
      return new PVector(x, y);
    else
      throw new RuntimeException("Error: the two line-segments do not intersect."); 
  }
  
  void draw() {
    // Draw the line
    stroke(0);
    strokeWeight(3);
    line(this.p1.x, this.p1.y, this.p2.x, this.p2.y);
    
    // Draw the red glow marking the side of the halfspace
    stroke(color(255, 0, 0, 110));
    strokeWeight(4);
    line(this.p1.x + (this.n.x * -3), this.p1.y + (this.n.y * -3), this.p2.x + (this.n.x * -3), this.p2.y + (this.n.y * -3));
    
    // Draw the coordinates
    //fill(0);
    //textAlign(CENTER);
    //text("p1: [" + (int)this.p1.x + ", " + (int)this.p1.y + "]", this.p1.x > WINDOW_W/2 ? (int)this.p1.x - 45 : (int)this.p1.x + 40, this.p1.y > WINDOW_H/2 ? (int)this.p1.y - 30 : (int)this.p1.y + 30);
    //text("p2: [" + (int)this.p2.x + ", " + (int)this.p2.y + "]", this.p2.x > WINDOW_W/2 ? (int)this.p2.x - 45 : (int)this.p2.x + 40, this.p2.y > WINDOW_H/2 ? (int)this.p2.y - 30 : (int)this.p2.y + 30);
  }
}

void mousePressed() {
  if (mouseButton == LEFT) {
    for (int i = 0; i < spaces.length; i++)
      spaces[i] = new Halfspace(new PVector(random(WINDOW_W+1), random(WINDOW_H+1)), new PVector(random(WINDOW_W+1), random(WINDOW_H+1)));

    poly = new Polygon(new PVector[]{new PVector(0, 0), new PVector(WINDOW_W, 0), new PVector(WINDOW_W, WINDOW_H), new PVector(0, WINDOW_H)});
    loop();
  }
}

void setup() {
  size(WINDOW_W, WINDOW_H, OPENGL);
  hint(ENABLE_OPENGL_4X_SMOOTH);
  background(255);
  
  font = loadFont("Dialog.plain-20.vlw");
  textFont(font, 15);
  
  poly = new Polygon(new PVector[]{new PVector(0, 0), new PVector(WINDOW_W, 0), new PVector(WINDOW_W, WINDOW_H), new PVector(0, WINDOW_H)});
  
  spaces = new Halfspace[NSPACES];
  for (int i = 0; i < spaces.length; i++)
    spaces[i] = new Halfspace(new PVector(random(WINDOW_W+1), random(WINDOW_H+1)), new PVector(random(WINDOW_W+1), random(WINDOW_H+1)));
}

void draw() {
  try {
    background(255);
    for (int i = 0; i < spaces.length; i++) {
      spaces[i].draw();
      poly.suthHodgClip(spaces[i]);
    }
    poly.draw();
    noLoop();
  }
  catch (Throwable t) {
    println("Message: " + t.getMessage());
    println("Cause: " + t.getCause());
    println("Stack trace:");
    t.printStackTrace();
    
    exit();
  }
}

/*
  for (int i = 0; i < spaces.length; i++)
    spaces[i]=spaces();
Halfspace spaces(){
  float r = random(min(WINDOW_W,WINDOW_H)/2-100)+50;
  float alp = random(360);
  float ca=cos(alp/360*2*PI);
  float sa=sin(alp/360*2*PI);
  PVector a= new PVector(WINDOW_W/2+ca*r,WINDOW_H/2+sa*r);
  PVector b=new PVector(WINDOW_W/2+ca*r-sa,WINDOW_H/2+sa*r+ca);
  return new Halfspace(a,b);
//  return new Halfspace(new PVector(random(WINDOW_W+1), random(WINDOW_H+1)), new PVector(random(WINDOW_W+1), random(WINDOW_H+1)));
}
*/
