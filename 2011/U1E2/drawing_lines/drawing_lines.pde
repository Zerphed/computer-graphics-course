int SCREEN_W = 860;           // Screen width
int SCREEN_H = 640;           // Screen height
float TOLERANCE = 20.0;       // Tolerance of the "snap to place"
float TIME_PER_UNIT = 10.0;   // Time it takes for the image to traverse one point vector length unit

ArrayList<PVector> pvectors;  // An array of point vectors according to which the shape is formed
PImage img;                   // The image that appears once the shape is complete
PFont fontA;                  // Font for printing the mouse coordinates

// Initialize the screen
void setup() {
  // Create the array list and load the image
  pvectors = new ArrayList<PVector>();
  img = loadImage("data/amigab.png");
  
  // Set the font and its size (in units of pixels)
  fontA = loadFont("data/Ziggurat-HTF-Black-32.vlw");
  textFont(fontA, 15);
  
  // Set screen size and background
  size(SCREEN_W, SCREEN_H, P3D);
  background(255, 255, 255);
  smooth();
}

// Check whether the shape is closed
boolean isClosed() {
  int size = pvectors.size();

  if (size > 3) {
    float distance = pvectors.get(size-1).dist(pvectors.get(0));
    if (distance < TOLERANCE) // If the euclidean distance between start and end is < 20 px fill in the gap
      return true;
  }
  
  return false;
}

// Draw the damn animated object
void animateObject(int ms) {
  
  // Let's relate the time to which vector
  // the picture is in
  int index = (ms / 1000) % pvectors.size();

  // Get the two points a and b
  PVector a = pvectors.get((index));
  PVector b = pvectors.get((index+1) % pvectors.size());

  // Distance in x-, y-axis and euclidean distance
  float dx = b.x - a.x;
  float dy = b.y - a.y;
  float distance = a.dist(b);
  
  // The time traversed between points a and b
  float time = ms % 1000 / 1000.0;
  
  float x = a.x + (int)(time * dx);
  float y = a.y + (int)(time * dy);
  
  imageMode(CENTER);
  image(img, x, y, 40, 40);
}

// Draws info, mouse coord, fps etc..
void drawInfo() {
  fill(0);
  textAlign(RIGHT);
  text("X: " + mouseX + "  Y: " + mouseY + "\n" + (int)frameRate + " fps", SCREEN_W - 15, 20);
}

// The main draw loop
void draw() {
  background(255, 255, 255);
  int size = pvectors.size();
  
  // Just for teh lulz draw the mouse coordinates and fps in the up right corner
  drawInfo();
  
  // If there is a PVector and the shape is not closed, model the new
  // line for the user
  if (size > 0 && !isClosed()) {
    stroke(1, 90);
    strokeWeight(2);
    
    if ( size > 2 && dist(pvectors.get(0).x, pvectors.get(0).y, mouseX, mouseY) < TOLERANCE ) {
      line(pvectors.get(size-1).x, pvectors.get(size-1).y, pvectors.get(0).x, pvectors.get(0).y);
      mouseX = (int)pvectors.get(0).x;
      mouseY = (int)pvectors.get(0).y;
    }
    else
      line(pvectors.get(size-1).x, pvectors.get(size-1).y, mouseX, mouseY);
  }
  
  // Draw all the lines that are ready (between start and end coordinates)
  stroke(1);
  strokeWeight(2);
  
  for (int i = 0; i < size-1; i++)
    line(pvectors.get(i).x, pvectors.get(i).y, pvectors.get(i+1).x, pvectors.get(i+1).y);
  
  // If the shape is closed animate the object
  if (isClosed()) {
    animateObject(millis());
  }
}

// Action taken when mouse pressed
void mousePressed() {
  
  // If the shape is closed no more drawing dammit!
  // If it's not add the new PVector to the array
  if (!isClosed()) {
    PVector newVec = new PVector(mouseX, mouseY);
    pvectors.add(newVec);
  }
}
