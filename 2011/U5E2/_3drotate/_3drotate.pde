final int WINDOW_W = 640;
final int WINDOW_H = 480;

float rotx = PI/4;
float roty = PI/4;

float transx = width/2 + 300;
float transy = width/2 + 200;
float transz = -500;

void setup() 
{
  size(WINDOW_W, WINDOW_H, P3D);
  fill(255);
  stroke(color(44,48,32));
}

void draw() 
{
  try {  
    // Draw the cube
    pushMatrix();
      background(0);
      strokeWeight(2);
      translate(transx, transy, transz);
      
      // Apply the rotation
      rotateX(rotx);
      rotateY(roty);
      
      // Draw the lights and the cube
      lights();
      ambientLight(51, 102, 126);
      box(180);
    popMatrix();
  }
  catch (Throwable t) {
    exit();
  }
}

void mouseDragged() {
  float rate = 0.01;
  rotx += (pmouseY-mouseY) * rate;
  roty += (mouseX-pmouseX) * rate;
}

void keyPressed() {
  if (key == CODED) {
    if (keyCode == UP)
      transz+=10;
    else if (keyCode == DOWN)
      transz-=10;
  }
}
