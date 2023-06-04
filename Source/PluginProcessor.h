/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_core/juce_core.h>



//==============================================================================
/**
*/



class ColorMixerAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
,
private juce::OSCReceiver,
private juce::OSCReceiver::ListenerWithOSCAddress<juce::OSCReceiver::MessageLoopCallback>
{
public:
    //==============================================================================
    ColorMixerAudioProcessor();
    ~ColorMixerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    //==============================================================================
        
    //overriding oscMessageReceived to define behavior when receiving OSC messages
    void oscMessageReceived(const juce::OSCMessage& message) override;
    
    //==============================================================================
   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //==============================================================================
    // Interface
    void loadSong(juce::String& song);
    void setTrackVolume(int n_track , float volume);
    void setPlayHeadStart(float position);
    void setLoopEnd(float position);
    void setLoopStart(float position);
    void setIsPlaying(bool playing);
    void setIsLooping(bool looping);
    void setIsStopped(bool stopping);
    void setSongArray(juce::String selectedSong);
    void sendSeconds();

    
    
    


private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColorMixerAudioProcessor)

    
    void loadFileIntoBuffer(juce::File& file, juce::AudioSampleBuffer& buffer);

    
    int trackLength; 
    int playHead;
    int loopEnd;
    int loopStart;
    int numTracks;
    int numSamples;
    
    int nrTracks = 5;
    
    bool isLooping;
    bool isPlaying;
    bool isStopped;
    bool namesSent = false;
    
    float durationInSeconds = 0;

    
    juce::StringPairArray folderNamesDictionary;
    juce::String songName;

    std::map<int, juce::AudioSampleBuffer*> trackBuffers;
    std::vector<float> trackVolumes;
    
};
