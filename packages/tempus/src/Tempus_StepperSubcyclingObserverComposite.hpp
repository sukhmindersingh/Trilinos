// @HEADER
// ****************************************************************************
//                Tempus: Copyright (2017) Sandia Corporation
//
// Distributed under BSD 3-clause license (See accompanying file Copyright.txt)
// ****************************************************************************
// @HEADER

#ifndef Tempus_StepperSubcyclingObserverComposite_hpp
#define Tempus_StepperSubcyclingObserverComposite_hpp

#include "Tempus_StepperSubcyclingObserver.hpp"
#include "Tempus_SolutionHistory.hpp"


namespace Tempus {

// Forward Declaration for recursive includes (this Observer <--> Stepper)
template<class Scalar> class StepperSubcycling;

/** \brief StepperSubcyclingObserver class for StepperSubcycling.
 *
 * This is a means for application developers to perform tasks
 * during the time steps, e.g.,
 *   - Compute specific quantities
 *   - Output information
 *   - "Massage" the working solution state
 *   - ...
 *
 * <b>Design Considerations</b>
 *   - StepperSubcyclingObserver is not stateless!  Developers may touch the
 *     solution state!  Developers need to be careful not to break the
 *     restart (checkpoint) capability.
 */
template<class Scalar>
class StepperSubcyclingObserverComposite
 : virtual public Tempus::StepperSubcyclingObserver<Scalar>
{
public:

  /// Constructor
  StepperSubcyclingObserverComposite(){}

  /// Destructor
  virtual ~StepperSubcyclingObserverComposite(){}

  /// Observe Stepper at beginning of takeStep.
  virtual void observeBeginTakeStep(
    Teuchos::RCP<SolutionHistory<Scalar> > sh ,
    Stepper<Scalar> &  stepper )
  {
    for(auto&o : observers_)
      o->observeBeginTakeStep(sh, stepper);
  }

  /// Observe Stepper at end of takeStep.
  virtual void observeEndTakeStep(
    Teuchos::RCP<SolutionHistory<Scalar> > sh , Stepper<Scalar> &  stepper)
  {
    for(auto&o : observers_)
      o->observeEndTakeStep(sh, stepper);
  }

  // Add observer to the composite list
  void addObserver(const Teuchos::RCP<StepperSubcyclingObserver<Scalar> > &observer)
  {  observers_.push_back(observer); }

  // clear all observer from the composite observer list
  void clearObservers() { observers_.clear(); }

  // get the number of RK stepper observers present in the composite
  std::size_t getSize() const { return observers_.size(); }


private:

std::vector<Teuchos::RCP<StepperSubcyclingObserver<Scalar> > > observers_;

};

} // namespace Tempus
#endif // Tempus_StepperSubcyclingObserver_hpp
