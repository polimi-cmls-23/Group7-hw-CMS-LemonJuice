#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_gui_basics/juce_gui_basics.h>

ColorMixerAudioProcessorEditor::ColorMixerAudioProcessorEditor(ColorMixerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(300, 800);
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::black);
    
    // Set initial buttons text
    playButton.setButtonText("Play");
    loopButton.setButtonText("Loop OFF");
    stopButton.setButtonText("Stop");
    
    // Add the components to the GUI
    addAndMakeVisible(playButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loopSlider);
    addAndMakeVisible(leftNumBox);
    addAndMakeVisible(rightNumBox);
    addAndMakeVisible(currentPosition);
    
    // Add listeners for buttons and sliders
    playButton.addListener(this);
    loopButton.addListener(this);
    stopButton.addListener(this);
    loopSlider.addListener(this);
   
    
    // Meters colors
    instrumentColors.add(juce::Colour(255, 0, 0)); // HSV(0,100,100)
    instrumentColors.add(juce::Colour(204, 255, 0)); // HSV(72,100,100)
    instrumentColors.add(juce::Colour(0, 255, 102)); // HSV(144,100,100)
    instrumentColors.add(juce::Colour(0, 102, 255)); // HSV(216,100,100)
    instrumentColors.add(juce::Colour(204, 0, 255)); // HSV(288,100,100)
    instrumentColors.add(juce::Colour(178, 0, 0));
    instrumentColors.add(juce::Colour(195, 201, 26));
    instrumentColors.add(juce::Colour(23, 166, 80));
    instrumentColors.add(juce::Colour(11, 52, 114));
    instrumentColors.add(juce::Colour(130, 28, 155));
    
    // Takes the names of the songs from the project directory
    fillSongnames();

    // Create the items for the dropdown menu
    for (int i = 0; i < sizeArray; i++) {
        dropdown.addItem(songNames[i], i + 1);
    }

    // Set the default selection to the first song
    dropdown.setSelectedId(1, juce::dontSendNotification);

    // Register the listener for dropdown events
    dropdown.addListener(this);

    // Add the dropdown menu to the GUI
    addAndMakeVisible(dropdown);
    
    
    // Send to the processor the selected song
    if (dropdown.getSelectedId() != 0)
    {
        selectedTitle = dropdown.getText();
        selectedSong = dropdown.getSelectedId();
        std::cout << "Selected song: " << selectedTitle<<std::endl;
        
        audioProcessor.setSongArray(folderNamesDictionary[selectedTitle]);
    }
        
    // Set the slider range
    loopSlider.setSliderStyle(juce::Slider::SliderStyle::TwoValueHorizontal);
    loopSlider.setRange(minValue, maxValue,0.01);
    
    
}
    

ColorMixerAudioProcessorEditor::~ColorMixerAudioProcessorEditor()
{
}

