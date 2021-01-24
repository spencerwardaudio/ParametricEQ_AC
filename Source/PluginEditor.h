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
class ParametricEQ_ACAudioProcessorEditor  : public AudioProcessorEditor, /*public XYController::Listener, */ public ChangeListener, public Timer
{
public:
    ParametricEQ_ACAudioProcessorEditor (ParametricEQ_ACAudioProcessor&);
    ~ParametricEQ_ACAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void changeListenerCallback (ChangeBroadcaster* sender) override;
    
    void bandVisible(int i, bool visible);
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
        BandEditor(size_t i, ParametricEQ_ACAudioProcessor& processor);

        void resized () override;

        void setFrequency (int band, float freq);

        void setGain (int band, float Gain);

        void setType (int type);

        Path frequencyResponse;
        
        std::vector<std::unique_ptr<Slider>> frequencyCutoffV;
        std::vector<std::unique_ptr<Slider>> qV;
        std::vector<std::unique_ptr<Slider>> gainV;
        
        std::vector<std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>> attachments;

        private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BandEditor)
        
        size_t index;
        ParametricEQ_ACAudioProcessor& processor;
    };
    

private:

    void updateFrequencyResponses ();

    static float getPositionForFrequency (float freq);

    static float getFrequencyForPosition (float pos);

    OwnedArray<BandEditor>  bandEditor;

    Rectangle<int>          plotFrame;

    Path                    frequencyResponse;
    
    OwnedArray<XYController> mXYPoints;

    ParametricEQ_ACAudioProcessor& processor;
    
    Path    analyzerPath;

    int                     draggingBand = -1;
    bool                    draggingGain = false;

    OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;

    std::unique_ptr<ACMainPanel> mMainPanel;
    std::unique_ptr<ACBottomRightPanel> mBottomRightPanel;
    std::unique_ptr<ACLookAndFeel> mLookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParametricEQ_ACAudioProcessorEditor)
};
