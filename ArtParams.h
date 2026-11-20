#pragma once
#include <windows.h>
#include <cstdint>
#include <cstdio>
#include <cstdarg>

// ASM converted from SPU to CBU / Parameter (x64) from art/MessageParams.h.
namespace ArtParams {

enum Type : int { kInvalid = 0, kInt, kFloat, kBool, kString, kVector3, kReference, kUnknown };

struct Parameter {
    int  type;
    char name[32];
    union alignas(8) {
        int         i;
        float       f;
        bool        b;
        char        s[32];
        float       vec[3];
        const void* r;
    } v;
};
struct Base {
    Parameter* params;
    int        maxCount;
    int        usedCount;
    bool       overflow;
};

static_assert(sizeof(Parameter) == 72, "Parameter layout mismatch");
static_assert(offsetof(Parameter, v) == 40, "Parameter value offset mismatch");
static_assert(sizeof(Base) == 24, "MessageParamsBase layout mismatch");

inline bool InUserRange(const void* p) {
    return (uint64_t)p > 0x10000 && (uint64_t)p < 0x00007FFFFFFFFFFFull;
}

inline bool LooksLikeParams(const void* p) {
    if (!InUserRange(p) || ((uint64_t)p & 7)) return false;
    __try {
        const Base* b = (const Base*)p;
        if (!InUserRange(b->params) || ((uint64_t)b->params & 7)) return false;
        if (b->maxCount < 1 || b->maxCount > 256) return false;
        if (b->usedCount < 0 || b->usedCount > b->maxCount) return false;
        if (*(const uint8_t*)&b->overflow > 1) return false;
        volatile char probe = *(const char*)(b->params + b->maxCount - 1); // probe something in the array to ensure it's readable?
        (void)probe;
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}

inline void Append(char* out, int cap, int& pos, const char* fmt, ...) {
    if (pos >= cap - 1) return;
    va_list ap;
    va_start(ap, fmt);
    int n = _vsnprintf_s(out + pos, cap - pos, _TRUNCATE, fmt, ap);
    va_end(ap);
    pos += (n < 0) ? (cap - pos - 1) : n;
}

inline bool Format(const void* p, char* out, int cap) {
    int pos = 0;
    out[0] = 0;
    __try {
        const Base* b = (const Base*)p;
        Append(out, cap, pos, " {");
        for (int i = 0; i < b->usedCount; ++i) {
            const Parameter& q = b->params[i];
            Append(out, cap, pos, "%s%.31s=", i ? ", " : "", q.name);
            switch (q.type) {
            case kInt:       Append(out, cap, pos, "%d", q.v.i); break;
            case kFloat:     Append(out, cap, pos, "%g", q.v.f); break;
            case kBool:      Append(out, cap, pos, "%s", q.v.b ? "true" : "false"); break;
            case kString:    Append(out, cap, pos, "\"%.31s\"", q.v.s); break;
            case kVector3:   Append(out, cap, pos, "(%g, %g, %g)", q.v.vec[0], q.v.vec[1], q.v.vec[2]); break;
            case kReference: Append(out, cap, pos, "ref:%p", q.v.r); break;
            default:         Append(out, cap, pos, "?type%d", q.type); break;
            }
        }
        Append(out, cap, pos, b->overflow ? ", OVERFLOW}" : "}");
    }
    __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
    for (int i = 0; out[i]; ++i)
        if ((unsigned char)out[i] < 0x20 || (unsigned char)out[i] >= 0x7F) out[i] = '?';
    return true;
}

inline int Describe(void* const* candidates, int count, char* out, int cap) {
    for (int i = 0; i < count; ++i)
        if (LooksLikeParams(candidates[i]) && Format(candidates[i], out, cap)) return i;
    return -1;
}

} 