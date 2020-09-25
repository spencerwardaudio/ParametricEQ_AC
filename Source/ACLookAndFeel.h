/*
  ==============================================================================

    ACLookAndFeel.h
    Created: 2 Aug 2020 2:25:13pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ACInterfaceDefines.h"

class ACLookAndFeel
: public LookAndFeel_V4
{
public:
    ACLookAndFeel()
    {
        // store image assets
        mSliderImage = ImageCache::getFromMemory(BinaryData::AC_knob_png, BinaryData::ACSize_bg_pngSize / 2);
     
        // comboBox colours
        setColour(ComboBox::backgroundColourId, ACColour_3);
        setColour(ComboBox::outlineColourId, ACColour_2);
        setColour(ComboBox::arrowColourId, ACColour_1);
        setColour(ComboBox::textColourId, ACColour_1);

        //button text colours
        setColour(TextButton::buttonColourId, ACColour_1);
        setColour(TextButton::textColourOnId, ACColour_1);
        setColour(TextButton::textColourOffId, ACColour_1);
        
        //slider text colour
        setColour(Slider::textBoxTextColourId, ACColour_8);
    }
    
    virtual ~ACLookAndFeel(){};
    
    /** buttons */
    Font getTextButtonFont (TextButton&, int buttonHeight) override
    {
        return font_1;
    }
    
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override
    {
        Colour fillColour;
        
        if(isButtonDown){
            fillColour = ACColour_6;
        } else if(isMouseOverButton){
            fillColour = ACColour_3;
        } else {
            fillColour = ACColour_5;
        }
        
        const float cornerSize = 6.0f;
        const juce::Rectangle<float> bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
        
        g.setColour(fillColour);
        g.fillRoundedRectangle(bounds.reduced(1), cornerSize);
    }
    
    /** comboBoxes */
    Font getLabelFont(Label& label) override
    {
        return font_1;
    }
    
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon, const Colour* textColour) override
    {
        juce::Rectangle<int> r (area);
        
        Colour fillColour = isHighlighted ? ACColour_5 : ACColour_4;
        
        g.setColour(fillColour);
        g.fillRect(r.getX(), r.getY(), r.getWidth(), r.getHeight() -1);
        
        Colour myTextColour = isTicked ? ACColour_7 : ACColour_1;
        g.setColour(myTextColour);
        g.setFont(font_1);
        
        r.setLeft(10);
        r.setY(1);
        g.drawFittedText(text, r, Justification::left, 1);
    }
    
    void drawComboBox (Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox& box) override
    {
        const float cornerSize = 3.0f;
        const juce::Rectangle<int> boxBounds (0, 0, width, height);
        
        g.setColour(ACColour_3);
        g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);
        
        juce::Rectangle<int> arrow (width - 30, 0, 20, height);
        
        Path path;
        
        path.startNewSubPath(arrow.getX() + 3.0f, arrow.getCentreY() - 2.0f);
        path.lineTo(arrow.toFloat().getCentreX(), arrow.toFloat().getCentreY() + 3.0f);
        path.lineTo(arrow.toFloat().getRight() - 3.0f, arrow.toFloat().getCentreY() - 2.0f);
        
        Colour arrowColour = box.findColour(ComboBox::arrowColourId).withAlpha(box.isEnabled() ? 0.9f : 0.2f);
        g.setColour(arrowColour);
        g.strokePath(path, PathStrokeType(2.0f));
        
    }
    
    /** sliders */
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, Slider& slider) override
    {
        const int numFrames = mSliderImage.getHeight() / mSliderImage.getWidth();
        const int frameIndex = (int)std::ceil(sliderPosProportional * (numFrames - 1));
        
        const float radius = jmin(width*0.5, height*0.5);
        const float centreX = x + width * 0.5;
        const float centreY = y + height * 0.5f;
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        
        
        g.drawImage(mSliderImage,
                    rx,
                    ry,
                    2*radius,
                    2*radius,
                    0,
                    frameIndex * mSliderImage.getWidth(),
                    mSliderImage.getWidth(),
                    mSliderImage.getWidth());
    }
    

    void drawLabel (Graphics& g, Label& label) override
{
    // g.fillAll (label.findColour (Label::backgroundColourId));
    g.setColour (Colours::skyblue);
    
    g.drawRoundedRectangle (10.f, 4.f, 60.f, 16.f, 3.f, 1.f);

    g.drawFittedText(label.getText(), 5, 0, 70, 25, Justification::centred, 1, 0.5f);
    
            g.setColour(label.findColour(Label::outlineColourId));
        if (!label.isBeingEdited())
        {
            auto alpha = label.isEnabled() ? 1.0f : 0.5f;
            const Font font(getLabelFont(label));

            g.setColour(label.findColour(Label::textColourId).withMultipliedAlpha(alpha));
            g.setFont(font);
        }

}
    
private:
    
    Image mSliderImage;
            
};

