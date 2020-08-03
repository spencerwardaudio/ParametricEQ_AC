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
MultiAvptslAudioProcessorEditor::MultiAvptslAudioProcessorEditor (MultiAvptslAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{

    auto* bandEditor0 = bandEditors.add (new BandEditor (1, processor));
    
    addAndMakeVisible (bandEditor0);

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
    
    mLookAndFeel = std::make_unique<ACLookAndFeel>();
    bandEditors[0]->setLookAndFeel(mLookAndFeel.get());
//    LookAndFeel::setDefaultLookAndFeel(mLookAndFeel.get());
        
//    }
    
//    mXYPoints->addComponentListener(this);
    
    setSize(500, 200);
        
    updateFrequencyResponses();

    processor.addChangeListener(this);

}

MultiAvptslAudioProcessorEditor::~MultiAvptslAudioProcessorEditor()
{
    processor.removeChangeListener(this);
    
}

//==============================================================================
void MultiAvptslAudioProcessorEditor::paint (Graphics& g)
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
    
    g.drawFittedText(String(24.0f) + " dB", plotFrame.getX() + 5, plotFrame.getY() + 2, 50, 15, Justification::left, 1);
    g.drawFittedText(String(12.0f) + " dB", plotFrame.getX() + 5, roundToInt (plotFrame.getY() + 2 + (0.25 * plotFrame.getHeight())), 50, 15, Justification::left, 1);
    g.drawFittedText(String(0.0f) + " dB", plotFrame.getX() + 5, roundToInt (plotFrame.getY() + 2 + (0.5 * plotFrame.getHeight())), 50, 15, Justification::left, 1);
    g.drawFittedText(String(-12.0f) + " dB", plotFrame.getX() + 5, roundToInt (plotFrame.getY() + 2 + (0.75 * plotFrame.getHeight())), 50, 15, Justification::left, 1);

}

void MultiAvptslAudioProcessorEditor::resized()
{
    
    plotFrame = getLocalBounds().reduced (3, 3);

    auto bandSpace = plotFrame.removeFromBottom (getHeight() / 2);
    auto width = roundToInt (bandSpace.getWidth());
    for (auto* bandEditor : bandEditors)
        bandEditor->setBounds(bandSpace.removeFromLeft (width));

    plotFrame.reduce (3, 3);

    updateFrequencyResponses();
}

