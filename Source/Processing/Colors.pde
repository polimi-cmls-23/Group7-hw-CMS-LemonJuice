class Colors{
 
  public color getColor(int i, int numCol) {
    
    // Returns the color of each instrument circle based on its angular position
    int angle = 360/numCol;
    colorMode(HSB, 360, 100, 100);
    float hue = i*angle;
    return color(hue,100,100);
    
  }
}