void ColorMixerAudioProcessorEditor::paint(juce::Graphics& g)
{
    juce::MessageManagerLock lock;
    
    // First time set-up
    if(firstTime){
        audioProcessor.sendSeconds();
        loopSlider.setMinValue(minValue);
        loopSlider.setMaxValue(maxValue);
        firstTime = false;
    }
    
    g.fillAll(juce::Colours::black);



    // Draw rectangles
    int width = getWidth();
    int height = getHeight();
    int centerWidth = width * 0.8;
    upperWidth = width * 0.8;
    int centerHeight = height * 0.6;
    upperHeight = height * 0.08;
    int bottomWidth = upperWidth;
    int bottomHeight = upperHeight;
    padding = 10;
    
    juce::Rectangle<int> centerRectangle(0.1*width, 0.14*height, centerWidth, centerHeight);
    upperRectangle.setBounds(0.1*width, 0.03*height, upperWidth, upperHeight);
    juce::Rectangle<int> bottomRectangle(0.1*width, 0.89*height, bottomWidth,  bottomHeight);


    g.setColour(juce::Colours::lightgrey);
    g.fillRect(centerRectangle);
    g.fillRect(upperRectangle);
    g.fillRect(bottomRectangle);
    
    // Draw buttons
    int buttonWidth = (bottomRectangle.getWidth()-2*padding) / 3;
    int buttonHeight = bottomRectangle.getHeight()-2*padding;
    int buttonX = bottomRectangle.getX()+padding;
    int buttonY = bottomRectangle.getY()+padding;

    playButton.setBounds(buttonX, buttonY, buttonWidth, buttonHeight);
    loopButton.setBounds(buttonX + buttonWidth, buttonY, buttonWidth, buttonHeight);
    stopButton.setBounds(buttonX + (2 * buttonWidth), buttonY, buttonWidth, buttonHeight);

    
    // Draw loop slider and numboxes
    int slidWidth = 0.5 * width;
    int slidHeight = 0.08 * height;
    int slidX = 0.25 * width;
    int slidY = 0.78 * height;
    
    
    int boxWidth = 0.12 * width;
    int boxHeight = 0.03 * height;
    int leftBoxX = 0.11 * width;
    int boxY = 0.8 * height;
    int rightBoxX = 0.79 * width;
    
    
    
    loopSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    loopSlider.setBounds(slidX, slidY, slidWidth, slidHeight);
    leftNumBox.setBounds(leftBoxX, boxY, boxWidth, boxHeight);
    rightNumBox.setBounds(rightBoxX, boxY, boxWidth, boxHeight);
    loopSlider.setWantsKeyboardFocus(true);
    
    

    // Divide centerRectangle vertically into 5 new rectangles and create the subrectangles for the volume (Level Meters)
    rectMetersHeight = centerHeight * 0.98;
    int rectMetersX = centerRectangle.getX() + padding/2;
    int rectMetersY = centerRectangle.getY() + padding/2;
    int rectMetersWidth = centerRectangle.getWidth()/5 - 0.60*padding;
    int delta = rectMetersWidth + padding/2;
   
    
    juce::Rectangle<int> rectMeter1(rectMetersX, rectMetersY,rectMetersWidth, rectMetersHeight);
    juce::Rectangle<int> rectMeter2(rectMetersX + delta , rectMetersY, rectMetersWidth, rectMetersHeight);
    juce::Rectangle<int> rectMeter3(rectMetersX + 2*delta , rectMetersY, rectMetersWidth, rectMetersHeight);
    juce::Rectangle<int> rectMeter4(rectMetersX + 3*delta , rectMetersY, rectMetersWidth, rectMetersHeight);
    juce::Rectangle<int> rectMeter5(rectMetersX + 4*delta , rectMetersY, rectMetersWidth, rectMetersHeight);

    
    rectMeter12.setBounds(rectMetersX, rectMeter1.getBottom(),rectMetersWidth, -(juce::jmap(volumesArray[0],0.0f,1.0f,0.0f,(float)rectMetersHeight)));
    rectMeter22.setBounds(rectMetersX + delta , rectMeter1.getBottom(), rectMetersWidth, -(juce::jmap(volumesArray[1],0.0f,1.0f,0.0f,(float)rectMetersHeight)));
    rectMeter32.setBounds(rectMetersX + 2*delta , rectMeter1.getBottom(), rectMetersWidth, -(juce::jmap(volumesArray[2],0.0f,1.0f,0.0f,(float)rectMetersHeight)));
    rectMeter42.setBounds(rectMetersX + 3*delta , rectMeter1.getBottom(), rectMetersWidth, -(juce::jmap(volumesArray[3],0.0f,1.0f,0.0f,(float)rectMetersHeight)));
    rectMeter52.setBounds(rectMetersX + 4*delta , rectMeter1.getBottom(), rectMetersWidth, -(juce::jmap(volumesArray[4],0.0f,1.0f,0.0f,(float)rectMetersHeight)));
    
    // Position the Label for the progressing of the song
    currentPosition.setBounds(0.55*centerWidth, 0.85*height, boxWidth, boxHeight);
  
    
    
    // Set colors for the meters rectangles
    g.setColour(instrumentColors[5]);
    g.fillRect(rectMeter1);
    
    g.setColour(instrumentColors[6]);
    g.fillRect(rectMeter2);
    
    g.setColour(instrumentColors[7]);
    g.fillRect(rectMeter3);
    
    g.setColour(instrumentColors[8]);
    g.fillRect(rectMeter4);
    
    g.setColour(instrumentColors[9]);
    g.fillRect(rectMeter5);
    
    g.setColour(instrumentColors[0]);
    g.fillRect(rectMeter12);
    
    g.setColour(instrumentColors[1]);
    g.fillRect(rectMeter22);
    
    g.setColour(instrumentColors[2]);
    g.fillRect(rectMeter32);
    
    g.setColour(instrumentColors[3]);
    g.fillRect(rectMeter42);
    
    g.setColour(instrumentColors[4]);
    g.fillRect(rectMeter52);
    
    // Set up the dropdown menu
    dropdown.setBounds(upperRectangle.getX() + padding, upperRectangle.getY() + padding, upperWidth - 2 * padding, upperHeight - 2 * padding);
    dropdown.setColour(juce::ComboBox::backgroundColourId, juce::Colours::darkgrey);
    
    
}