void MultiAvptslAudioProcessorEditor::changeListenerCallback (ChangeBroadcaster* sender)
{
            ignoreUnused (sender);
            updateFrequencyResponses();
  
//    auto priorDot = mXYPoints->activeDot;  && (mXYPoints->activeDot != priorDot)
//    int activeXY = mXYPoints.indexOf(XYController.activeDot);
    if (mXYPoints[0]->mousing == true)
    {
//        if(mXYPoints[0]->dotVector.begin() == mXYPoints[0]->activeDot){
//            std::cout << " the first element of the vector was selected " << std::endl;
//        }


        int iSelected = mXYPoints[0]->selectedDot;
//        String str = std::to_string(i);
//
//        String xySelection = "frequencyCutoff" + str;\
        
        
        //if selected dot is equalizvalent to a given index then define visible slider
        
        switch (iSelected) {
            case 1:
                bandEditors[0]->frequencyCutoff1.Component::setVisible(true);
                bandEditors[0]->Q1.Component::setVisible(true);
                bandEditors[0]->Gain1.Component::setVisible(true);
                
                
                bandEditors[0]->frequencyCutoff2.Component::setVisible(false);
                bandEditors[0]->Q2.Component::setVisible(false);
                bandEditors[0]->Gain2.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff3.Component::setVisible(false);
                bandEditors[0]->Q3.Component::setVisible(false);
                bandEditors[0]->Gain3.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff4.Component::setVisible(false);
                bandEditors[0]->Q4.Component::setVisible(false);
                bandEditors[0]->Gain4.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff5.Component::setVisible(false);
                bandEditors[0]->Q5.Component::setVisible(false);
                bandEditors[0]->Gain5.Component::setVisible(false);
                
                break;
                
            case 2:
                 bandEditors[0]->frequencyCutoff2.Component::setVisible(true);
                bandEditors[0]->Q2.Component::setVisible(true);
                bandEditors[0]->Gain2.Component::setVisible(true);
                
                
                bandEditors[0]->frequencyCutoff1.Component::setVisible(false);
                bandEditors[0]->Q1.Component::setVisible(false);
                bandEditors[0]->Gain1.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff3.Component::setVisible(false);
                bandEditors[0]->Q3.Component::setVisible(false);
                bandEditors[0]->Gain3.Component::setVisible(false);
                
                
                bandEditors[0]->frequencyCutoff4.Component::setVisible(false);
                bandEditors[0]->Q4.Component::setVisible(false);
                bandEditors[0]->Gain4.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff5.Component::setVisible(false);
                bandEditors[0]->Q5.Component::setVisible(false);
                bandEditors[0]->Gain5.Component::setVisible(false);
                
                 break;
                
            case 3:
                 bandEditors[0]->frequencyCutoff3.Component::setVisible(true);
                bandEditors[0]->Q3.Component::setVisible(true);
                bandEditors[0]->Gain3.Component::setVisible(true);
                
                bandEditors[0]->frequencyCutoff1.Component::setVisible(false);
                bandEditors[0]->Q1.Component::setVisible(false);
                bandEditors[0]->Gain1.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff2.Component::setVisible(false);
                bandEditors[0]->Q2.Component::setVisible(false);
                bandEditors[0]->Gain2.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff4.Component::setVisible(false);
                bandEditors[0]->Q4.Component::setVisible(false);
                bandEditors[0]->Gain4.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff5.Component::setVisible(false);
                bandEditors[0]->Q5.Component::setVisible(false);
                bandEditors[0]->Gain5.Component::setVisible(false);
                
                 break;
                
            case 4:
                 bandEditors[0]->frequencyCutoff4.Component::setVisible(true);
                bandEditors[0]->Q4.Component::setVisible(true);
                bandEditors[0]->Gain4.Component::setVisible(true);
                
                bandEditors[0]->frequencyCutoff1.Component::setVisible(false);
                bandEditors[0]->Q1.Component::setVisible(false);
                bandEditors[0]->Gain1.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff2.Component::setVisible(false);
                bandEditors[0]->Q2.Component::setVisible(false);
                bandEditors[0]->Gain2.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff3.Component::setVisible(false);
                bandEditors[0]->Q3.Component::setVisible(false);
                bandEditors[0]->Gain3.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff5.Component::setVisible(false);
                bandEditors[0]->Q5.Component::setVisible(false);
                bandEditors[0]->Gain5.Component::setVisible(false);
                
                break;
                
            case 5:
                 bandEditors[0]->frequencyCutoff5.Component::setVisible(true);
                bandEditors[0]->Q5.Component::setVisible(true);
                bandEditors[0]->Gain5.Component::setVisible(true);
                
                bandEditors[0]->frequencyCutoff1.Component::setVisible(false);
                bandEditors[0]->Q1.Component::setVisible(false);
                bandEditors[0]->Gain1.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff2.Component::setVisible(false);
                bandEditors[0]->Q2.Component::setVisible(false);
                bandEditors[0]->Gain2.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff3.Component::setVisible(false);
                bandEditors[0]->Q3.Component::setVisible(false);
                bandEditors[0]->Gain3.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff4.Component::setVisible(false);
                bandEditors[0]->Q4.Component::setVisible(false);
                bandEditors[0]->Gain4.Component::setVisible(false);
                
                 break;
                
            default:
                bandEditors[0]->frequencyCutoff1.Component::setVisible(true);
                bandEditors[0]->Q1.Component::setVisible(true);
                bandEditors[0]->Gain1.Component::setVisible(true);
                
                bandEditors[0]->frequencyCutoff2.Component::setVisible(false);
                bandEditors[0]->Q2.Component::setVisible(false);
                bandEditors[0]->Gain2.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff3.Component::setVisible(false);
                bandEditors[0]->Q3.Component::setVisible(false);
                bandEditors[0]->Gain3.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff4.Component::setVisible(false);
                bandEditors[0]->Q4.Component::setVisible(false);
                bandEditors[0]->Gain4.Component::setVisible(false);
                
                bandEditors[0]->frequencyCutoff5.Component::setVisible(false);
                bandEditors[0]->Q5.Component::setVisible(false);
                bandEditors[0]->Gain5.Component::setVisible(false);
                
                break;
        }

//        std::cout << mXYPoints[0]->dotVector.size() << std::endl;
//        std::cout << mXYPoints[0]->dotVector[1] << std::endl;
        
    }
            
            repaint();
}


void MultiAvptslAudioProcessorEditor::timerCallback()
{
    if (processor.checkForNewAnalyserData())
        repaint (plotFrame);
}

void MultiAvptslAudioProcessorEditor::updateFrequencyResponses ()
{
    auto pixelsPerDouble = 2.0f * plotFrame.getHeight() / Decibels::decibelsToGain (24.0f);

    for (int i=0; i < 1; ++i)
    {
        auto* bandEditor = bandEditors.getUnchecked (i);

        if (auto* band = processor.getBand(size_t (i)))
        {

            bandEditor->frequencyResponse.clear();
            processor.createFrequencyPlot (bandEditor->frequencyResponse, band->magnitudes, plotFrame.withX (plotFrame.getX() + 1), pixelsPerDouble);
        }

    }
    frequencyResponse.clear();
    processor.createFrequencyPlot (frequencyResponse, processor.getMagnitudes(), plotFrame, pixelsPerDouble);
}

