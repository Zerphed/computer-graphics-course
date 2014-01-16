import java.awt.event.MouseWheelListener;
import java.awt.event.MouseWheelEvent;

final int WINDOW_W = 640;
final int WINDOW_H = 480;

// Rotation specification
float rotx = PI/4;
float roty = PI/4;

// Zoom specification
int zoom = -100;

// Angle specification
float ANGLE = 0.0;

// Lathing specifications
float LRADIUS = 150.0;
float LANGLE = 0.0;

// The toroid
Toroid t;

class Toroid {
  // Color of the toroid
  int r, g, b;
  
  // Toroid properties
  private int pts;
  private int segments;
  private float radius;
  
  boolean wireframe;
  
  // Vertex arrays
  PVector v1[], v2[];
  
  Toroid(int pts, int segments, float radius) {
    // Se the toroid color
    this.r = 160;
    this.g = 45;
    this.b = 45;
    
    // Set the toroid properties
    this.pts = pts;
    this.segments = segments;    
    this.radius = radius;
    this.wireframe = true;
    
    // Initialize the vertex arrays with empty PVectors
    this.v1 = new PVector[pts+1];
    this.v2 = new PVector[pts+1];
   
    for (int i = 0; i < pts + 1; i++) {
      this.v1[i] = new PVector();
      this.v2[i] = new PVector();
    }
  }
  
  void switch_mode() {
    this.wireframe = !this.wireframe;
  }
  
  void switch_color() {
    this.r = (int)random(255);
    this.g = (int)random(255);
    this.b = (int)random(255);
  }
  
  void draw() {
    if (wireframe)
    {
      noFill();
      stroke(this.r, this.g, this.b);
    }
    else {
      noStroke();
      fill(this.r, this.g, this.b);
    }
    
    ANGLE = 0;
    for (int i = 0; i < this.pts + 1; i++) {
      v1[i].x = LRADIUS + sin(radians(ANGLE)) * this.radius;
      v1[i].z = cos(radians(ANGLE)) * this.radius;
      ANGLE += 360.0/this.pts;
    }
    
    LANGLE = 0.0;
    for (int i = 0; i < this.segments + 1; i++) {
      beginShape(QUAD_STRIP);
        for (int j = 0; j < this.pts + 1; j++) {
          if (i > 0)
            vertex(v2[j].x, v2[j].y, v2[j].z);
          
          v2[j].x = cos(radians(LANGLE)) * v1[j].x;
          v2[j].y = sin(radians(LANGLE)) * v1[j].x;
          v2[j].z = v1[j].z;
          
          vertex(v2[j].x, v2[j].y, v2[j].z);
        }
      LANGLE += 360/this.segments;   
      endShape();
    }
  }
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
  
  t = new Toroid(40, 60, 80.0);
}

void draw() {
  try {
    // Set the background to gray
    background(0);
    
    // Put the lighs on
    lights();
    
    pushMatrix();
      // Translate the toroid to the center
      translate(width/2, height/2, zoom);
      
      rotateX(rotx);
      rotateY(roty);
    
      // Draw the toroid
      t.draw();
    popMatrix();
  }
  catch (Throwable t) {
    exit(); 
  }
}

void mouseWheel(int delta) {
  if (delta > 0)
    zoom+=10;
  else
    zoom-=10;
}

void mouseDragged() {
  float rate = 0.01;
  rotx += (pmouseY-mouseY) * rate;
  roty += (mouseX-pmouseX) * rate;
}

void keyPressed() {
  if (key == 'w')
    t.switch_mode();
  if (key == 'c')
    t.switch_color();
}
