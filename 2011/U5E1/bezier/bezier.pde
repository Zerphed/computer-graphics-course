final int WINDOW_W = 640;
final int WINDOW_H = 480;
final int P_RADIUS = 10;

ArrayList BPoints;
ArrayList BSplines;

boolean hide = false;

class BPoint {  
    
  private PVector APosition;  // Anchor point position
  private PVector CPosition;  // Control point position
  private boolean mover;      // Mover indicator for the control point
  
  BPoint(float ax, float ay) {
    // The anchor position is the mouse click position
    this.APosition = new PVector(ax, ay);
    
    // The control point position is either straight to left or right from anchor
    float cx = ax+15 < WINDOW_W ? ax+30 : ax-30;
    this.CPosition = new PVector(cx, ay);
    
    // Initially the mouse is NOT over the control point
    this.mover = false;
  }
  
  float ax() {
    return this.APosition.x;
  }
  
  float ay() {
    return this.APosition.y;
  }
  
  float cx() {
    return this.CPosition.x;
  }
  
  float cy() {
    return this.CPosition.y;
  }
  
  boolean mover() {
    return this.mover;
  }
  
  void updateAndDraw() {
    // Draw the anchor point in red
    fill(#F21120);
    ellipse(this.APosition.x, this.APosition.y, P_RADIUS, P_RADIUS);
    
    // Draw the control point 
    PVector mousePos = new PVector(mouseX, mouseY);
    this.mover = this.CPosition.dist(mousePos) < P_RADIUS ? true : false;
    
    float fillColor = this.mover ? 100 : 255;
    fill(fillColor);
    ellipse(this.CPosition.x, this.CPosition.y, P_RADIUS, P_RADIUS);
    
    // Draw a line in between the two
    if (!hide)
      line(this.APosition.x, this.APosition.y, this.CPosition.x, this.CPosition.y);
    
  }
  
  BPoint mirror() {
    PVector dir = PVector.sub(this.CPosition, this.APosition);
    dir.normalize();
    dir.mult(-1);
    PVector mirror = new PVector(this.APosition.x + (dir.x * this.APosition.dist(this.CPosition)), this.APosition.y + (dir.y * this.APosition.dist(this.CPosition)));
    
    BPoint ret = new BPoint(this.APosition.x, this.APosition.y);
    ret.CPosition = mirror;
    return ret;
  }
  
  // Drags the point, called only if mover is true
  void drag()
  {
    this.CPosition.x = mouseX;
    this.CPosition.y = mouseY;
  }
}

class BSpline {

  private BPoint bp1, bp2;

  BSpline(BPoint bp1, BPoint bp2) {
    this.bp1 = bp1;
    this.bp2 = bp2;
  }
  
  BPoint getBp1() {
    return this.bp1;
  }
  
  BPoint getBp2() {
    return this.bp2;
  }
  
  void setBp1(BPoint bp) {
    this.bp1 = bp;
  }
  
  void setBp2(BPoint bp) {
    this.bp2 = bp;
  }
  
  void draw() {
    noFill();
    bezier(this.bp1.ax(), this.bp1.ay(), this.bp1.cx(), this.bp1.cy(), this.bp2.cx(), this.bp2.cy(), this.bp2.ax(), this.bp2.ay());
    bp1.updateAndDraw();
    bp2.updateAndDraw();
  }
  
  boolean mover() {
    return bp1.mover() || bp2.mover();
  }
  
  BPoint drag() {
    if (bp1.mover()) {
      bp1.drag();
      return this.bp1;
    }
    else {
      bp2.drag();
      return this.bp2;
    }
  }
}


void setup() {
  size(WINDOW_W, WINDOW_H, JAVA2D);
  BPoints = new ArrayList();
  BSplines = new ArrayList();
}

void draw() {
  try {
    background(255);
    
    for (int i = 0; i < BSplines.size(); i++)
      ((BSpline)BSplines.get(i)).draw();
    
    if (BPoints.size() == 1)
      ((BPoint)BPoints.get(BPoints.size()-1)).updateAndDraw();
  } 
  catch (Throwable t) {
    exit();
  }
}

void mouseClicked() {
  //if (mouseButton == LEFT) {
    BPoints.add(new BPoint(mouseX, mouseY));
    if (BPoints.size() == 2)
      BSplines.add(new BSpline((BPoint)BPoints.get(BPoints.size()-2), (BPoint)BPoints.get(BPoints.size()-1)));
    else if (BPoints.size() > 2)
      BSplines.add(new BSpline(((BPoint)BPoints.get(BPoints.size()-2)).mirror(), (BPoint)BPoints.get(BPoints.size()-1)));
  //}
  /*
  else if (mouseButton == RIGHT) {
    BSpline currentSpline;
    for (int i = 0; i < BSplines.size(); i++) {
      currentSpline = (BSpline)BSplines.get(i);
      if (currentSpline.mover())
        BSplines.remove(i);
    }
    BPoint currentPoint;
    for (int i = 0; i < BPoints.size(); i++) {
      currentPoint = (BPoint)BPoints.get(i);
      if (currentPoint.mover()) {
        BPoints.remove(i);
      }
    }
  }
  else if (mouseButton == CENTER) {
    BPoints.clear();
    BSplines.clear();
  }
  */
}

void keyPressed() {
  if (keyPressed && key == 'h')
    hide = !hide;
}

void mouseDragged() {
  if (mouseButton == LEFT) {
    BSpline current, previous, next;
    
    for (int i = 0; i < BSplines.size(); i++) {
      current = (BSpline)BSplines.get(i);
      if (current.mover()) {
        BPoint dragged = current.drag();
        if (i - 1 >= 0) {
          previous = (BSpline)BSplines.get(i-1);
          if (previous.getBp2().ax() == dragged.ax() && previous.getBp2().ay() == dragged.ay())
            previous.setBp2(dragged.mirror());
        }
        if (i + 1 < BSplines.size()) {
          next = (BSpline)BSplines.get(i+1);
          if (next.getBp1().ax() == dragged.ax() && next.getBp1().ay() == dragged.ay())
            next.setBp1(dragged.mirror());
        }
        break;
      }
    }
    
    if (BPoints.size() == 1)
      if (((BPoint)BPoints.get(0)).mover())
        ((BPoint)BPoints.get(0)).drag();
  } 
}
