# ColorMixer

ColorMixer is an innovative sound-mixing application being developed by the LemonJUICE team. 
The project is built using a combination of Juce, Processing, and Arduino technologies. Juce is used for the graphical user interface and audio settings, Processing handles the visual rendering and color manipulation, while Arduino is utilized for integrating the Arduino Uno R3 board and the Display RGB LED as a color sensor. 
With the Color Mixer application, users can select from a range of songs and visually manipulate the mix by interacting with colors. The application provides real-time feedback, allowing users to see and hear the impact of their color choices on the song's mix.
For the Color Mixer project to function properly and establish the connection between the sensor and the port, it is essential to run Juce first and then Processing.

## Features

- **Color Sensing**: Uses a TCS34725 RGB Color Sensor attached to an Arduino setup to capture user-presented colors.
- **Track Volume Control**: Adjusts the volume of individual tracks in a song based on the proximity of the scanned color to the reference colors assigned to each track (5 tracks per song).
- **Visual Feedback**: Provides dynamic visual feedback using Processing, based on OSC messages from JUCE.
- **User Control**: Includes a simple GUI for playing, pausing, looping, and switching songs.

## Graphical User Interface

The GUI of the Color Mixer combines Juce and Processing windows, providing users with a comprehensive interface to interact with both the color controls and the audio settings.  

<img width="1440" alt="competeGUI" src="https://github.com/ale-moro/ColorMixer/assets/79703541/a9b48d8f-bf7c-4c5f-8217-2feee33c40ee">

## Repository Content

- **ColorMixer.jucer**: Project file generated by the Projucer tool.
- **JuceLibraryCode**: Folder that contains alle the used juce libraries.
- **Report_HW3_Group7**: Report of the project.
- **Presentation_HW3_Group7**: Slides presentation of the project.
- **Songs**: This folder contains a collection of songs that can be selected within the application:
  - Charlie Puth - Attention
  - Aretha Franklin - You Make Me Feel
- **Source**: This folder contains the soruces file:
  - arduino_code: 
    - arduino_code.ino
    - Arduino Libraries (folder)
  - Processing: 
    - ColorWheel.pde
    - Pointer.pde
    - Colors.pde
  - PluginEditor.cpp
  - PluginEditor.h
  - PluginProcessor.cpp
  - PluginProcessor.h
  
  
## Group Members
- Guglielmo Fratticioli
- Chiara Lunghi
- Alessandra Moro
- Elia Pirrello
