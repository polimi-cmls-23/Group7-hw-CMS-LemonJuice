import processing.serial.*; // Provides functionality for communication with serial devices
import oscP5.*;  // Used for implementing Open Sound Control (OSC) communication in Processing
import netP5.*;  //  Works in conjunction with the oscP5 library and provides networking capabilities for OSC communication
import controlP5.*; // Provides buttons and Sliders 
import g4p_controls.*; // Provides  dropdown menu

// Classes init
Colors colors;
Pointer pointer;

// Serial init
Serial myPort;

//Button, sliders and dropdown initialization
ControlP5 cp5;
Button button;
Button stopB;
Slider sliderR;
Slider sliderG;
Slider sliderB;
ScrollableList dropdown;
ScrollableList dropPort;


// RGB values initialization
int[] rgb_colors = {0,0,0};
float[] hsv_colors = {0,0,0};

boolean serialOn = true;
boolean defaultCol = false;
boolean slidUpdated = false;
boolean initPointer = false;
boolean portSelected = false;
boolean updR = true;
boolean updG = true;
boolean updB = true;
boolean portBusy = true;
int index = 0;
int chosenPort = 0;
int nrColors = 5;
int winWidth = 1150;
int winHeight = 800;


// OSC initialization
OscP5 oscP5;
NetAddress myRemoteLocation;


void setup(){
  
  // Set the size of the window
  size(1150,800);
  
  // Creates a new instance of the Colors class
  colors = new Colors();  
  
  // Creates a new instance of the Pointer class
  pointer = new Pointer();
  
  // Creates a new instance of the ControlP5 class
  cp5 = new ControlP5(this);
  
  // Mode selection button
  button = cp5.addButton("button")
           .setPosition(75,50)
           .setSize(150,100)
           .setCaptionLabel("Sensor Mode On")
           .setFont(createFont("Arial", 15))
           .setColorForeground(color(219, 34, 13))
           .setColorBackground(color(48, 210, 75));
  
  // Button to disconnect the port
  stopB = cp5.addButton("stopB")
          .setPosition(1000,80)
          .setSize(100,40)
          .setColorBackground(color(170,170,170))
          .setColorForeground(color(141,135,135))
          .setColorActive(color(141,135,135))
          .setCaptionLabel("Disconnect");
        
        
  // Slider for the R value
  sliderR = cp5.addSlider("sliderR")
            .setPosition(75,250)
            .setSize(450,75)
            .setRange(0, 255)
            .setCaptionLabel("R")
            .setFont(createFont("Arial",25))
            .setColorForeground(color(189,21,2))
            .setColorBackground(color(255,0,0))
            .setValue((float)rgb_colors[0])
            .setColorActive(color(189,21,2))
            .setLock(true);
            
  // Slider for the G value          
  sliderG = cp5.addSlider("sliderG")
            .setPosition(75,425)
            .setSize(450,75)
            .setRange(0, 255)
            .setCaptionLabel("G")
            .setFont(createFont("Arial",25))
            .setColorForeground(color(2,146,41))
            .setColorBackground(color(0,255,0))
            .setValue((float)rgb_colors[1])
            .setColorActive(color(2,146,41))
            .setLock(true);
            
  // Slider for the B value          
  sliderB = cp5.addSlider("sliderB")
            .setPosition(75,600)
            .setSize(450,75)
            .setRange(0, 255)
            .setCaptionLabel("B")
            .setFont(createFont("Arial",25))
            .setColorForeground(color(13,3,144))
            .setColorBackground(color(0,0,255))
            .setValue((float)rgb_colors[2])
            .setColorActive(color(13,3,144))
            .setLock(true);
            
 // Create dropdwon menu for default colors
   dropdown = cp5.addScrollableList("dropdown")
             .setPosition(255,50)
             .setSize(260,500)
             .setItemHeight(40)
             .setBarHeight(100)
             .setCaptionLabel("Select a default color")
             .setFont(createFont("Arial",15))
             .setColorBackground(color(170,170,170))
             .setColorForeground(color(141,135,135))
             .setColorActive(color(141,135,135))
             .addItem("Coral", 0)
             .addItem("Turquoise", 1)
             .addItem("Violet", 2)
             .addItem("Chocolate", 3)
             .close()
             .setLock(true);
             
           
  
  // Create dropdown menu to select the input port
  dropPort = cp5.addScrollableList("dropPort")
             .setPosition(970,20)
             .setSize(150,200)
             .setBarHeight(40)
             .setColorBackground(color(170,170,170))
             .setColorForeground(color(141,135,135))
             .setColorActive(color(141,135,135))
             .setCaptionLabel("Select a port")
             .setItemHeight(30)
             .close();
             
  // Adding items to the dropdown menu with the available ports names         
  int numPort = (Serial.list()).length;
  index = numPort/2;
  int j = 0;
  
  for(int i = index; i<numPort; i++){
      dropPort.addItem(Serial.list()[i],j);
      j++;
      
  }
  
  
  // Listeners
  button.addListener(new ButtonClickListener());
  sliderR.addListener(new ButtonClickListener());
  sliderG.addListener(new ButtonClickListener());
  sliderB.addListener(new ButtonClickListener());
  dropdown.addListener(new ButtonClickListener());
  dropPort.addListener(new ButtonClickListener());
  stopB.addListener(new ButtonClickListener());
  
  // OSC messages
  oscP5 = new OscP5(this,8080); // Open a new OSC connection on port 6448
  myRemoteLocation = new NetAddress("127.0.0.1",8080); // Set the address to send OSC messages to loopback address, port 6448
  
 

}



