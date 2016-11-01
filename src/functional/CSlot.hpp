//
//  CSlot.hpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/20/16.
//
//

#ifndef CSLOT_H
#define CSLOT_H

template <typename R, typename... Arg>
class CSlot {
public:
    typedef R(TSignature)(Arg...);

public:
    CSlot()
        : m_Invoker()
    {
    }

    template <typename F>
    CSlot(F function)
        : m_Invoker(new CSimpleFunctionHolder<F>(function))
    {
    }

    template <typename F, typename C>
    CSlot(F C::*member)
        : m_Invoker(new CMemberFunctionHolder<F, Arg...>(member))
    {
    }

    CSlot(const CSlot& other)
        : m_Invoker(other.m_Invoker->Clone())
    {
    }

    CSlot& operator=(const CSlot& other)
    {
        m_Invoker = other.m_Invoker->Clone();

        return *this;
    }

    R operator()(Arg... args) const
    {
        return m_Invoker->Invoke(args...);
    }

    bool operator==(const CSlot& other) const
    {
        return Address() == other.Address();
    }

    uintptr_t Address() const
    {
        return m_Invoker->Address();
    }

private:
    /*
     * Base function holder
     */
    class CFunctionHolderBase {
    public:
        CFunctionHolderBase() = default;
        virtual ~CFunctionHolderBase() = default;

        virtual R Invoke(Arg... args) const = 0;
        virtual std::auto_ptr<CFunctionHolderBase> Clone() = 0;
        virtual uintptr_t Address() const = 0;

        CFunctionHolderBase(const CFunctionHolderBase& other) = delete;
        void operator=(const CFunctionHolderBase& other) = delete;
    };

    typedef std::auto_ptr<CFunctionHolderBase> TInvoker;

    /*
     * Simple function holder
     */
    template <typename F>
    class CSimpleFunctionHolder : public CFunctionHolderBase {
    public:
        CSimpleFunctionHolder(const F& function)
            : CFunctionHolderBase()
            , m_Function(function)
        {
        }

        virtual R Invoke(Arg... args) const
        {
            return m_Function(args...);
        }

        virtual TInvoker Clone()
        {
            return TInvoker(new CSimpleFunctionHolder(m_Function));
        }

        virtual uintptr_t Address() const
        {
            return reinterpret_cast<uintptr_t>(&m_Function);
        }

    private:
        F m_Function;
    };

    /*
     * Member function holder
     */
    template <typename F, typename C, typename... Args>
    class CMemberFunctionHolder : public CFunctionHolderBase {
    public:
        typedef F C::*TMemberSignature;

        CMemberFunctionHolder(const TMemberSignature& function)
            : m_Function(function)
        {
        }

        virtual R Invoke(C obj, Args... args) const
        {
            return (obj.*m_Function)(args...);
        }

        virtual TInvoker Clone()
        {
            return TInvoker(new CMemberFunctionHolder(m_Function));
        }

        virtual uintptr_t Address() const
        {
            return reinterpret_cast<uintptr_t>(m_Function);
        }

    private:
        TMemberSignature m_Function;
    };

    TInvoker m_Invoker;
};

#endif /* CSLOT_H */
