#ifndef UTILITIES_H
#define UTILITIES_H


#if __cplusplus > 199711L
    #define CPLUSPLUS11
#endif // __cplusplus < 199711L

///
/// \brief delete_safe
/// \param t
/// \return
///
/// Safely deallocates a memory block/object
///
/// Note 1:
///    Templates are automatically and necessarily inlined,
///    IIRC, unless (partially) specialized.
/// Note 2:
///    It is necessary to pass a reference to the pointer
///    or no changes to its value will be applied
///    after deallocating the memory block
///
template <typename T>
void delete_safe(T*& t)
{
    delete t;
#if defined CPLUSPLUS11
    t = nullptr;
#else
    t = 0;
#endif // CPLUSPLUS11
}

///
/// \brief delete_array_safe
/// \param t
/// \return
///
/// Safely deallocates a memory block/array
///
/// Note 1:
///    Templates are automatically and necessarily inlined,
///    IIRC, unless (partially) specialized.
/// Note 2:
///    It is necessary to pass a reference to the pointer
///    or no changes to its value will be applied
///    after deallocating the memory block
///
template <typename T>
void delete_array_safe(T*& t)
{
    delete[] t;
#if defined CPLUSPLUS11
    t = nullptr;
#else
    t = 0;
#endif // CPLUSPLUS11
}

#endif // UTILITIES_H
