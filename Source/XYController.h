/*
  ==============================================================================

    XYController.h
    Created: 15 Jun 2020 6:27:21pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "PluginProcessor.h"
#include "XYDot.h"

//--------------------------------------------------------------------
class XYController: public Component, public ChangeBroadcaster, public AudioProcessorValueTreeState::Listener
{
    
public:
    XYController(AudioProcessorValueTreeState& stateToControl,
                    const size_t& numberOfDots);
    
       
       void paint(Graphics& g) override;
    

       const std::vector<std::unique_ptr<Component>>& getDotVector() {return dotVector;}
       
       //Called if a child component is moved or resized, like by mouse dragging
       void childBoundsChanged(Component* component) override {
           // A lambda that ensures the center of the dot lays inside the xy controller
           auto bindDot = [&](auto*dot){
               dot->setCentrePosition(getBounds().getConstrainedPoint(dot->getBounds().getCentre()));
           };
           
           // A lambda that sets the given param values based on the x and y position of the dot
           auto setDotPosition = [&](auto* dot, String xParamID, String yParamID){
               const auto xParamValue = static_cast<float>(dot->getBounds().getCentreX())/getWidth();
               const auto yParamValue = static_cast<float>(dot->getBounds().getCentreY())/getHeight();
               
               //Find the index of the current dot+1 so that we can affect the band we want
               size_t index = 1+std::distance(dotVector.begin(),
                                              std::find_if(dotVector.begin(), dotVector.end(),
                                                           [&](auto& selectedDot)
                                                               {return selectedDot.get() == dot;}));
               
//               std::cout << index << std::endl;
               selectedDot = (int)index;
//               std::cout << selectedDot << std::endl;
               
               //update parameter & slider values,
               //but only if we've caused the change by using the xycontroller
               
               
                  //Casting index to an int because String's += doesn't work on size_t
               
               if(mousing) {
                   if(auto* xParam = parameterTree.getParameter(xParamID+=(int)index))
                       xParam->setValueNotifyingHost(xParamValue);
            
                   
                   if(auto* yParam = parameterTree.getParameter(yParamID+=(int)index))
                       yParam->setValueNotifyingHost(yParamValue);

               }
           };
           
           //If the child component is a type of dot,
           //update the freq and gain slider,
           //and constrain its position to inside this component's bounds
           //The if statement below sets a pointer dot equal to the result of a dynamic cast
           //if the result is a nullptr, the pointer evaluates to false in a boolean context
           //so we don't take this branch
           //otherwise, the variable is then useable inside of the if block
           if(auto* dot = dynamic_cast<XYDot*>(component)) {
               setDotPosition(dot, "Freq", "Gain");
               bindDot(dot);
           }
           //Gotta check for XY and XYR separately, otherwise we don't know which type passes the cast
           else if(auto* dot = dynamic_cast<XYRDot*>(component)) {
               setDotPosition(dot, "Freq", "Gain");
               bindDot(dot);
           }
       }
       
       //update the position of the relavent dot when a parameter changes
       //Moving the sliders, moving the dot, and automation all update the params
       void parameterChanged (const String& paramID, float newValue) override {
           //If we're mousing around in the xycontroller, we don't want to receive changes to the dot position
           //Put bluntly, if the DAW/Sliders and the user are fighting over the controls, the user wins
           if(!mousing)
               //If we're to update, we want to do so asynchronously
               //This is because this is doesn't need to be updated very quickly (in the audio sense)
               //And because, since this function can trigger a resize call,
               //we don't want to call it on the audio thread
               MessageManager::callAsync([&, paramID, newValue](){
                   //first, check to make sure the component that called this still exists
                   if(!thisPointer.getComponent())
                       return;

                   //Get the paramID without the number to determine which axis to move on
                   //Then, use the ID to get the desired dot
                   const auto paramIDType = paramID.dropLastCharacters(1);
                   if(paramIDType == "Gain" || paramIDType == "Freq") {
                       //A lambda to get the dot position
                       //This just gets the normalized parameter value
                       const auto getDotPosition = [&](){
                           return parameterTree.getParameter(paramID)->getNormalisableRange().convertTo0to1(newValue);
                       };
                       //Freq and Gain are both 4 letters. We want to grab the rest of the string as an int
                       const auto index = paramID.substring(4, paramID.length()).getIntValue()-1;
                       //If it's a gain parameter, we want to position the dot on the y axis
                       //We set the dot's center position to it's current position offset by the right gain amount
                       //Maximum gain will draw at the top of the component
                       //Minimum gain at the bottom
                       if(paramIDType == "Gain")
                           //set the vertical position of the dot
                           dotVector[index]->setCentrePosition(
                              dotVector[index]->getBounds()
                              .getCentre().withY((1.0-getDotPosition())*getHeight()));
                       //Otherwise, it's a freq parameter, and we want to position the dot on the x axis
                       //We set the dot's center position to it's current position offset by the right freq amount
                       //Maximum freq will draw at the left of the component
                       //Minimum freq at the right
                       else
                           //set the horizontal position of the dot
                           dotVector[index]->setCentrePosition(
                            dotVector[index]->getBounds()
                            .getCentre().withX(getDotPosition()*getWidth()));
                   }
               }
           );
       };
    
    
//   private:
    
       //Find the closest dot in the dot vector
       auto* getClosestDot(juce::Point<int> point) {
           return std::min_element(dotVector.begin(), dotVector.end(), [&](const auto& dot1, const auto& dot2){
               // for every dot in the dot vector, check which dot has the closest frequency (x co-ordinate)
               // Because, when you click, you want to move the closest filter.
               // Filters don't get closer to eachother by changing gain, they get closer by changing frequency
               return std::abs(point.x-dot1->getBounds().getCentreX())
                    < std::abs(point.x-dot2->getBounds().getCentreX());
           })->get();
       }

       
       void mouseDown (const MouseEvent& e) override
       {
           //set the mousing flag to true
           //this is used to block updates from the parameter/sliders from moving the dot
           //while the user is interacting with it


           mousing = true;
           
   

           
           //get the position of where the mouse was clicked, relative to this component's bounds
           const auto mousePosition = e.getEventRelativeTo(this).position.toInt();
           //We do all operations on the closest dot.
           //This removes ambiguity as to which dot is chosen when several dots are within eachothers bounds
           activeDot = getClosestDot(mousePosition);
           //If the active dot is an XYR dot, and we clicked directly on it,
           //we need to do some special stuff
           if(dynamic_cast<XYRDot*>(activeDot)
              && activeDot->getBounds().contains(mousePosition)) {
                   //XYR dots handle their own mouse clicks,
                   //so we set a flag that indicates we want to forward mouse events to the dot
                   delegated = true;
                   dynamic_cast<XYRDot*>(activeDot)->mouseDown(e);
               }
               else {
                   //If we didn't click on an XYRdot, we handle the mouse events
                   delegated = false;
                   //Set the dot's centre position to where the click happened in this component
                   activeDot->setCentrePosition(mousePosition);
               }
        
              
              sendChangeMessage();
           
//           mousing = false;
       }

       void mouseDrag (const MouseEvent& e) override {
           //If the active dot doesn't exist or some other weird thing, we stop
           if(activeDot == nullptr)
               return;
           if(delegated)
               //If we're delegating the mouse events, pass the event to the dot
               activeDot->mouseDrag(e);
           else
               //Otherwise, move the dot to the cursor
               activeDot->setCentrePosition(e.getEventRelativeTo(this).position.toInt());
       }

       void mouseUp (const MouseEvent& e) override {
           if(activeDot != nullptr && delegated)
               //If we've been sending events to the dot, send it a mouse up
               activeDot->mouseUp(e);
           mousing = delegated = false;
       }
    
//    class Listener
//    {
//    public:
//        /** Destructor. */
//        virtual ~Listener() = default;
//
//        /** Called when the button is clicked. */
//        virtual void XYClicked (XYController*) = 0;
//
//    };
//
//
//    void addListener (Listener* newListener);
//
//
//    void removeListener (Listener* listener);
//
    
//    ListenerList<Listener> XYListeners;
       
       AudioProcessorValueTreeState& parameterTree;
       std::vector<std::unique_ptr<Component>> dotVector;
       Component* activeDot;
       SafePointer<Component> thisPointer{this};
       bool delegated{false}, mousing{false};
    
        int selectedDot;
    
    
   };



//struct XYListener
//{
//    XYListener();
//    virtual ~XYListener() = default;
//
//    virtual void XYClicked (XYController*) = 0;
//
//};

