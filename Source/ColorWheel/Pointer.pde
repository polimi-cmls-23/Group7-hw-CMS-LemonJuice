class Pointer{
  int centerX = 500;
  int centerY = 350;
  float hue, sat, bri;
  color c;
  float hueRadians, volume, x, y;
  float position;


  public void updatePointer(float hue, float sat, float bri){
    
    hueRadians = radians(hue);
    x = centerX + cos(hueRadians) * sat;
    y = centerY + sin(hueRadians) * sat;
    volume = map(bri, 0,100,10,60);
    
    colorMode(HSB, 360,100,100);
    c = color(hue,sat,bri);
    fill(c);
    noStroke();
    ellipse(x,y,volume,volume);
  }
}