// Buttons listener
void ColorMixerAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    juce::MessageManagerLock lock;
    
    if(button == &playButton){
        // If the clicked button is the playButton
        
        if(isPlaying){
            // Update the text of the playButton
            playButton.setButtonText("Play");
            isPlaying = false;
            
            // Set the isPlaying flag in the audio processor
            audioProcessor.setIsPlaying(isPlaying);
            
        }else{
            // Update the text of the playButton
            playButton.setButtonText("Pause");
            isPlaying = true;
            
            
            if(startLoop == endLoop){
                // If startLoop and endLoop are equal, update the loop start and end positions
                audioProcessor.setLoopStart(startLoop);
                audioProcessor.setLoopEnd(endLoop);
                
                // Update the minimum and maximum values of the loopSlider
                loopSlider.setMinValue(minValue);
                loopSlider.setMaxValue(maxValue);
            
            }else{
                // If startLoop and endLoop are not equal, update only the loop start and end positions
                audioProcessor.setLoopStart(startLoop);
                audioProcessor.setLoopEnd(endLoop);
            }
            
            // Set the isPlaying flag in the audio processor
            audioProcessor.setIsPlaying(isPlaying);
            
            if(isStopped){
                
                // If isStopped is true, update the loop start position
                audioProcessor.setLoopStart(startLoop);
                isStopped = false;
                
                // Set the isStopped flag in the audio processor
                audioProcessor.setIsStopped(isStopped);
                
            }
            
            
        }
        
    }
    
    if(button == &stopButton){
        // If the clicked button is the stopButton
        isPlaying = false;
        isStopped = true;
        
        // Update the text of the playButton
        playButton.setButtonText("Play");
        
        // Set the isPlaying flag in the audio processor
        audioProcessor.setIsPlaying(isPlaying);
        if(isLooping){
            // If looping is enabled, update the loop start and end positions in the audio processor
            audioProcessor.setLoopStart(startLoop);
            audioProcessor.setLoopEnd(endLoop);
        }else{
            // If looping is disabled, set the loop start and end positions to default values (0.00f and 1.00f)
            audioProcessor.setLoopStart(0.00f);
            audioProcessor.setLoopEnd(1.00f);
        }
        
        // Set the isStopped flag in the audio processor
        audioProcessor.setIsStopped(isStopped);
        
    }
    
    if(button == &loopButton){
        // If the clicked button is the loopButton
        
        if(isLooping){
            // Update the text of the loopButton
            loopButton.setButtonText("Loop OFF");
            isLooping = false;
            
            // Set the isLooping flag in the audio processor
            audioProcessor.setIsLooping(isLooping);
            
            // Set the loop start and end positions to default values (0.00f and 1.00f)
            audioProcessor.setLoopStart(0.00f);
            audioProcessor.setLoopEnd(1.00f);
          
        }else{
            // Update the text of the loopButton
            loopButton.setButtonText("Loop ON");
            isLooping = true;
            
            // Set the isLooping flag in the audio processor
            audioProcessor.setIsLooping(isLooping);
            
            // Update the loop start and end positions in the audio processor
            audioProcessor.setLoopStart(startLoop);
            audioProcessor.setLoopEnd(endLoop);
        
       
        }
    }
}


