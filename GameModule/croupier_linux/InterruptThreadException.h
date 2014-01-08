#ifndef INTERRUPTTHREADEXCEPTION_H
#define INTERRUPTTHREADEXCEPTION_H

class InterruptThreadException : public std::exception
{
    public:
        const char* what() const _GLIBCXX_USE_NOEXCEPT { return "interrupt"; }
};

#endif // INTERRUPTTHREADEXCEPTION_H
