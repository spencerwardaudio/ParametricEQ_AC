/*
  ==============================================================================

    XYController.cpp
    Created: 15 Jun 2020 6:27:21pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "XYController.h"

XYController::XYController(AudioProcessorValueTreeState& stateToControl,
                           const size_t& numberOfDots)
    : parameterTree(stateToControl)
{
    
    dotVector.reserve(numberOfDots);
    //for every dot we want to add
    for (auto i = 0; i < numberOfDots; ++i) {
        //move it into the back of the vector
        dotVector.push_back(std::make_unique<XYRDot>(i+1,
                                                     Colours::skyblue,
                                                     stateToControl,
                                                     String{"Q"}+=(i+1))
                            );
        //give it some bounds
        dotVector.back()->setSize(50,50);
        //display it
        
        dotVector.back()->setTopLeftPosition(i*105, 50);
        
        
        addAndMakeVisible(dotVector.back().get());
        //if the dot has a slider,
        //add a mouse listener to it for this component
        //so that it can get mouse clicks and not have them be automatically delegated to the knob
        if(auto* p = dynamic_cast<XYRDot*>(dotVector.back().get()))
            dotVector.back()->addMouseListener(this, true);
        //and make sure its child components receive clicks
//        dotVector.back()->setInterceptsMouseClicks (false, true);
        
        //Add listeners for this band's gain an frequency params
        parameterTree.addParameterListener(String{"Gain"}+=i+1, this);
        parameterTree.addParameterListener(String{"Freq"}+=i+1, this);
    }

    
}

void XYController::paint(Graphics& g) {
    g.setColour(Colours::black.withAlpha(.3f));
    g.fillRect(getBounds().reduced(1));
}

//
//void XYController::addListener (Listener* l)      { XYListeners.add (l); }
//void XYController::removeListener (Listener* l)   { XYListeners.remove (l); }
