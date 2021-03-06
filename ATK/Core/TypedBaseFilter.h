/**
 * \file TypedBaseFilter.h
 */

#ifndef ATK_CORE_TYPEDBASEFILTER_H
#define ATK_CORE_TYPEDBASEFILTER_H

#include "BaseFilter.h"

#include <memory>
#include <vector>

#include <boost/align/aligned_allocator.hpp>

namespace ATK
{
  /// Base class for typed filters, contains arrays
  template<typename DataType_, typename DataType__ = DataType_>
  class ATK_CORE_EXPORT TypedBaseFilter : public BaseFilter
  {
  protected:
    /// Simplify parent calls
    typedef BaseFilter Parent;
  public:
    /// To be used by inherited APIs
    typedef DataType_ DataType;
    /// To be used by inherited APIs
    typedef DataType_ DataTypeInput;
    /// To be used by inherited APIs
    typedef DataType__ DataTypeOutput;
    /// To be used for filters that require alignment (like EQs)
    typedef std::vector<DataType, boost::alignment::aligned_allocator<DataType, 32> > AlignedVector;

    /// Base constructor for filters with actual data
    TypedBaseFilter(std::size_t nb_input_ports, std::size_t nb_output_ports);
    /// Move constructor
    TypedBaseFilter(TypedBaseFilter&& other);
    /// Destructor
    virtual ~TypedBaseFilter();
    
    TypedBaseFilter(const TypedBaseFilter&) = delete;
    TypedBaseFilter& operator=(const TypedBaseFilter&) = delete;

    /**
     * @brief Returns an array with the processed output
     * @param port is the port that the next plugin listens to
     */
    DataType__* get_output_array(std::size_t port) const;
    std::size_t get_output_array_size() const;

    virtual void set_nb_input_ports(std::size_t nb_ports) override;
    virtual void set_nb_output_ports(std::size_t nb_ports) override;

    virtual void full_setup() override;

    /// Connects this filter input to another's output
    virtual void set_input_port(std::size_t input_port, BaseFilter* filter, std::size_t output_port) override final;
    
  private:
    virtual int get_type() const override;
  protected:
    /// This implementation does nothing
    virtual void process_impl(std::size_t size) const override;
    /// Prepares the filter by retrieving the inputs arrays
    virtual void prepare_process(std::size_t size) override final;
    /// Prepares the filter by resizing the outputs arrays
    virtual void prepare_outputs(std::size_t size) override final;
    
    /// Used to convert other filter outputs to DataType*
    void convert_inputs(std::size_t size);

    /// Input arrays with the input delay, owned here
    std::vector<std::unique_ptr<DataTypeInput[]> > converted_inputs_delay;
    /// Input arrays, starting from t=0 (without input delay)
    std::vector<DataTypeInput*> converted_inputs;
    /// Current size of the input arrays, without delay
    std::vector<std::size_t> converted_inputs_size;

    /// Output arrays with the output delay, owned here
    std::vector<std::unique_ptr<DataTypeOutput[]> > outputs_delay;
    /// Output arrays, starting from t=0 (without output delay)
    std::vector<DataTypeOutput*> outputs;
    /// Current size of the output arrays, without delay
    std::vector<std::size_t> outputs_size;

    /// A vector containing the default values for the input arrays
    std::vector<DataTypeInput> default_input;
    /// A vector containing the default values for the output arrays
    std::vector<DataTypeOutput> default_output;
  };
}

#endif