// Slider listener
void ColorMixerAudioProcessorEditor::sliderValueChanged(juce::Slider *slider){
   
    juce::MessageManagerLock lock;
    
    
    // Get the loop start and end values from the loopSlider
    startLoop = loopSlider.getMinValue();
    endLoop = loopSlider.getMaxValue();

    // Map the loop start and end values to the range 0.00f to songSeconds (Duration of the song)
    mappedStart = juce::jmap((float)startLoop, 0.00f, 1.00f,0.00f, songSeconds);
    mappedEnd = juce::jmap((float)endLoop,0.00f,1.00f,0.00f,songSeconds);
    
    
    // Update the text of the leftNumBox and rightNumBox with formatted time values (mm:ss)
    leftNumBox.setText(formatTime(startLoop));
    rightNumBox.setText(formatTime(endLoop));
 
    if(isLooping){
        // Set the loop start and end positions in the audio processor
        audioProcessor.setLoopStart(startLoop);
        audioProcessor.setLoopEnd(endLoop);
    }else{
        // Set the loop start and end positions to default values (0.00f and 1.00f) in the audio processor
        audioProcessor.setLoopStart(0.00f);
        audioProcessor.setLoopEnd(1.00f);
    }
    
    // Repaint the component
    repaint();
    
    
}


// Dropdown menu listener
void ColorMixerAudioProcessorEditor::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged){
    
    juce::MessageManagerLock lock;
     
    if (comboBoxThatHasChanged == &dropdown){
    
        // Check if an actual item selection has occurred
        if (dropdown.getSelectedId() != 0)
        {
            // Retrieve the selected title and song ID from the dropdown
            selectedTitle = dropdown.getText();
            selectedSong = dropdown.getSelectedId();
            std::cout << "Selected song: " << selectedTitle<<std::endl;

            //STOP FUNCTION
            // Stop playback and set the play button text to "Play"
            isPlaying = false;
            isStopped = true;
            playButton.setButtonText("Play");
            audioProcessor.setIsPlaying(isPlaying);
            audioProcessor.setIsStopped(isStopped);
            
            // Set the song title in the audio processor based on the selected title
            audioProcessor.setSongArray(folderNamesDictionary[selectedTitle]);
            
            // Send the duration of the selected song to the audio processor
            audioProcessor.sendSeconds();
            
            
            
            
        }
      
    }

}

void ColorMixerAudioProcessorEditor::setVolumeArray(juce::Array<float> trackVolumes){
    // Set the volumes array from the processor
    volumesArray = trackVolumes;
    
    // Call the function to update the Level Meters based on the new volumes
    updateMeterRectangles();
}

void ColorMixerAudioProcessorEditor::setSeconds(float seconds){
    juce::MessageManagerLock lock;
    
    // Set the duration of the song from the processor
    songSeconds = seconds;
    
    // Call the slider listener to update the values of the boxes
    sliderValueChanged(&loopSlider);
    
    // Repaint the component
    repaint();
    
}