void draw(){
  
  // Draw the color wheel
  float centerX, centerY;
  centerX = 850;
  centerY = 390;    
  int radius = 500;
  background(0);
  colorMode(RGB,255,255,255);
  fill(color(255,255,255)); 
  ellipse(centerX,centerY,radius,radius); 
  
  // Draw a circle for each instrument inside the color wheel
  
  // Calculate the angular distance from each circle 
  float posAngle = radians(360/nrColors);
  
  for (int i = 0; i < nrColors; i++) {
    // Calculate the angular position of each circle
    float angle = i * posAngle; 
    
    // Calculate the X and Y coordinate of each circle
    float smallCircleX = centerX + cos(angle) * (205);  
    float smallCircleY = centerY + sin(angle) * (205);

    
    noStroke();
    colorMode(HSB,360,100,100);
    
    // Get the color of each circle from the class Color based on the current circle
    color c = colors.getColor(i, nrColors); 
    fill(c);
    stroke(0);
    strokeWeight(3);
    ellipse(smallCircleX, smallCircleY, 50, 50); 
  }
  
    
  // Convert RGB to HSV
  hsv_colors = pointer.rgbToHsb(rgb_colors[0], rgb_colors[1], rgb_colors[2]);
  
  // Create/update the pointer in the wheel
  pointer.updatePointer(hsv_colors[0], hsv_colors[1], hsv_colors[2]);
  
  // Update the sliders value with the ones read by the sensor
  if(serialOn){
    updateSliders(rgb_colors);
  }
 
 // Port on off circle
 colorMode(RGB,255,255,255);
 if(portBusy){
   // Red if the port is busy
   fill(color(255,0,0)); 
   dropPort.setCaptionLabel("Select a port");
 }else if(!serialOn && !portBusy){
   // Yellow if the sensor is off
   fill(color(255,255,0)); 
 }else{
   // Green if the port is connected
   fill(color(0,255,0)); 
 }
  ellipse(930,40,30,30);
  
  
  // Draw the instrument color legend
  fill(color(190,190,190));
  rect(600,720,520,40);
  
  for(int i = 0; i<nrColors; i++){    
    legendMaker(i); 
  }
  
  
}