float MultiAvptslAudioProcessorEditor::getPositionForFrequency (float freq)
{
    return (std::log (freq / 20.0f) / std::log (2.0f)) / 10.0f;
}

float MultiAvptslAudioProcessorEditor::getFrequencyForPosition (float pos)
{
    return 20.0f * std::pow (2.0f, pos * 10.0f);
}

MultiAvptslAudioProcessorEditor::BandEditor::BandEditor (size_t i, MultiAvptslAudioProcessor& p)
  :
frequencyCutoff1 (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
frequencyCutoff2 (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
frequencyCutoff3 (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
frequencyCutoff4 (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
frequencyCutoff5 (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),


  Q1   (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
    Q2   (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
    Q3   (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
    Q4   (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
    Q5   (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),

  Gain1     (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
Gain2     (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
Gain3     (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
Gain4     (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),
Gain5     (Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow),

index(i),
    processor (p)


{

    addAndMakeVisible (frequencyCutoff1);
    addAndMakeVisible (frequencyCutoff2);
    addAndMakeVisible (frequencyCutoff3);
    addAndMakeVisible (frequencyCutoff4);
    addAndMakeVisible (frequencyCutoff5);

    frequencyCutoff2.setVisible(false);
    frequencyCutoff3.setVisible(false);
    frequencyCutoff4.setVisible(false);
    frequencyCutoff5.setVisible(false);
    
    
    addAndMakeVisible (Q1);
    addAndMakeVisible (Q2);
    addAndMakeVisible (Q3);
    addAndMakeVisible (Q4);
    addAndMakeVisible (Q5);
    
    Q2.setVisible(false);
    Q3.setVisible(false);
    Q4.setVisible(false);
    Q5.setVisible(false);
    
    
    addAndMakeVisible (Gain1);
    addAndMakeVisible (Gain2);
    addAndMakeVisible (Gain3);
    addAndMakeVisible (Gain4);
    addAndMakeVisible (Gain5);
    
    Gain2.setVisible(false);
    Gain3.setVisible(false);
    Gain4.setVisible(false);
    Gain5.setVisible(false);

    
//    String ID = "Freq1";

    
//    for (int r = 1; r < 2; r++)
//    {
//
//       String m =  ID.replaceSection(4, 1, (String)r);

    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Freq1", frequencyCutoff1));
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Freq2", frequencyCutoff2));
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Freq3", frequencyCutoff3));
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Freq4", frequencyCutoff4));
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Freq5", frequencyCutoff5));
    
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Q1", Q1));
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Q2", Q2));
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Q3", Q3));
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Q4", Q4));
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Q5", Q5));
    
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Gain1", Gain1));
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Gain2", Gain2));
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Gain3", Gain3));
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Gain4", Gain4));
    attachments.add (new AudioProcessorValueTreeState::SliderAttachment (processor.getPluginState(), "Gain5", Gain5));
//
//        std::cout << "ID " << m << std::endl;
//    }

}

void MultiAvptslAudioProcessorEditor::BandEditor::resized ()
{

    setSize(500, 200);
//
//    int w = 100;
//    int h = 100;
//    int x = 0;
    
    int fcX = 125;
    int yPadding = 15;
    
    const int slider_size = 75;

    frequencyCutoff1.setBounds (fcX, yPadding, slider_size, slider_size);
    frequencyCutoff2.setBounds (fcX, yPadding, slider_size, slider_size);
    frequencyCutoff3.setBounds (fcX, yPadding, slider_size, slider_size);
    frequencyCutoff4.setBounds (fcX, yPadding, slider_size, slider_size);
    frequencyCutoff5.setBounds (fcX, yPadding, slider_size, slider_size);
    
    int qX = fcX + 75;
    
    Q1.setBounds (qX, yPadding, slider_size, slider_size);
    Q2.setBounds (qX, yPadding, slider_size, slider_size);
    Q3.setBounds (qX, yPadding, slider_size, slider_size);
    Q4.setBounds (qX, yPadding, slider_size, slider_size);
    Q5.setBounds (qX, yPadding, slider_size, slider_size);
    
    int gX = qX + 75;
    
    Gain1.setBounds (gX, yPadding, slider_size, slider_size);
    Gain2.setBounds (gX, yPadding, slider_size, slider_size);
    Gain3.setBounds (gX, yPadding, slider_size, slider_size);
    Gain4.setBounds (gX, yPadding, slider_size, slider_size);
    Gain5.setBounds (gX, yPadding, slider_size, slider_size);
    
}

void MultiAvptslAudioProcessorEditor::BandEditor::setFrequency (float freq)
{
    frequencyCutoff1.setValue (freq, sendNotification);
    
}

void MultiAvptslAudioProcessorEditor::BandEditor::setGain (float gainToUse)
{
    Gain1.setValue (gainToUse, sendNotification);
}

