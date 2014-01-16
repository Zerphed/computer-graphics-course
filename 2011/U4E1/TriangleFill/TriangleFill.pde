final int WINDOW_W = 860;
final int WINDOW_H = 640;
PFont font;

Triangle tri;
ScanLine sl;

class Triangle {
  private PVector[] vertices;
  
  Triangle(PVector[] v) {
    if (v.length != 3)
      throw new RuntimeException("Error: invalid number of vertices for triangle.");
      
    this.vertices = v;
  }
  
  PVector[] getVertices() {
    return this.vertices;
  }
  
  void draw() {
    stroke(255);
    char[] alpha = {'A', 'B', 'C'};
    
    for (int i = 0; i < this.vertices.length; i++) {
      textAlign(CENTER);
      text(alpha[i] + "\n", (int)this.vertices[i].x, (int)this.vertices[i].y-4);
      ellipse ((int)this.vertices[i].x, (int)this.vertices[i].y, 3, 3);
    }     
  }
  
}

class Edge {
  
  private PVector v1, v2;
  
  Edge(PVector v1, PVector v2) {
     this.v1 = v1;
     this.v2 = v2;
  }
  
  PVector v1() {
    return this.v1;
  }
  
  PVector v2() {
    return this.v2;
  }
  
  void draw() {
    line(this.v1.x, this.v1.y, this.v2.x, this.v2.y);
  }
}

class ScanLine {
  
  private int x1, y1, x2, y2;
  private ArrayList intersections;
  private Edge[] edges;
  private int[] buffer;
  
  ScanLine(PVector[] vertices) {
    this.x1 = this.y1 = this.y2 = 0;
    this.x2 = WINDOW_W;
    this.intersections = new ArrayList();
    this.edges = new Edge[vertices.length];
    this.buffer = new int[WINDOW_W * WINDOW_H];
    
    initBuffer();
    
    for (int i = 0; i < vertices.length; i++)
      this.edges[i] = new Edge(vertices[i%vertices.length], vertices[(i+1)%vertices.length]);
      
  }
  
  ScanLine(int x1, int y1, int x2, int y2, PVector[] vertices) {
    this.x1 = x1;
    this.x2 = x2;
    this.y1 = y1;
    this.y2 = y2;
    this.intersections = new ArrayList();
    this.edges = new Edge[vertices.length];
    this.buffer = new int[WINDOW_W * WINDOW_H];
    
    initBuffer();
    
    for (int i = 0; i < vertices.length; i++)
      this.edges[i] = new Edge(vertices[i%vertices.length], vertices[(i+1)%vertices.length]);
  }
  
  void initBuffer() {
    loadPixels();
    for (int i = 0; i < WINDOW_W * WINDOW_H; i++)
      this.buffer[i] = color(0, 0, 0);
    updatePixels();
  }
  
  void advance() {
    if (this.y1 > WINDOW_H || this.y2 > WINDOW_H) {
      this.y1 = this.y2 = 0;
      initBuffer();
    }
    else
      this.y1 = this.y2 = this.y1 + 1;
    
    this.intersections.clear();
    this.findIntersections();
    
    if (intersections.size() > 0)
      this.fill();
  }
  
  void findIntersections() {
    for (int i = 0; i < edges.length; i++) {
      PVector r = new PVector(this.x1, this.y1);
      PVector s = new PVector(this.x2, this.y2);
      PVector p = new PVector(edges[i].v1.x, edges[i].v1.y);
      PVector q = new PVector(edges[i].v2.x, edges[i].v2.y);
    
      float divisor = (q.x - p.x)*(r.y - s.y) - (r.x - s.x)*(q.y - p.y);
      float t = ((r.x - p.x)*(r.y - s.y) - (r.x - s.x)*(r.y - p.y)) / divisor;
      float h = ((q.x - p.x)*(r.y - p.y) - (r.x - p.x)*(q.y - p.y)) / divisor;
    
      float x = this.x1 + h * (this.x2 - this.x1);
      if ((t >= 0 && t <= 1) && (h >= 0 && h <= 1))
        intersections.add(new PVector((int)x, this.y1));
    }
  }
  
  void fill() {
    loadPixels();
    
    if (intersections.size() == 1) {
      buffer[(int)((this.y1 * WINDOW_W) + ((PVector)intersections.get(0)).x)] = color(255, 102, 204);
    }
    else if (intersections.size() == 2) {
      // Color the pixels between the x values
      int x1, x2;
      x1 = min((int)(((PVector)intersections.get(0)).x), (int)(((PVector)intersections.get(1)).x));
      x2 = max((int)(((PVector)intersections.get(0)).x), (int)(((PVector)intersections.get(1)).x));
      
      for (int i = x1; i < x2; i++)
        buffer[(this.y1 * WINDOW_W) + i] = color(255, 102, 204);
    }
    else
      throw new RuntimeException("Error: WTF more than 2 intersections in a triangle!?");

    for (int i = 0; i < WINDOW_W * WINDOW_H; i++) {
      if (buffer[i] == color(255, 102, 204))
        pixels[i] = buffer[i];
    }
    
    updatePixels();
  }
  

  void draw() {
    
    textAlign(LEFT);
    text("ScanLine coordinates [" + this.x1 + ", " + this.y1 + "]", 5, 15);
    stroke(255);
    line(this.x1, this.y1, this.x2, this.y2);
  }

}

void setup() {
  size(WINDOW_W, WINDOW_H) ;
  background(0);
 
  // Load font
  font = loadFont("Corbel-15.vlw");
  textFont(font);
 
  // Create the initial random triangle
  PVector[] v = new PVector[]{new PVector(random(WINDOW_W), random(WINDOW_H)), new PVector(random(WINDOW_W), random(WINDOW_H)), new PVector(random(WINDOW_W), random(WINDOW_H))};
  tri = new Triangle(v);
  
  // Create the ScanLine object
  sl = new ScanLine(v);
}

/*
void mousePressed() {
  // Create a new random triangle
  if (mouseButton == LEFT) {
    PVector[] v = new PVector[]{new PVector(random(WINDOW_W), random(WINDOW_H)), new PVector(random(WINDOW_W), random(WINDOW_H)), new PVector(random(WINDOW_W), random(WINDOW_H))};
    tri = new Triangle(v);
    sl = new ScanLine(v);
    background(0);
  }
}
*/

void draw() {
  background(0);
  
  try {
    // Draw the triangle
    tri.draw();
    
    // Draw and advance the ScanLine
    sl.advance();
    sl.draw();
  }
  catch (Throwable t) {
    exit();
  }
}
