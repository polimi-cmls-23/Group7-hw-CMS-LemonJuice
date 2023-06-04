class Pointer{
  int centerX = 850;
  int centerY = 390;
  float hue, sat, bri;
  color c;
  float hueRadians, volume, x, y, saturation;
  float position;
  float[] hsv_colors = {0,0,0};


  public void updatePointer(float hue, float sat, float bri){
    
    // Convert hue value to radians
    hueRadians = radians(hue);
    
    // Map the saturation value from the range [0, 100] to [0, 205] (205 is the distance from the center of the ColorWheel to the center of the circles)
    saturation = map(sat,0,100,0,205);
    
    // Calculate the X and Y coordinates of the pointer center
    x = centerX + cos(hueRadians) * saturation;
    y = centerY + sin(hueRadians) * saturation;
    
    // Map the brightness value from the range [0, 100] to [10, 50] ([10,50] are the max and min diameter of the pointer)
    volume = map(bri, 0,100,10,50);
    
    colorMode(HSB, 360,100,100);
    // Create a color using the hue, saturation, and brightness values
    c = color(hue,sat,bri);
    fill(c);
    strokeWeight(5);
    stroke(0);
    ellipse(x,y,volume,volume);
  }
  
  public float[] rgbToHsb(int r, int g, int b) {
    float h=0, s=0, br;
    
    // Find the maximum value among r, g, and b
    float max = max(r, max(g, b));
    
    // Find the minimum value among r, g, and b
    float min = min(r, min(g, b));
    
    // Calculate the brightness value as a percentage
    br = max * 100 / 255.0;
    
    float delta = max - min;
   
    
    if (max != 0) {
      // Calculate the saturation value as a percentage
      s = delta * 100 / max;
      
      // Calculate the hue value based on the position of the maximum value
      if (r == max) {
        h = (g - b) / delta;
      } else if (g == max) {
        h = 2 + (b - r) / delta;
      } else {
        h = 4 + (r - g) / delta;
      }
      
      // Convert hue to degrees
      h *= 60;
      
      if (h < 0){
        // Wrap hue value if it is negative
        h += 360;
      }
      
      if (delta == 0){
        // Set hue to 0 if the difference is 0, i.e., all RGB values are the same
        h = 0;
       }
    
     }
    
    hsv_colors[0] = h;
    hsv_colors[1] = s;
    hsv_colors[2] = br;
    
    
    return hsv_colors;
  }
}
