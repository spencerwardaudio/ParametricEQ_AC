/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "XYController.h"
#include "Knobs.h"

#include "ACParameters.h"

#include "ACMainPanel.h"
#include "ACBottomRightPanel.h"
#include "ACLookAndFeel.h"

#include "ACAnalyzer.h"

//==============================================================================
/**
*/
class MultiAvptslAudioProcessorEditor  : public AudioProcessorEditor, /*public XYController::Listener, */ public ChangeListener, public Timer
{
public:
    MultiAvptslAudioProcessorEditor (MultiAvptslAudioProcessor&);
    ~MultiAvptslAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void changeListenerCallback (ChangeBroadcaster* sender) override;
//
//    void XYClicked (XYController* xyControl) override
//    {
//        if (xyControl == (mXYPoints.get()))
//        {
//            std::cout << " XYClicked " <<std::endl;
//        }
//    }
    
    void timerCallback() override;
    
//    void XYClicked(XYController *xyPoint) override;
//
//    bool XYControllerClicked {false};
//
//    void updateSliderVisibility();
//
//
    

    class BandEditor : public Component
    {
    public:
        BandEditor(size_t i, MultiAvptslAudioProcessor& processor);

        void resized () override;

        void setFrequency (float freq);

        void setGain (float Gain);

        void setType (int type);

        Path frequencyResponse;
        
        
        Slider              frequencyCutoff1, frequencyCutoff2, frequencyCutoff3, frequencyCutoff4, frequencyCutoff5, Q1, Q2,Q3,Q4,Q5,Gain1,Gain2,Gain3, Gain4, Gain5;
        

        private:
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandEditor)

            size_t index;
            MultiAvptslAudioProcessor& processor;


            OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;
    };
    

private:

    void updateFrequencyResponses ();

    static float getPositionForFrequency (float freq);

    static float getFrequencyForPosition (float pos);

    OwnedArray<BandEditor>  bandEditors;

    Rectangle<int>          plotFrame;

    Path                    frequencyResponse;
    
    OwnedArray<XYController> mXYPoints;

    MultiAvptslAudioProcessor& processor;
    
    Path    analyzerPath;

    int                     draggingBand = -1;
    bool                    draggingGain = false;

    OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;

    std::unique_ptr<ACMainPanel> mMainPanel;
    std::unique_ptr<ACBottomRightPanel> mBottomRightPanel;
    std::unique_ptr<ACLookAndFeel> mLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiAvptslAudioProcessorEditor)
};
