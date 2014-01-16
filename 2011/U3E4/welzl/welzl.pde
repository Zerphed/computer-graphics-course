import processing.opengl.*;

final int WINDOW_W = 860;
final int WINDOW_H = 640;

Circle circle;
ArrayList points;
PFont fontA;
PFont fontB;

class Circle {
  private PVector c;
  private float r;
  
  Circle (float x, float y, float r) {
    this.c = new PVector(x, y);
    this.r = r;
  }
  
  PVector center() {
    return this.c;
  }
  
  float radius() {
    return this.r;
  }
  
  void reset() {
    this.c.set(0, 0, 0);
    this.r = 0;
  }
  
  void draw() {
    stroke(1);
    ellipse(c.x, c.y, 2*r, 2*r);
  }
  
}

boolean pointInCircle(PVector point, Circle c) {
  if (point.dist(c.center()) > c.radius()) 
    return false;
  return true;
}

Circle calculateCircle(ArrayList r) {
  if (r.size() == 1) {
    return new Circle(((PVector)r.get(0)).x, ((PVector)r.get(0)).y, 0);
  } 
  else if (r.size() == 2) {
    PVector a = (PVector)r.get(0);
    PVector b = (PVector)r.get(1);
    
    // The radius of the enclosing circle if half of the line segment
    float radius = a.dist(b) / 2;
    float x = ((abs(b.x - a.x) / 2) + min(a.x, b.x));
    float y = ((abs(b.y - a.y) / 2) + min(a.y, b.y));
    
    return new Circle(x, y, radius);
  }
  else if (r.size() == 3) {
    float x1, x2, x3;
    float y1, y2, y3;
    
    x1 = ((PVector)r.get(0)).x;
    x2 = ((PVector)r.get(1)).x;
    x3 = ((PVector)r.get(2)).x;
    
    y1 = ((PVector)r.get(0)).y;
    y2 = ((PVector)r.get(1)).y;
    y3 = ((PVector)r.get(2)).y;
    
    // B - A
    float a = x2 - x1;
    float b = y2 - y1;
    
    // C - A
    float c = x3 - x1;
    float d = y3 - y1;
    
    float e = a * (x2 + x1) * 0.5 + b * (y2 + y1) * 0.5;
    float f = c * (x3 + x1) * 0.5 + d * (y3 + y1) * 0.5;
    float det = a*d - b*c;    
  
    float cx = (d*e - b*f) / det;
    float cy = (-c*e + a*f) / det;
    
    float radius = sqrt((x1-cx)*(x1-cx) + (y1-cy)*(y1-cy));
    
    return new Circle(cx, cy, radius);
  }
  else
    throw new RuntimeException("Too many circle points: " + r.size());
}

ArrayList copyArrayList(ArrayList a) {
  ArrayList ret = new ArrayList();
  
  for (int i = 0; i < a.size(); i++)
    ret.add(new PVector(((PVector)a.get(i)).x, ((PVector)a.get(i)).y));
    
  return ret;
}

// @param:
// p: the set of points in the plane. 
// r: the set of points lying on the boundary of the current circle.
// @return:
// the smallest enclosing circle
Circle welzl(ArrayList p, ArrayList r) {
  Circle c;
  
  if (p.size() == 0 && r.size() == 0)
    return new Circle(0, 0, 0);
  else if (p.size() == 0 || r.size() == 3)
    c = calculateCircle(r);
  else {
    int idx = (int)random(p.size());
    PVector point = (PVector)p.get(idx);
    
    // Important: keep the original ArrayList's intact after calling the
    //            necessary recursion.
    p.remove(idx);
    c = welzl(p, r);
    
    if (!pointInCircle(point, c)) {
      r.add(point);
      c = welzl(p, r);
      r.remove(point);  // Keep the original r intact
    }
    
    p.add(idx, point);  // Keep the original p intact
  }
  

  return c;
}

void setup() {
  background(255, 255, 255);
  size(860, 640, OPENGL);
  smooth();
  
  // Initialize the global variables
  points = new ArrayList();
  circle = new Circle(0, 0, 0);
  fontA = loadFont("AgencyFB-Reg-20.vlw");
  fontB = loadFont("AgencyFB-Reg-15.vlw");
}

void mousePressed() {
  try {
    
    if (mouseButton == LEFT)
      points.add(new PVector(mouseX, mouseY));
      
    else if (mouseButton == RIGHT) {
      ArrayList pointsCopy = copyArrayList(points);
      ArrayList pointsOnCircle = new ArrayList();
      circle = welzl(pointsCopy, pointsOnCircle);
    }
    else if (mouseButton == CENTER) {
      points.clear();
      circle.reset();
    }
  }
  catch (Throwable t) {
    exit();
  }
}

void draw() {
    
  background(255, 255, 255);
  
  noStroke();
  for (int i = 0; i < points.size(); i++) {
    PVector current = ((PVector)points.get(i));
    
    fill(40, 0, 200);
    rectMode(CENTER);
    rect(current.x, current.y, 7, 7);
    
    textAlign(CENTER);
    textFont(fontB, 15);
    text("\n[" + (int)current.x + ", " + (int)current.y + "]", current.x, current.y);
  }
  
  noFill();
  stroke(0);
  strokeWeight(2);
  ellipse(circle.center().x, circle.center().y, circle.radius()*2, circle.radius()*2);

  if (circle.radius() != 0) {
    textAlign(CENTER);
    textFont(fontB, 15);
    text("*\n[" + (int)circle.center().x + ", " + (int)circle.center().y + "]", circle.center().x, circle.center().y);
    stroke(0);
    strokeWeight(2);
    line(circle.center().x, circle.center().y, circle.center().x + circle.radius(), circle.center().y);
  }
  
  textAlign(LEFT);
  textFont(fontA, 20);
  fill(40, 0, 200);
  text("Area: " + (circle.radius() * 2 * PI) + "\nRadius: " + circle.radius() + "\nPoints: " + points.size(), 10, 20);
}