void ColorMixerAudioProcessorEditor::setCurrent(float currentPos){
    
    juce::MessageManagerLock lock;
    
    // Assign the formatted time (mm:ss) to a temporary string
    temppos = transformMinutes(currentPos);
                
                
    if(temppos.length() == 0 ){
    // If the string is empty, set the Label to "0:00" ( Beginning of the song )
        currentPosition.setText("0:00", juce::NotificationType::dontSendNotification);
    }else{
    // Set the label to the current time of the song
        currentPosition.setText(temppos, juce::NotificationType::dontSendNotification);
    }
     
    // Repaint the component
    repaint();
}

bool ColorMixerAudioProcessorEditor::tempPosDiffers(float currentPos){
    // Check if the value of the current time has changed
    if (temppos != transformMinutes(currentPos)) return true;
    else return false;
}


//Receives a number from 0 to 1 and outputs a string mm:ss
juce::String ColorMixerAudioProcessorEditor::formatTime(float value){
   
    // Calculate the total number of seconds based on the value and songSeconds
    int totalSeconds = (int)(value * songSeconds) ;
    
    // Calculate the minutes
    int minutes = totalSeconds / 60;
    
    // Calculate the remaining seconds
    int seconds = totalSeconds % 60;
        
    juce::String formattedTime;
        
    // Format the time as "mm:ss"
    formattedTime << juce::String(minutes) << ":" << juce::String(seconds).paddedLeft('0', 2);
    return formattedTime;
}


// Receives the value of the current position in minutes and outputs a string mm:ss
juce::String ColorMixerAudioProcessorEditor::transformMinutes(float value){
    
    // Calculate the minutes
    int minutes = static_cast<int>(value);
    
    // Calculate the remaining seconds
    int seconds = static_cast<int>(((value - minutes) * 60.0));

    // Format the time as "mm:ss"
    juce::String formattedDuration = juce::String::formatted("%d:%02d", minutes, seconds);

     
    return formattedDuration;
}

void ColorMixerAudioProcessorEditor::updateMeterRectangles()
{
    juce::MessageManagerLock lock;
    
    // Update the heights of the meter rectangles based on the values in volumesArray
    rectMeter12.setHeight(-(juce::jmap(volumesArray[0],0.0f,1.0f,0.0f,(float)rectMetersHeight)));
    rectMeter22.setHeight(-(juce::jmap(volumesArray[1],0.0f,1.0f,0.0f,(float)rectMetersHeight)));
    rectMeter32.setHeight(-(juce::jmap(volumesArray[2],0.0f,1.0f,0.0f,(float)rectMetersHeight)));
    rectMeter42.setHeight(-(juce::jmap(volumesArray[3],0.0f,1.0f,0.0f,(float)rectMetersHeight)));
    rectMeter52.setHeight(-(juce::jmap(volumesArray[4],0.0f,1.0f,0.0f,(float)rectMetersHeight)));

    // Repaint the component
    repaint();
}

void ColorMixerAudioProcessorEditor::fillSongnames(){
    //LIST ALL SONGS AND SAVE THEM IN A DICTIONARY, ALONG WITH CORRECT NAMES
    juce::File songDirectory = juce::File::getSpecialLocation(juce::File::currentApplicationFile)
        .getChildFile("Contents")
        .getChildFile("Resources")
        .getChildFile("Songs");
    
    if (songDirectory.isDirectory())
    {
        juce::Array<juce::File> directories;
        songDirectory.findChildFiles(directories, juce::File::findDirectories, false);
        
        
        for (const juce::File& directory : directories)
        {
            juce::String originalName = directory.getFileName();
            
            // Substitute "_" with a space
            juce::String modifiedName = originalName.replaceCharacter('_', ' ');
            
            // Substitute "-" with " - "
            modifiedName = modifiedName.replace("-", " - ");
            
            // Save the names in the dictionary
            folderNamesDictionary.set(modifiedName, originalName);

        }
        
        // Fill the string array with all the songs titles
        songNames = folderNamesDictionary.getAllKeys();

        // Takes the size of the array for knowing the number of songs
        sizeArray = songNames.size();

    }
}



void ColorMixerAudioProcessorEditor::resized()
{
    // ...
}