// Function to draw the legend
void legendMaker(int index){
  int x = 625 + index*(100);
  color c = colors.getColor(index, nrColors);
  String instrument;
  fill(c);
  strokeWeight(3);
  ellipse(x,740,20,20);
  
  colorMode(RGB,255,255,255);
  fill(color(0,0,0));
  textSize(18);
  
  
  if( index == 0){
    instrument = "Bass";
  }else if(index == 1){
    instrument = "Drums";
  }else if(index == 2){
    instrument = "Vocal";
  }else if(index == 3){
    instrument = "Guitar";
  }else{
    instrument = "Others";
  }
  
  int x_text = 642 + index*(100);
  text(instrument, x_text, 750);
  

}

void sliderEvent(){
    
  // Initialize droprdown  menu label
  dropdown.setCaptionLabel("Select a default color");
      
  // Send OSC messages if we're not in Sensor mode
  if(!serialOn){
    sendOsc(rgb_colors, computeVolumes(hsv_colors));
    
    // Get the values from the sliders
    if(!updR){
      rgb_colors[0] = (int) sliderR.getValue();
      updR = true;
    }else if(!updG){
      rgb_colors[1] = (int) sliderG.getValue();
      updG = true;
    }else if(!updB){
      rgb_colors[2] = (int) sliderB.getValue();
      updB = true;
    }
  }
}



void serialEvent(Serial myPort) 
{   
    if(serialOn){
      String data = myPort.readStringUntil('\n');  
  
      if (data != null) 
      {
        
        // Removes any whitespace and newline characters
        data = data.trim();
        
        // Print the received data
        println(data);   
        
        // Split the data and convert it into integers
        int[] newColors = int(split(data, ','));  
    
        // Check that we have exactly three values
        if (newColors.length == 3) {  
          portBusy = false;
          
          // Update colors if valid data is received
          rgb_colors = newColors; 
          
          // Call the function to send OSC messages
          sendOsc(rgb_colors, computeVolumes(hsv_colors));
          
          }
      }  
    }
    
}


// Function to create and send OSC messages
void sendOsc(int[] rgb_actual, float[] weights){
        // The OSC address pattern
        OscMessage myMessage = new OscMessage("/color");
        
        // Add each color value to the OSC message
        myMessage.add(rgb_actual[0]);  
        myMessage.add(rgb_actual[1]);
        myMessage.add(rgb_actual[2]);
        myMessage.add(weights[0]);
        myMessage.add(weights[1]);
        myMessage.add(weights[2]);
        myMessage.add(weights[3]);
        myMessage.add(weights[4]);
        
        // Send the OSC message
        oscP5.send(myMessage, myRemoteLocation);
        
}



