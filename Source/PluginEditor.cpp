#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_gui_basics/juce_gui_basics.h>

ColorMixerAudioProcessorEditor::ColorMixerAudioProcessorEditor(ColorMixerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(800, 600);
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::black);

    // Initialize and set up the buttons
    playButton.setButtonText("Play");
    loopButton.setButtonText("Loop");
    button3.setButtonText("Button 3");

    addAndMakeVisible(playButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(button3);
    addAndMakeVisible(tableListBox);
    tableListBox.setHeaderHeight(30);
    circleColor = juce::Colours::wheat;
}

ColorMixerAudioProcessorEditor::~ColorMixerAudioProcessorEditor()
{
}

void ColorMixerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);

    int width = getWidth();
    int height = getHeight();
    int leftWidth = width * 0.6;
    int rightWidth = width * 0.4;
    int leftRectHeight = height * 0.8;
    int rightRectHeight = height * 0.6;
    int padding = 10;
    int borderWidth = 2;

    // Draw rectangles
    juce::Rectangle<int> leftRectangle(0, 0, leftWidth, leftRectHeight);
    juce::Rectangle<int> rightRectangle(leftWidth, 0, rightWidth, rightRectHeight);
    juce::Rectangle<int> middleRectangle(leftWidth, rightRectHeight, rightWidth, leftRectHeight - rightRectHeight);
    juce::Rectangle<int> bottomRectangle(0, leftRectHeight, width, height - leftRectHeight);
    juce::Rectangle<int> canvasRectangle(bottomRectangle.getX() + padding, bottomRectangle.getY() + padding, bottomRectangle.getWidth() - (2 * padding), bottomRectangle.getHeight() - (2 * padding));

    g.setColour(juce::Colours::lightblue);
    g.fillRect(leftRectangle);

    g.setColour(juce::Colours::lightgreen);
    g.fillRect(rightRectangle);

    g.setColour(juce::Colours::coral);
    g.fillRect(middleRectangle);

    g.setColour(juce::Colours::lightyellow);
    g.fillRect(bottomRectangle);

    g.setColour(juce::Colours::white);
    g.fillRect(canvasRectangle);
    g.setColour(juce::Colours::black);
    g.drawRect(canvasRectangle, borderWidth);

    // Divide rightRectangle vertically into two new rectangles
    int topRightRectHeight = rightRectHeight * 0.65; // 60% height of rightRectangle
    int bottomRightRectHeight = rightRectHeight * 0.35; // 40% height of rightRectangle

    juce::Rectangle<int> topRightRectangle(rightRectangle.getX(), rightRectangle.getY(), rightRectangle.getWidth(), topRightRectHeight);
    juce::Rectangle<int> bottomRightRectangle(rightRectangle.getX(), topRightRectangle.getBottom(), rightRectangle.getWidth(), bottomRightRectHeight);

    // Set colors for the new rectangles
    g.setColour(juce::Colours::lightpink);
    g.fillRect(topRightRectangle);

    g.setColour(juce::Colours::lightgrey);
    g.fillRect(bottomRightRectangle);

    // Draw buttons
    int buttonWidth = (middleRectangle.getWidth() - 2 * padding) / 3;
    int buttonHeight = middleRectangle.getHeight() - (2 * padding);
    int buttonX = middleRectangle.getX() + padding;
    int buttonY = middleRectangle.getY() + padding;

    playButton.setBounds(buttonX, buttonY, buttonWidth, buttonHeight);
    loopButton.setBounds(buttonX + buttonWidth, buttonY, buttonWidth, buttonHeight);
    button3.setBounds(buttonX + (2 * buttonWidth), buttonY, buttonWidth, buttonHeight);

    // Draw the circle in the topRightRectangle
    int circleX = topRightRectangle.getX() + topRightRectangle.getWidth() / 2;
    int circleY = topRightRectangle.getY() + topRightRectangle.getHeight() / 2;

    // Calculate the circle's radius based on the smaller dimension of the topRightRectangle
    int circleRadius = fmin(topRightRectangle.getWidth(), topRightRectangle.getHeight()) * 0.4;


    // Draw the circle
    g.setColour(circleColor);
    g.fillEllipse(circleX - circleRadius, circleY - circleRadius, 2 * circleRadius, 2 * circleRadius);
    
    // Set bounds for tableListBox in the bottomRightRectangle
    int tableListBoxX = bottomRightRectangle.getX() + padding;
    int tableListBoxY = bottomRightRectangle.getY() + padding;
    int tableListBoxWidth = bottomRightRectangle.getWidth() - (2 * padding);
    int tableListBoxHeight = bottomRightRectangle.getHeight() - (2 * padding);

    tableListBox.setBounds(tableListBoxX, tableListBoxY, tableListBoxWidth, tableListBoxHeight);
}


void ColorMixerAudioProcessorEditor::resized()
{
    // ...
}

