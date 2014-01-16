import processing.opengl.*;

final int WINDOW_W = 860;  // Window width
final int WINDOW_H = 640;  // Window height

final int TILE_W = 60;     // Tile width
final int TILE_H = 12;     // Tile height

Level lvl;                 // The current level

boolean finished = false;
boolean pause = false;     // Pause status
PFont fontA;               // Font for texts

public class Level {
 
  private BouncyBall ball;
  private Paddle pad;
  private ArrayList tiles;
  private int ntiles;
  
  private int pts;
  private String name;
  
  // Initializes the level
  public Level(String name, int ntiles) {
    
    // Initialize the BouncyBall, Paddle, tiles array
    this.ball = new BouncyBall(5, WINDOW_W/2, WINDOW_H/2 + 15);
    this.pad = new Paddle(WINDOW_W/2, WINDOW_H/2, 80, 12);
    this.tiles = new ArrayList();
    this.ntiles = ntiles;
    this.pts = 0;
    this.name = name;
    
    // Push the tiles into the tiles array
    int r = 255, g = 255, b = 255;
    for (int i = 0; i < this.ntiles; i++) {
      if (i%9 == 0) {
        r = (int)random(255);
        g = (int)random(255);
        b = (int)random(255);
      }
      this.tiles.add(new Tile(i%9 * (TILE_W + 30) + 70, (((i+9)/9) * 30) + 60, TILE_W, TILE_H, r, g, b));
    }
  }
  
  BouncyBall ball() {
    return this.ball;
  }
  
  // This method is invoked when all the tiles are gone
  void win() {
    background(0, 0, 0);
    textAlign(CENTER);
    fill(255, 0, 0);
    textFont(fontA, 50);
    text("CONGRATULATIONS\nYOU WIN!", WINDOW_W/2, WINDOW_H/2, 1);
    finished = true;
  }
  
  // This method is invoked when the ball missed the pad
  void lose() {
    background(0, 0, 0);
    textAlign(CENTER);
    fill(255, 0, 0);
    textFont(fontA, 50);
    text("GAME OVER", WINDOW_W/2, WINDOW_H/2, 1);
    finished = true;
  }
  
  void increaseDifficulty() {
    lvl.ball().setXspeed(lvl.ball().getXspeed() * 1.10);
    lvl.ball().setYspeed(lvl.ball().getYspeed() * 1.10);
  }
  
  void drawLvlInfo() {
    // Draw the points
    textAlign(LEFT);
    textFont(fontA, 12);
    fill(255, 255, 255);
    text(this.name + "\nPoints: " + this.pts + "\nTime: " + (millis()/1000 - 1) + " s", 5, 20);
  }
  
  void draw() {
    background(0, 0, 0);
  
    // Draw and update the tiles
    for (int i = 0; i < tiles.size(); i++) {
      Tile current = ((Tile)tiles.get(i));
  
      if (current.hit()) {
        tiles.remove(i); // remove the tile
        this.pts += 100;      // increase the score
        break;           // break the loop, no more bounces on this round
      }
      else
        current.draw();
    }
    
    // Check whether the pad was hit
    this.pad.hit();
    
    // Draw the pad and ball
    this.pad.draw();
    
    // Draw the level information
    this.drawLvlInfo();
    
    // Increase difficulty every 10 seconds
    if (millis()%10000 < 15) this.increaseDifficulty();
    
    this.ball.draw();
    if (this.tiles.size() == 0) win();
  }
}

public class BouncyBall {
  
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
    this.xspeed = 0;
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
  
  int getRadius() {
    return this.radius;
  }
  
  float getXspeed() {
    return xspeed;
  }
  
  void setXspeed(float xspeed) {
    this.xspeed = xspeed;
  }
  
  float getYspeed() {
    return yspeed;
  }
  
  void setYspeed(float yspeed) {
    this.yspeed = yspeed;
  }  
  
  void bounce(float angle, int xpos, int ypos) {
    float c = cos(angle / 180 * PI);
    float s = sin(angle / 180 * PI);
    float temp = this.xspeed * c + this.yspeed * s;
    
     if (temp > 0) {
      temp = sqrt(this.xspeed * this.xspeed + this.yspeed * this.yspeed);
      this.xspeed = (this.xpos - xpos) * temp;
      this.yspeed = (this.ypos - ypos) * temp;
      temp = sqrt((this.xpos - xpos) * (this.xpos - xpos) + (this.ypos - ypos) * (this.ypos - ypos));
      this.xspeed /= temp;
      this.yspeed /= temp;
      return;
    }
    
    temp = this.xspeed * (s * s - c * c) + this.yspeed * 2 * c * s;
    this.yspeed = this.xspeed * 2 * c * s + this.yspeed * (c * c - s * s);
    this.xspeed = temp;
  }
  
