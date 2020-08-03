/*
  ==============================================================================

    Knobs.h
    Created: 15 Jun 2020 6:28:01pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

// wrapper for a slider that adds a label below the slider that displays the slider's name
// Stolen from my FM synth
template<typename KnobType>
class LabeledKnob : public Component
{
public:
    LabeledKnob(const String& Name,
                const String& textBoxSuffix = "",
                const String& tooltipText = "")
    : knob(Slider::RotaryHorizontalVerticalDrag,
           Slider::TextEntryBoxPosition::TextBoxBelow)
    {
        static_assert(std::is_base_of<juce::Slider, KnobType>::value, "KnobType needs to be some sort of slider!");
        addAndMakeVisible(&knob);
        addAndMakeVisible(&label);

        knob.setTooltip(tooltipText);
        knob.setTextValueSuffix(textBoxSuffix);
        knob.setTextBoxIsEditable(true);
        
        label.setJustificationType(Justification::centred);
        
        label.setColour(Label::textColourId, Colours::skyblue);
        label.setFont (Font (16.0f, Font::bold));
        label.setText(Name, NotificationType::dontSendNotification);
        label.setTooltip(tooltipText);

    }

    void resized() override {
        auto bounds = this->getLocalBounds();
        const auto knobBounds = bounds.removeFromTop(roundToInt(bounds.getHeight() * .8f));
        knob.setBounds(knobBounds);
        label.setBounds(bounds);
    }

private:
    KnobType knob;
    Label label;
};

// A simple wrapper that subclasses some type, which should be a slider,
// and automatically creates and owns a slider attatchment for it.
template<typename KnobType>
class AttatchedKnob : public KnobType
{
public:
    //The constructor takes in a parameter pack of args- we don't know what the underlying slider needs for contructor args, and we don't care- we just pass along what we are given and expect the caller to pass in the correct args for the slider
    template<typename... Args>
    AttatchedKnob(AudioProcessorValueTreeState& parameterTree, const String& id, Args&&... args) :
    KnobType(args...)
    {
        //If this is a slider, attatch it to the tree
        if(auto* slider = dynamic_cast<Slider*>(this))
            attatchment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(parameterTree, id, *slider);
        //Otherwise, if a child component is a slider, attatch it to the tree
        else
            for(auto* comp : this->getChildren())
                if(auto* slider = dynamic_cast<Slider*>(comp))
                   attatchment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(parameterTree,
                                                                                        id,
                                                                                        *slider);
    }
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> attatchment;
};

//A component that holds two attatched knobs
//Just a way to coalesce the resizing behavior of a common grouping of components
class KnobPair : public Component
{
public:
    KnobPair(AudioProcessorValueTreeState& parameterTree,
             const String& knob1ID, const String& knob2ID) :
    //Initialise the sliders with the given tree and param id
    knob1(parameterTree, knob1ID,
          parameterTree.getParameter(knob1ID)->name,
          parameterTree.getParameter(knob1ID)->label),
    knob2(parameterTree, knob2ID,
          parameterTree.getParameter(knob2ID)->name,
          parameterTree.getParameter(knob2ID)->label)
    {
        //Show the knobs
        addAndMakeVisible(&knob1);
        addAndMakeVisible(&knob2);
    }
    
    void resized() override {
        //make the knobs squares with whichever is smallest- half the width or the height
        const auto minSide{std::min(getHeight(), roundToInt(getWidth()*.5))};
        knob1.setSize(minSide, minSide);
        knob2.setSize(minSide, minSide);
        
        // both knobs are vertically centered evenly spaced horizontally
        knob1.setCentrePosition(getWidth()*.33, getHeight()*.5);
        knob2.setCentrePosition(getWidth()*.66, getHeight()*.5);
    }
    
private:
    AttatchedKnob<LabeledKnob<Slider>> knob1, knob2;
};
