/**
 * \file VariableDelayLineFilter.h
 */

#ifndef ATK_DELAY_VARIABLEDELAYFILTER_H
#define ATK_DELAY_VARIABLEDELAYFILTER_H

#include <vector>

#include <ATK/Core/TypedBaseFilter.h>
#include "config.h"

namespace ATK
{
  /// Variable delay line. Second port drives the delay. This delay must always be lower than max_delay
  template<typename DataType_>
  class ATK_DELAY_EXPORT VariableDelayLineFilter final : public TypedBaseFilter<DataType_>
  {
    class VDLF_Impl;
  protected:
    /// Simplify parent calls
    typedef TypedBaseFilter<DataType_> Parent;
    using typename Parent::DataType;
    using Parent::converted_inputs;
    using Parent::outputs;
    using Parent::nb_input_ports;
    using Parent::nb_output_ports;
    using Parent::output_delay;

  public:
    /*!
    * @brief construct the filter with a maximum delay line size
    * @param max-delay is the maximum delay allowed
    */
    VariableDelayLineFilter(std::size_t max_delay);
    /// Destructor
    ~VariableDelayLineFilter();

    virtual void full_setup() override final;
  protected:
    virtual void process_impl(std::size_t size) const override final;

  private:
    std::unique_ptr<VDLF_Impl> impl;
    
    /// Max delay for the delay line
    std::size_t max_delay;
  };
}

#endif