class ButtonClickListener implements ControlListener {
  public void controlEvent(ControlEvent event) {
    
    //Button is clicked
    if (event.isController() && event.getController().getName().equals("button")) {
      
       colorMode(RGB,255,255,255);
       if(serialOn){
          // Switch to sensor mode off
          serialOn = false; 
          initPointer = false;
          
          // Activate sliders and dropdownmenu
          dropdown.setLock(false);
          sliderR.setLock(false);
          sliderG.setLock(false);
          sliderB.setLock(false);
          
          // Stop the port 
          if(portSelected){
            myPort.stop();
          }
          
          // Change the mode on the button label
          button.setCaptionLabel("Sensor Mode Off").setFont(createFont("Arial", 15)).setColorForeground(color(48, 210, 75)).setColorBackground(color(219, 34, 13));
          
          // Initialize pointer and sliders
          initializePointer();
          if(initPointer){
            updateSliders(rgb_colors);
            slidUpdated = false;
            defaultCol = false;
          }
         
          
          
        }else{
          // Switch to sensor mode on
          serialOn = true; 
          initPointer = false;
          defaultCol = false;
          
          // Deactivate sliders and dropdown menu
          sliderR.setLock(true);
          sliderG.setLock(true);
          sliderB.setLock(true);
          dropdown.setLock(true);
          
          // Change the mode on the button label
          button.setCaptionLabel("Sensor Mode on").setFont(createFont("Arial", 15)).setColorForeground(color(219, 34, 13)).setColorBackground(color(48, 210, 75));
          
          // Activate port
          initializeSerial();
          
          
          // Initialize pointer and sliders
          initializePointer();
          if(initPointer){
            updateSliders(rgb_colors);
            slidUpdated = false;
            defaultCol = false;
          }
          
     
        }
        
        // Initialize droprdown menu label
        dropdown.setCaptionLabel("Select a default color");
        
    }
    
    // Slider R is used 
    if (event.isController() && event.getController().getName().equals("sliderR")) {
      if(!defaultCol){
         updR = false;
        sliderEvent();
                          
      }
      
    }
    
    // Slider G is used 
    if (event.isController() && event.getController().getName().equals("sliderG")) {
      if(!defaultCol){
        updG = false;
        sliderEvent();
                 
      }
    }
    
    // Slider B is used 
    if (event.isController() && event.getController().getName().equals("sliderB")) {
      if(!defaultCol){
        updB = false;
        sliderEvent();
        
      }
    }
    
    // Dropdown menu
    if (event.isController() && event.getController().getName().equals("dropdown")) {
      
      // Get the selected value
      int selectedValue = (int) event.getValue();
      
      slidUpdated = false;
      defaultCol = true;
      
      if(selectedValue == 0){ 
        // Coral
        rgb_colors[0] = 255;
        rgb_colors[1] = 127;
        rgb_colors[2] = 80;
        
      }else if(selectedValue == 1){ 
        // Turquoise
        rgb_colors[0] = 64;
        rgb_colors[1] = 224;
        rgb_colors[2] = 208;   
        
      }else if(selectedValue == 2){ 
        // Violet
        rgb_colors[0] = 238;
        rgb_colors[1] = 130;
        rgb_colors[2] = 238;  
        
      }else if(selectedValue == 3){ 
        // Chocolate
        rgb_colors[0] = 210;
        rgb_colors[1] = 105;
        rgb_colors[2] = 30; 
        
      }
      
      updateSliders(rgb_colors);
      dropdown.close();
      
      // Convert in HSV and send the values to OSC messages
      hsv_colors = pointer.rgbToHsb(rgb_colors[0], rgb_colors[1], rgb_colors[2]);
      sendOsc(rgb_colors, computeVolumes(hsv_colors));
      defaultCol = false;

    }
    
    // DropPort menu
    if (event.isController() && event.getController().getName().equals("dropPort")) {
      //Choose the port
      chosenPort();
      
      //Open the port
      initializeSerial();

    }
    
    // Disconnect button
    if (event.isController() && event.getController().getName().equals("stopB")) {
      //Disconnect the port and initialize pointer and sliders
      myPort.stop();
      initializePointer();
       if(initPointer){
          updateSliders(rgb_colors);
          slidUpdated = false;
          defaultCol = false;
        }
      
      portBusy = true;
      println("Port disconnected");
    }
     
    
  }
}

void initializeSerial(){
    
    try{
      // Use the selected port
      myPort = new Serial(this, Serial.list()[chosenPort], 9600); 
      
      // Set a marker to trigger buffer events 
      myPort.bufferUntil('\n');  
      portSelected = true;
     
    } catch(Exception e){
      portBusy = true;  
    }
    
  
}

void chosenPort(){
  
  // Get the selected value
  int port_ind = (int)dropPort.getValue();
  int j = index;
  int[] temp={0,0,0,0,0,0,0,0,0,0};
      
  // Find the corrispondent selected value in the list of available ports
  for(int k=0; k<index; k++){
     temp[k] = j;
     j++;
        
   } 
  chosenPort = temp[port_ind];
      
}


void updateSliders(int[] rgb){
  
  // Update the sliders to the current RGB value
  sliderR.setValue(rgb[0]);
  sliderG.setValue(rgb[1]);
  sliderB.setValue(rgb[2]);
  slidUpdated = true;
  
}

