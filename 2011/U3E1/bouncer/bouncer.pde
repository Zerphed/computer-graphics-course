int WINDOW_W = 860;  // Window width
int WINDOW_H = 640;  // Window height
int NTILES = 4;      // Number of tiles
int PENALTY = 200;   // New tile spawn time

// Initialize the BouncyBall and tiles
BouncyBall ball = new BouncyBall(10, WINDOW_W/4, WINDOW_H/2);
ArrayList tiles = new ArrayList();

class BouncyBall {
  
  private int radius, xpos, ypos, xdir, ydir;
  private float xspeed, yspeed;
  
  public BouncyBall(int r, int xpos, int ypos, float xspeed, float yspeed) {
    this.radius = r;
    
    this.xpos = xpos;
    this.ypos = ypos;
    this.xspeed = xspeed;
    this.yspeed = yspeed;
    
    // Directions are initialized always downwards
    this.xdir = 1;
    this.ydir = 1;
  }

  public BouncyBall(int r, int xpos, int ypos) {
    this.radius = r;
    
    this.xpos = xpos;
    this.ypos = ypos;
    
    // Give some arbitrary values to these
    this.xspeed = 5.4;
    this.yspeed = 4.0;
    
    // Directions are initialized always downwards
    this.xdir = 1;
    this.ydir = 1;
  }
  
  int getX() {
    return this.xpos;
  }
  
  int getY() {
    return this.ypos;
  }
  
  float getYspeed() {
    return this.yspeed;
  }
  
  void setYspeed(float ys) {
    this.yspeed = ys;
  }
  
  int getRadius() {
    return this.radius;
  }
  
  void update() {
    // Update the position according to the speed
    this.xpos = (int)(this.xpos + (this.xspeed * this.xdir));
    this.ypos = (int)(this.ypos + (this.yspeed * this.ydir));
    
    // Test to see if the ball is within the boundaries of the
    // program window
    if (this.xpos > WINDOW_W - this.radius || this.xpos < this.radius)
      this.xdir *= -1; // reverse the x direction
    if (this.ypos > WINDOW_H - this.radius ||this.ypos < this.radius)
      this.ydir *= -1; // reverse the y direction
  }
  
  void draw() {
    // Draw the shape
    ellipse(this.xpos, this.ypos, this.radius*2, this.radius*2);
  }
  
}

class Tile {
  private float xpos, ypos, w, h;
  
  public Tile(float xpos, float ypos, float w, float h) {
    this.xpos = xpos;
    this.ypos = ypos;
    this.w = w;
    this.h = h;
  }  
  
  public boolean hit() {
    // Calculate the ball information
    int bx = ball.getX();
    int by = ball.getY();
    int br = ball.getRadius();
    
    float b_dist_x = abs(bx - this.xpos);
    float b_dist_y = abs(by - this.ypos);
    
    if (b_dist_x > (this.w/2 + br) || b_dist_y > (this.h/2 + br)) 
      return false;
    
    if (b_dist_x <= (this.w/2) || b_dist_y <= (this.h/2))
      return true;
      
    float corner_distance_sq = (int)(b_dist_x - this.w/2) << 1 + ((int)(b_dist_y - this.h/2) << 1);
    return (corner_distance_sq <= (br << 1));
  }
  
  public void bounce() {
    ball.setYspeed(ball.getYspeed() * -1);
  }
  
  public void draw() {
    rectMode(CENTER);
    rect(xpos, ypos, w, h);
  }
  
}

void setup() {
  size(WINDOW_W, WINDOW_H);
  noStroke();
  smooth();
  // Push the tiles into the tiles array
  for (int i = 0; i < NTILES; i++)
    tiles.add(new Tile(random(WINDOW_W - 150), random(WINDOW_H - 30), 120, 15));
}

void draw() {

  background(102);
  
  // Update the ball's state and call it's draw method.
  ball.update();
  ball.draw();
  
  for (int i = 0; i < tiles.size(); i++) {
    Tile current = ((Tile)tiles.get(i));
    if (!current.hit())
      current.draw();
    else {
      current.bounce();
      tiles.remove(i);
    }
  }
  
  if (tiles.size() < NTILES) {
    if (PENALTY <= 0) {
      tiles.add(new Tile(random(WINDOW_W - 150) + 75, random(WINDOW_H - 30) + 15, 120, 15));
      PENALTY = 200;
    }
    else
      PENALTY--;
  }

}
