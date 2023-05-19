Colors colors;
Pointer pointer;



void setup(){
  
  size(1000,700);  
  colors = new Colors(); 
  pointer = new Pointer();
 
}

void draw(){
  
  background(0);
  fill(255);
  ellipse(500,350,600,600); // Draw the circle


  // Draw eight lines inside the ellipse
  for (int i = 0; i < 8; i++) {
    float startAngle = i * PI / 4; // Starting angle of each slice

    float x1 = 500; // X-coordinate of the ellipse's center
    float y1 = 350; // Y-coordinate of the ellipse's center

    float x2 = 500 + cos(startAngle) * 300; // Starting x-coordinate of the line
    float y2 = 350 + sin(startAngle) * 300; // Starting y-coordinate of the line

    //stroke(0);
    //strokeWeight(0.7);
    //line(x1, y1, x2, y2);

    float cx = (x1 + x2) / 2; // X-coordinate of the small circle
    float cy = (y1 + y2) / 2; // Y-coordinate of the small circle (adjusted for positioning)

    
    float smallCircleRadius = 100; // Radius of the small circle
    float angleMid = startAngle + 9*PI / 32 ; // Middle angle of the slice
    float smallCircleX = cx + cos(angleMid) * smallCircleRadius;
    float smallCircleY = cy + sin(angleMid) * smallCircleRadius;

    noStroke();
    colorMode(HSB,360,100,100);
    color c = colors.getColor(i);
    fill(c);
    stroke(0);
    strokeWeight(5);
    ellipse(smallCircleX, smallCircleY, 60, 60); // Draw the small circle
  }
  
  // serve solo per i test
  float h_ran = random(360);
  float s_ran = random(100);
  float b_ran = random(100);
  
  // bisogna mandargli in ingresso i 3 valori da juce, la funzione draw() si chiama da sola in loop
  pointer.updatePointer(h_ran, s_ran, b_ran);
     
  println(h_ran,s_ran,b_ran);
  
  // serve per aspettare un attimo tra un colore e l'altro 
  //( al momento ci mette un attimo anche a caricare lo sfondo, poi sarà da togliere e la pallina cambierà colore e posizione quando le arriverà l'input da juce)
  delay(400);
  
}
