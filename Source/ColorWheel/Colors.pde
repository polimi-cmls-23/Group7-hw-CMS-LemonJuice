class Colors{
 
  public color getColor(int i) {
    
    colorMode(HSB, 360, 100, 100);
    if (i == 0) {
      return color(0, 100, 100);
    } else if (i == 1) {
      return color(45, 100, 100);
    } else if (i == 2) {
      return color(90, 100, 100);
    } else if (i == 3) {
      return color(135, 100, 100);
    } else if (i == 4) {
      return color(180, 100, 100);
    } else if (i == 5) {
      return color(225, 100, 100);
    } else if (i == 6) {
      return color(270, 100, 100);
    } else if (i == 7) {
      return color(315, 100, 100);
    } else {
      return color(0);
    }
  }
}
