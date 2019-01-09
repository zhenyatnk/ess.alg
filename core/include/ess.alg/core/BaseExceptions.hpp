#pragma once

#include <ess.alg/core/export.hpp>
#include <stdexcept>

namespace ess_alg {
namespace core {
namespace exceptions {

class ESS_ALG_CORE_EXPORT ess_alg_error_base
    :public std::runtime_error
{
public:
    ess_alg_error_base(const std::string &aMessage, const int &aErrorCode)
        :runtime_error(aMessage), m_ErrorCode(aErrorCode)
    {}

    int GetErrorCode() const
    {
        return m_ErrorCode;
    }

private:
    int m_ErrorCode;
};

}
}
}
