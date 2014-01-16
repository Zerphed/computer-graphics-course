final int WINDOW_W = 500;
final int WINDOW_H = 465;

PFont font;

PImage source;
PImage dest;
PImage comparison;

// Convolution matrices, sharpen and blur
float[][] smatrix = {{ -1, -1, -1 },
                     { -1,  9, -1 },
                     { -1, -1, -1 } }; 

float v = 1.0/9.0;
float[][] bmatrix = { { v, v, v },
                      { v, v, v },
                      { v, v, v } }; 

color cfilter(int x, int y, float[][] m, int msize, PImage img) {
  // The result colors after convolution
  float rtot = 0.0, gtot = 0.0, btot = 0.0;
  
  // Offset value to calculate the current pixel
  int offset = msize / 2;
  
  for (int i = 0; i < msize; i++) {
    for (int j = 0; j < msize; j++) {
      int xloc, yloc, loc;
      xloc = x + i - offset;
      yloc = y + j - offset;
      
      // Location mapping from 2D to 1D: loc = x + w * y
      // Also make sure we dont jump off the pixels array
      loc = xloc + (img.width * yloc);
      loc = constrain(loc, 0, img.pixels.length-1);
      
      // Calculate the convolution, i.e. sum all the neighbouring pixels
      // multiplied by the values in the parameter matrix
      rtot += (red(img.pixels[loc]) * m[i][j]);
      gtot += (green(img.pixels[loc]) * m[i][j]);
      btot += (blue(img.pixels[loc]) * m[i][j]);
    }
  }
  // Constrain the values and add 0.5 to fix the rounding error
  // when calling the color() constructor
  rtot = constrain(rtot, 0, 255) + 0.5;
  gtot = constrain(gtot, 0, 255) + 0.5;
  btot = constrain(btot, 0, 255) + 0.5;
  
  return color(rtot, gtot, btot);
}

void setup() {
  size(WINDOW_W*2, WINDOW_H);
  
  // Load the font
  font = loadFont("Candara-25.vlw");
  
  // The comparison, "original", image
  comparison = loadImage("sunflower.jpg");
  
  // The images needed for the filtering
  source = loadImage("sunflower.jpg");
  dest = loadImage("sunflower.jpg");
  
  background(0);
}

void mousePressed() {
  if (mouseButton == LEFT) PAUSE = !PAUSE;
  
  if (mouseButton == RIGHT) {
    source.loadPixels();
    dest.loadPixels();
    comparison.loadPixels();
    
    dest.pixels = comparison.pixels.clone();
    source.pixels = comparison.pixels.clone();
    iteration = 1;
    PAUSE = false;
    
    source.updatePixels();
    dest.updatePixels();
  }
}

// Pause and iteration status
boolean PAUSE = false;
int iteration = 1;

void draw() {
  
  try {
    if (!PAUSE) {
      background(0);
      
      source.loadPixels();
      dest.loadPixels();
      
      // Run each pixel through the convolution filter
      for (int x = 0; x < dest.width; x++) {
        for (int y = 0; y < dest.height; y++) {
          color c = cfilter(x, y, bmatrix, bmatrix.length, source);
          int loc = x + (y * dest.width);
          dest.pixels[loc] = c;
        }
      }
      
      dest.updatePixels();
      source.pixels = dest.pixels.clone();
      source.updatePixels();
      
      image(dest, 0, 0);
      image(comparison, WINDOW_W, 0);
      
      textFont(font, 20);
      textAlign(LEFT);
      text("Filter iteration: " + iteration++, 4, 18);
    }
  }
  catch (Throwable t) {
    println("Message: " + t.getMessage());
    println("Cause: " + t.getCause());
    println("Stack trace:");
    t.printStackTrace();
  }
}
