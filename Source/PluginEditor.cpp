/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "ACParameters.h"

//==============================================================================
ParametricEQ_ACAudioProcessorEditor::ParametricEQ_ACAudioProcessorEditor (ParametricEQ_ACAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{

    auto* bandEditors = bandEditor.add (new BandEditor (1, processor));
    
    addAndMakeVisible (bandEditors);

//    for (int i = 0; i < 1; ++i){
     XYController* xyPoint  =  new XYController (processor.getTreeState(), 5);

//    xyPoint->addListener(this);
    mXYPoints.add (xyPoint);
    
    xyPoint->setTopLeftPosition(0, 0);
    xyPoint->setSize(500,100);

    addAndMakeVisible(xyPoint);
    
    mMainPanel = std::make_unique<ACMainPanel>(&processor);
    mMainPanel->setBounds(0, 100, 150, 200);
    addAndMakeVisible(mMainPanel.get());
    
    mBottomRightPanel = std::make_unique<ACBottomRightPanel>(&processor);
    mBottomRightPanel->setBounds(350, 100, 150, 200);
    addAndMakeVisible(mBottomRightPanel.get());
    
    mLookAndFeel = std::make_unique<ACLookAndFeel>();
    bandEditors->setLookAndFeel(mLookAndFeel.get());
    
    setSize(500, 200);
        
    updateFrequencyResponses();

    processor.addChangeListener(this);

}

ParametricEQ_ACAudioProcessorEditor::~ParametricEQ_ACAudioProcessorEditor()
{
    processor.removeChangeListener(this);
    
    bandEditor[0]->setLookAndFeel(nullptr);
    
}

//==============================================================================
void ParametricEQ_ACAudioProcessorEditor::paint (Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.fillAll(Colours::black);
   
    auto* filterBand0 = processor.getBand (4);

    g.setColour ( filterBand0->magColour.withAlpha (1.0f));
    g.setColour (draggingBand == int (0) ? filterBand0->magColour : filterBand0->magColour.withAlpha (1.0f));
        
    g.strokePath (frequencyResponse, PathStrokeType (1.5));
    
    const Colour inputColour = Colours::dimgrey;
    
    g.reduceClipRegion (plotFrame);
    
    processor.createAnalyserPlot (analyzerPath, plotFrame, 20.0f, true);
    g.setColour (inputColour);

    g.strokePath (analyzerPath, PathStrokeType (1.0));
    processor.createAnalyserPlot (analyzerPath, plotFrame, 20.0f, false);
    
    //creating the plotFrame grid outlines
    g.setColour (Colours::darkgrey.withAlpha (0.8f));
    g.drawRoundedRectangle(plotFrame.toFloat(), 10, 2);
    for (int i = 0; i < 12; i++)
    {
        auto x = plotFrame.getX() + plotFrame.getWidth() * i * 0.1f;
        if (i > 0) g.drawVerticalLine (roundToInt(x), plotFrame.getY(), plotFrame.getBottom());
        
        auto freq = getFrequencyForPosition(i * 0.1f);
        g.drawFittedText((freq < 1000) ? String (freq) + " Hz" : String (freq / 1000, 1) + " kHz", roundToInt(x+3), plotFrame.getBottom() - 20, 50, 15, Justification::left, 1);
    }
    
    g.drawHorizontalLine(roundToInt(plotFrame.getY() + 0.25 * plotFrame.getHeight()), plotFrame.getX(), plotFrame.getRight());
    g.drawHorizontalLine(roundToInt(plotFrame.getY() + 0.75 * plotFrame.getHeight()), plotFrame.getX(), plotFrame.getRight());
    
    
    //TODO make dry

//    g.drawFittedText(String(24.0f) + " dB", plotFrame.getX() + 5, plotFrame.getY() + 2, 50, 15, Justification::left, 1);
    
    int x = 0.25;
    
    for (int j = 24; j > -12; j-12)
    {
    g.drawFittedText(String(j) + " dB", plotFrame.getX() + 5, roundToInt (plotFrame.getY() + 2 + (x * plotFrame.getHeight())), 50, 15, Justification::left, 1);
        
        x = x + 0.25;
    }
}

void ParametricEQ_ACAudioProcessorEditor::resized()
{
    
    plotFrame = getLocalBounds().reduced (3, 3);

    auto bandSpace = plotFrame.removeFromBottom (getHeight() / 2);
    auto width = roundToInt (bandSpace.getWidth());
    for (auto* bandEditors : bandEditor)
        bandEditors->setBounds(bandSpace.removeFromLeft (width));

    plotFrame.reduce (3, 3);

    updateFrequencyResponses();
}

void ParametricEQ_ACAudioProcessorEditor::changeListenerCallback (ChangeBroadcaster* sender)
{
    ignoreUnused (sender);
    updateFrequencyResponses();
 
    if (mXYPoints[0]->mousing == true)
    {
        int iSelected = mXYPoints[0]->selectedDot;
        
        bandVisible(iSelected, true);
            
        for(int i = 1; i < 5; ++i)
            bandVisible(i, false);
    }
    repaint();
}

void ParametricEQ_ACAudioProcessorEditor::bandVisible(int i, bool visible)
{
    bandEditor[0]->frequencyCutoffV[i]->Component::setVisible(visible);
    bandEditor[0]->qV.at(i)->Component::setVisible(visible);
    bandEditor[0]->gainV.at(i)->Component::setVisible(visible);
}

void ParametricEQ_ACAudioProcessorEditor::timerCallback()
{
    if (processor.checkForNewAnalyserData())
        repaint (plotFrame);
}

void ParametricEQ_ACAudioProcessorEditor::updateFrequencyResponses()
{
    auto pixelsPerDouble = 2.0f * plotFrame.getHeight() / Decibels::decibelsToGain (24.0f);

    for (int i=0; i < 1; ++i)
    {
        auto* bandEditors = bandEditor.getUnchecked (i);

        if (auto* band = processor.getBand(size_t (i)))
        {
            bandEditors->frequencyResponse.clear();
            processor.createFrequencyPlot (bandEditors->frequencyResponse, band->magnitudes, plotFrame.withX (plotFrame.getX() + 1), pixelsPerDouble);
        }
    }
    frequencyResponse.clear();
    processor.createFrequencyPlot (frequencyResponse, processor.getMagnitudes(), plotFrame, pixelsPerDouble);
}

float ParametricEQ_ACAudioProcessorEditor::getPositionForFrequency (float freq)
{
    return (std::log (freq / 20.0f) / std::log (2.0f)) / 10.0f;
}

float ParametricEQ_ACAudioProcessorEditor::getFrequencyForPosition (float pos)
{
    return 20.0f * std::pow (2.0f, pos * 10.0f);
}

ParametricEQ_ACAudioProcessorEditor::BandEditor::BandEditor (size_t i, ParametricEQ_ACAudioProcessor& p) :
    index(i),
    processor (p)
{

    for(int i = 0; i < 5; ++i)
    {
        auto frequencyCutoff = std::make_unique<Slider>(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow);
        
        addAndMakeVisible (frequencyCutoff.get());
        
        auto freqAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.getPluginState(), "Freq" + String(i), (Slider&)frequencyCutoff);
        
        attachments.push_back(std::move(freqAttachment));
            
        frequencyCutoffV.push_back(std::move(frequencyCutoff));
        
        //========================================
        
        auto quality = std::make_unique<Slider>(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow);
        
        addAndMakeVisible (quality.get());
        
        auto qualityAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (processor.getPluginState(), "Q" + String(i), (Slider&)quality);

        attachments.push_back(std::move(qualityAttachment));
            
        qV.push_back(std::move(quality));
        
        //========================================

        auto gain = std::make_unique<Slider>(Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow);
        
        addAndMakeVisible (gain.get());
        
        auto gainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment> (processor.getPluginState(), "Gain" + String(i), (Slider&)gain);

        attachments.push_back(std::move(gainAttachment));
            
        gainV.push_back(std::move(gain));
        
        frequencyCutoffV[i]->setTextBoxIsEditable(true);
        qV[i]->setTextBoxIsEditable(true);
        gainV[i]->setTextBoxIsEditable(true);
    }

    // last 4 frequency bands only
    for (int i = 1; i < 5; ++i)
    {
        frequencyCutoffV[i]->setVisible(false);
        qV[i]->setVisible(false);
        gainV[i]->setVisible(false);
    }
}

void ParametricEQ_ACAudioProcessorEditor::BandEditor::resized ()
{

    setSize(500, 200);
//
//    int w = 100;
//    int h = 100;
//    int x = 0;
    
    int fcX = 125;
    int yPadding = 15;
    
    const int slider_size = 75;

    //change to grid class arrangement
    for(int i = 0; i < 5; ++i)
        frequencyCutoffV[i]->setBounds (fcX, yPadding, slider_size, slider_size);
    
    int qX = fcX + 75;
    
    for(int i = 0; i < 5; ++i)
        qV[i]->setBounds (qX, yPadding, slider_size, slider_size);

    int gX = qX + 75;
    
    for(int i = 0; i < 5; ++i)
        gainV[i]->setBounds (gX, yPadding, slider_size, slider_size);
}

void ParametricEQ_ACAudioProcessorEditor::BandEditor::setFrequency (int band, float freq)
{
    frequencyCutoffV[band]->setValue (freq, sendNotification);
}

void ParametricEQ_ACAudioProcessorEditor::BandEditor::setGain (int band, float gainToUse)
{
    gainV[band]->setValue (gainToUse, sendNotification);
}
