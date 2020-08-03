/*
  ==============================================================================

    XYDot.h
    Created: 15 Jun 2020 6:27:48pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "Knobs.h"

class XYDot : public Component
{
public:
    XYDot(const size_t& initialNumber, const Colour& initialColor)
    : number(initialNumber), color(initialColor)
    {}
    
    void setColour (Colour col) { color = col; }
    
    void setNumber(const String& newNumber) { number = newNumber; }
    
    void mouseDown (const MouseEvent& e) override {
        // if the mouse was clicked inside the dot, move the center of the dot to the mouse position
        if(this->contains(e.position.toInt()))
            setCentrePosition(e.position.toInt()+getBounds().getTopLeft());
        
    }

    void mouseDrag (const MouseEvent& e) override {
        // if the mouse was dragged after clicking inside the dot,
        // move the center of the dot to the mouse position
        // A drag started outside this component will never call this.
        setCentrePosition(e.position.toInt()+getBounds().getTopLeft());
    }
    
    void paint (Graphics& g)  override {
        //Draw a circle that fills this component's bounds
        g.setColour (color);
        // line thickness is 3, so the circle's bounds should start from 1.5
        // because co-ordinates for drawEllipse specify where the middle of the line should go,
        // so they need to be inset so that the full line is drawn inside the component bounds
        // I round up to because sometimes using float values causes problems.
        // -4 at the end because you're specifying the width and height, not the bottom right point.
        const Rectangle<int> ellipseBounds{2, 2, getWidth()-4, getHeight()-4};
        g.drawEllipse(ellipseBounds.toFloat(), 3);
        //Same deal with the text. Justify it as center to get the number
        g.drawText(number, ellipseBounds, Justification::centred);
    }
private:
    String number{};
    Colour color{};
};

//----------------------------------------------------------------------

class XYRDot : public Component
{
public:
    XYRDot(const size_t& initialNumber, const Colour& initialColor,
           AudioProcessorValueTreeState& parameterTree, const String& id)
        : dot(initialNumber, initialColor), qKnob(parameterTree, id)
    {
        //Make the dot visible under the knob so that you can't click the dot and not the slider.
        addAndMakeVisible(&dot);
        qKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        qKnob.setColour(Slider::rotarySliderFillColourId, Colours::transparentWhite);
        qKnob.setColour(Slider::rotarySliderOutlineColourId, Colours::transparentWhite);
        qKnob.setColour(Slider::thumbColourId, Colours::skyblue);
        qKnob.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        addAndMakeVisible(&qKnob);
    }
    
    void resized() override {
        //slider seems to fill the bounds when given the bounds expanded by 5
        qKnob.setBounds(getBounds().expanded(5));
        //dot fits inside the slider when its bounds are inset by 18
        dot.setBounds(qKnob.getBounds().reduced(18));
        //tbh I don't think this will stay working if the size of the dot changes
    }
    
    void setColour(const Colour& c) noexcept {dot.setColour(c);}
    
    void mouseDown(const MouseEvent& e) override {
        const auto clickPosition = e.getEventRelativeTo(this).position.toInt();
        //<ake a line from the center of the dot to the click position
        Line<int> l{dot.getBounds().getCentre(), clickPosition};
        //If we click far enough away from the center, we want to move the slider
        if(l.getLength() > dot.getWidth()*.45f && l.getLength() < getWidth()) {
            usingSlider = true;
            qKnob.mouseDown(e);
        }
        //Otherwise we want to drag the dot
        else {
            usingSlider = false;
            setCentrePosition(clickPosition+getBounds().getTopLeft());
        }
    }
    
    void mouseDrag(const MouseEvent& e) override {
        //move the slider or the dot, depending on where the initial click happened
        if(usingSlider)
            qKnob.mouseDrag(e);
        else
            setCentrePosition(e.position.toInt()+getBounds().getTopLeft());
    }
    
    void mouseUp(const MouseEvent& e) override {
        //If we were using the slider, tell it we're done.
        //No need to reset the usingSlider flag because it gets set every time in mouse down.
        if(usingSlider)
            qKnob.mouseUp(e);
        usingSlider = false;
    }
    
    void setNumber(const String& number){
        dot.setNumber(number);
    }
    
private:
    XYDot dot;
    AttatchedKnob<Slider> qKnob;
//    Slider qKnob{};
    bool usingSlider{false};
    
};