  void draw() {
    // Update the position according to the speed
    this.xpos = (int)(this.xpos + (this.xspeed * this.xdir));
    this.ypos = (int)(this.ypos + (this.yspeed * this.ydir));
    
    // Test to see if the ball is within the boundaries of the
    // program window
    if (this.xpos > WINDOW_W - this.radius) 
      this.xspeed = -abs(this.xspeed);
    if(this.xpos < this.radius)
      this.xspeed = abs(this.xspeed);
    if (this.ypos > WINDOW_H - this.radius) {
      lvl.lose();
      return;
    }
    if (this.ypos < this.radius)
      this.yspeed = abs(this.yspeed);
    
    // Draw the shape
    fill(255, 255, 255);
    ellipse(this.xpos, this.ypos, this.radius * 2, this.radius * 2);
  }
  
}

public abstract class BouncyRectangle {
  
  protected int xpos, ypos, w, h;
  protected int r, g, b;
  
  public BouncyRectangle(int xpos, int ypos, int w, int h, int r, int g, int b) {
    this.xpos = xpos;
    this.ypos = ypos;
    this.w = w;
    this.h = h;
    
    this.r = r;
    this.g = g;
    this.b = b;
  }  

  public boolean hit() {
    
    // Get the ball radius
    int bx = lvl.ball().getX();
    int by = lvl.ball().getY();
    int br = lvl.ball().getRadius();
    
    // Calculate the ball's distance in x and y axis.
    int b_dist_x = abs(bx - this.xpos);
    int b_dist_y = abs(by - this.ypos);
    
    // Check whether a hit occured, a hit occured if the method
    // doesn't stop within the next 3 ifs
    if (b_dist_x > (this.w/2 + br) || b_dist_y > (this.h/2 + br)) return false;
    if (!(b_dist_x <= (this.w/2) || b_dist_y <= (this.h/2))) return false;
      
    int corner_distance_sq = (b_dist_x - this.w/2) << 1 + (b_dist_y - this.h/2) << 1;
    if (!(corner_distance_sq <= (br << 1))) return false;
    
    this.bounce();
    return true;
  }
  
  public abstract void bounce();
  public abstract void draw();
  
}

public class Tile extends BouncyRectangle {
  
  public Tile(int xpos, int ypos, int w, int h, int r, int g, int b) {
    super(xpos, ypos, w, h, r, g, b);
    
  }
  
  public void draw() {
    fill(r, g, b);
    rect(xpos, ypos, w, h);
  }
  
  public void bounce() {
    lvl.ball().setYspeed(lvl.ball().getYspeed() * -1);
  }
  
}

public class Paddle extends BouncyRectangle {
  
 public Paddle(int xpos, int ypos, int w, int h) {
   super(xpos, ypos, w, h, 255, 255, 255);
 }
  
 void draw() {
   // Draw method takes also care of updating
   this.xpos = mouseX;
   this.ypos = WINDOW_H - 20;
   
   fill(255, 255, 255);
   rect(mouseX, WINDOW_H - 20, w, h);
 }
 
 void bounce() {
   // Get the ball radius
   int bx = lvl.ball().getX();
   int by = lvl.ball().getY();
   int br = lvl.ball().getRadius();
    
   // Calculate the ball's distance in x and y axis.
   int b_dist_x = abs(bx - this.xpos);
   int b_dist_y = abs(by - this.ypos);
   
   // A hit occured, find the area
   int[] angle = new int[] {135, 90, 45, 180, 0, 0, 225, 270, 325};
  
   int hor, ver;
   if (bx > this.xpos + this.w/2)       //7,3,8
     hor = 2;
   else if (bx < this.xpos - this.w/2)  //5,4,6
     hor = 0;
   else                                 //1,9,2
     hor = 1;
    
   if (by > this.ypos + this.h/2)       //6,2,8
     ver = 2;
  
   else if (by < this.ypos - this.h/2)  //5,1,7
     ver = 0;
   else                                 //4,9,3
     ver = 1;
    
   if (ver==1 && hor==1)
     if (b_dist_x/w > b_dist_y/h)
       if (bx > this.xpos)
         hor = 2;
       else
         hor = 0;
     else
       if(by > this.ypos)
         ver = 2;
       else
         ver = 0;
         
   lvl.ball().bounce(angle[ver*3+hor], this.xpos, this.ypos);
 }
 
}

void mousePressed() {
  
  if (mouseButton == RIGHT) {
    lvl = new Level("Introduction", 72);
    finished = false;
  }
  
  if (mouseButton == LEFT) pause = !pause;
}

void setup() {
  
  // Initialize the screen and necessary modes
  size(WINDOW_W, WINDOW_H, OPENGL);
  noStroke();
  smooth();
  rectMode(CENTER);
  
  fontA = loadFont("Ziggurat-HTF-Black-32.vlw");
  lvl = new Level("Introduction", 72);
}

void draw() {
  if (!pause && !finished)
    lvl.draw();
}
