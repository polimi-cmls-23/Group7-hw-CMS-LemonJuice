/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class ColorMixerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener, public juce::ComboBox::Listener, public juce::Slider::Listener
{
public:
    ColorMixerAudioProcessorEditor (ColorMixerAudioProcessor&);
    ~ColorMixerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void buttonClicked(juce::Button *button) override;
    void sliderValueChanged (juce::Slider *slider) override;
    void comboBoxChanged (juce::ComboBox *comboBoxThatHasChanged) override;
    void resized() override;
    void setVolumeArray(juce::Array<float> trackVolumes);
    void updateMeterRectangles();
    void setDuration(float duration);
    void fillSongnames();
    juce::String transformMinutes(float value);
    juce::String formatTime(float value);
    void setCurrent(float currentPos);
    void updatePosition();
    bool tempPosDiffers(float currentPos);
    void setSeconds(float seconds);
   


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    void fillRectWithColor(juce::Graphics& g,const juce::Rectangle<int>& rect, juce::Colour color);
    
    
    ColorMixerAudioProcessor& audioProcessor;
    
    juce::TextButton playButton;
    juce::TextButton loopButton;
    juce::TextButton stopButton;
    
    juce::Slider loopSlider;
    juce::TextEditor leftNumBox;
    juce::TextEditor rightNumBox;
    juce::Label currentPosition;
    
    juce::ComboBox dropdown;
    juce::StringPairArray folderNamesDictionary;
    juce::StringArray songNames;
    juce::String selectedTitle;
    juce::String temppos;
    
    bool isPlaying = false;
    bool isLooping = false;
    bool isStopped = false;
    bool idSelected = false;
    bool isProgressing = false;
    float startLoop;
    float endLoop;
    float minValue = 0;
    float maxValue = 1;
    float mappedStart;
    float mappedEnd;
    int rectMetersHeight;
    int selectedSong;
    int sizeArray;
    int upperWidth;
    int padding;
    int upperHeight;
    int boxWidth;
    int boxHeight;
    int width;
    int height;
    int centerWidth;
    float durationTemp;
    float currentHead;
    
    float songSeconds=0;
    
    bool firstTime = true;
    
    
    
    juce::Array<float> volumesArray;
    
    juce::Rectangle<int> rectMeter12;
    juce::Rectangle<int> rectMeter22;
    juce::Rectangle<int> rectMeter32;
    juce::Rectangle<int> rectMeter42;
    juce::Rectangle<int> rectMeter52;
    juce::Rectangle<int> upperRectangle;
    
    
    juce::Array<juce::Colour> instrumentColors;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColorMixerAudioProcessorEditor)
};