void initializePointer(){
  
  // Initialize the pointer to zero
  rgb_colors[0] = 0;
  rgb_colors[1] = 0;
  rgb_colors[2] = 0;
  initPointer = true;
}

float calculateDistance(float[] actualColor_hsv, int pointIndex) {  //calculate the distance between the actual color and one of the fixed color, defined by "pointIndex"
  // Convert hsv into x,y,z
  float actual_x = actualColor_hsv[1]*cos(radians(actualColor_hsv[0]));
  float actual_y = actualColor_hsv[1]*sin(radians(actualColor_hsv[0]));
  float actual_z = actualColor_hsv[2];
  
  
   // Compute X and Y coordinate based on saturation and hue
  float fixed_x = saturation(colors.getColor(pointIndex, nrColors))*cos(radians(hue(colors.getColor(pointIndex, nrColors))));
  float fixed_y = saturation(colors.getColor(pointIndex, nrColors))*sin(radians(hue(colors.getColor(pointIndex, nrColors))));
  
  // Set z-coordinate of the fixed color as the brightness value
  float fixed_z = brightness(colors.getColor(pointIndex, nrColors));
  
  // Compute distance using Euclidean distance formula
  float distance = sqrt(pow((fixed_x-actual_x),2)+pow((fixed_y-actual_y),2)+pow((fixed_z-actual_z),2));
  
  print("Computing distance with    h: ",hue(colors.getColor(pointIndex, nrColors)),"s: ",saturation(colors.getColor(pointIndex, nrColors)),"v: ",brightness(colors.getColor(pointIndex, nrColors)), "      distance is: ",distance);
  println();
  
  return distance;
}

float[] computeVolumes(float[] actualColor_hsv){
  
  print("h: ",actualColor_hsv[0],"s: ",actualColor_hsv[1],"v: ",actualColor_hsv[2]);
  println();
  
  // Initialize arrays for distances and weights
  float[] distances = new float[nrColors];
  float[] weights = new float[nrColors];;
  
  // Initialize distances and weights arrays with default values
  for (int i = 0; i < nrColors; i++) {
    distances[i]=0;
    weights[i]=0;
  }
  
  // Initialize minimum distance as maximum possible value
  float minDist = Float.MAX_VALUE;
  
  // Initialize maximum distance as 0
  float maxDist = 0;
  
  
  // Calculate distances and find minimum and maximum distances
  for (int i = 0; i < distances.length; i++) {
    // Calculate distance between actualColor_hsv and each color
    distances[i] = calculateDistance(actualColor_hsv, i);
    if (distances[i] < minDist) {
      // Update minimum distance
      minDist = distances[i];
    }
    if (distances[i] > maxDist) {
      // Update maximum distance
      maxDist = distances[i];
    }
  }
  
  print("min: ",minDist," ");
  print("MAX: ",maxDist," ");
  println();
  
  // Flag to check if all weights are zero
  boolean allZeros = true;
  
  // Calculate weights for each color based on the distances
  for (int i = 0; i < distances.length; i++) {
      // If the distance range is small, set weight to 1 to avoid division by zero
      if((maxDist-minDist)<10){
        weights[i] = 1;
      }else weights[i] = (maxDist-distances[i])/(maxDist-minDist); // Calculate weight based on the distance excursion
    
    // Check if any weight is non-zero
    if(weights[i]!=0) allZeros = false;
    
    print("d ",i," = ",distances[i]," ");
    print("w ",i," = ",weights[i]," ");
    println();
  }
  
  // Handle the edge case where all weights are zero
  if (allZeros){
    // Create an array with all elements set to 1
    float[] ones = new float[5];
    for (int i = 0; i < nrColors; i++) ones[i] = 1;
    
    // Assign the array with all ones as the weights array
    weights = ones;
    println("Edge case, corrected to: ");
    for (int i = 0; i < distances.length; i++) {
      print("d ",i," = ",distances[i]," ");
      print("w ",i," = ",weights[i]," ");
      println();
    }
  }
  
  return weights;
}
