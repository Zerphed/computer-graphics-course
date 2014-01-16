int NUM_OF_ELLIPSES = 15; // Number of ellipses
int SCREEN_W = 860;       // Screen width
int SCREEN_H = 640;       // Screen height

ArrayList ellipses;       // Container to hold the objects.

// A class representing draggable ellipse
class draggableEllipse 
{
  boolean dragging = false;  // a) Is the object being dragged
  boolean mover = false;     // b) Is the mouse over the ellipse
  
  float x, y, w, h;          // Location and size of the ellipse
  int r, g, b;               // The ellipse's rgb colours
  float offsetX, offsetY;    // Mouseclick offset
  
  // Constructor of the draggableEllipse class
  draggableEllipse(float x, float y, float w, float h, int r, int g, int b) 
  {
    this.x = x; this.y = y;
    this.w = w; this.h = h;
    this.r = r; this.g = g; this.b = b;    
    this.offsetX = 0; this.offsetY = 0;
  }
  
  // Display method
  void display()
  {
    stroke(0);
    if (dragging) fill(50);                     // Change the fill to indicate a drag
    else if (mover) fill(100);                  // Change the fill to indicate a mover
    else fill(this.r, this.g, this.b);          // Default fill
    ellipse(this.x, this.y, this.w, this.h);    // Create the ellipse
  }
  
  void clicked(int mx, int my)
  {
    // Let's assume that the ellipse is a circle
    boolean within_x = ((mx <= this.x + (this.w/2)) && mx >= (this.x - (this.w/2)));
    boolean within_y = ((my <= this.y + (this.w/2)) && my >= (this.y - (this.w/2)));
    
    if (within_x && within_y)
      this.dragging = true;
      
      this.offsetX = x - mx;
      this.offsetY = y - my;
  }
  
  // A method to check whether the mouse is inside the ellipse
  // @param: mouse x-coord, mouse y-coord
  void mover(int mx, int my)
  {
    // Let's assume that the ellipse is a circle
    boolean within_x = ((mx <= this.x + (this.w/2)) && mx >= (this.x - (this.w/2)));
    boolean within_y = ((my <= this.y + (this.w/2)) && my >= (this.y - (this.w/2)));
    
    if ( within_x && within_y )
      this.mover = true;
    else
      this.mover = false;
  }
    
  void stopDragging()
  {
    this.dragging = false;
  }
  
  // A coarse collision detection
  // @param: new x-coordinate, new y-coordinate
  // @return: boolean value true if collides, false if not
  boolean checkCollisions(float new_x, float new_y)
  {
    boolean within_x, within_y;

    for (int i = 0; i < ellipses.size(); i++)
    {
      draggableEllipse current = (draggableEllipse) ellipses.get(i);
      
      // Check if the current object is in fact this object
      if (current.x == this.x && current.y == this.y) continue;
      
      float t_radius = this.w/2;     // This object's radius
      float c_radius = current.w/2;  // Current object's radius
      
      // Initialize the boolean values
      within_x = new_x + t_radius >= current.x - c_radius && new_x - t_radius <= current.x + c_radius;
      within_y = new_y + t_radius >= current.y - c_radius && new_y - t_radius <= current.y + c_radius;
      
      // If the coordinates were within another circle stop dragging and break the loop
      if (within_x && within_y)
        return true;
    }
    return false;
  }
  
  void drag(int mx, int my)
  {
    float new_x = mx + offsetX;
    float new_y = my + offsetY; 

    if (this.checkCollisions(new_x, new_y))
        this.dragging = false;
    
    if (dragging)
    {
      this.x = new_x;
      this.y = new_y;
    }
  }
  
  void setCoordinates(float new_x, float new_y)
  {
    this.x = new_x;
    this.y = new_y;
  }
}

// Time to take the new class out for a spin
void setup() 
{
  ellipses = new ArrayList();
  size(SCREEN_W, SCREEN_H);
  smooth();

  // Create random ellipses which do not overlap
  for (int i = 0; i < NUM_OF_ELLIPSES; i++)
  {
    float x_coord = random(SCREEN_W);
    float y_coord = random(SCREEN_H);
    float rad = random(100);
    
    draggableEllipse current = new draggableEllipse(x_coord, y_coord, rad, rad, int(random(255)), int(random(255)), int(random(255)));
    
    while (current.checkCollisions(x_coord, y_coord))
    {
      x_coord = random(SCREEN_W);
      y_coord = random(SCREEN_H);
      current.setCoordinates(x_coord, y_coord); 
    }
    
    ellipses.add(current);
  }
}

void draw() 
{
  background(255, 120, 20, 180);
  
  for (int i = 0; i < ellipses.size(); i++)
  {
    draggableEllipse current = (draggableEllipse) ellipses.get(i);
    current.mover(mouseX, mouseY);
    current.drag(mouseX, mouseY);
    current.display();
  }
}

void mousePressed() 
{
  for (int i = 0; i < ellipses.size(); i++)
  {
    draggableEllipse current = (draggableEllipse) ellipses.get(i);
    current.clicked(mouseX, mouseY);
  }
}

void mouseReleased() 
{
  for (int i = 0; i < ellipses.size(); i++)
  {
    draggableEllipse current = (draggableEllipse) ellipses.get(i);
    current.stopDragging();
  }
}
