// This general interpreter interface.
// Author: Tim Eves

// Build one of direct_machine.cpp or call_machine.cpp to implement this 
// interface.

#pragma once
#include <graphiteng/Types.h>

//#define CHECK_STACK   TRUE
#if defined(__GNUC__)
#define     HOT             __attribute__((hot))
#define     REGPARM(n)      __attribute__((hot, regparm(n)))
#else
#define     HOT
#define     REGPARM(n)
#endif

// Forward declarations
class Segment;

namespace vm 
{


typedef void *  instr;

enum {VARARGS = size_t(-1), MAX_NAME_LEN=32};

struct opcode_t 
{ 
    instr           impl[2];
    size_t          param_sz;
    int             stack_delta;
    char            name[MAX_NAME_LEN];
};

enum opcode {
    NOP = 0,

    PUSH_BYTE,      PUSH_BYTEU,     PUSH_SHORT,     PUSH_SHORTU,    PUSH_LONG,

    ADD,            SUB,            MUL,            DIV,
    MIN_,           MAX_,
    NEG,
    TRUNC8,         TRUNC16,

    COND,

    AND,            OR,             NOT,
    EQUAL,          NOT_EQ,
    LESS,           GTR,            LESS_EQ,        GTR_EQ,

    NEXT,           NEXT_N,         COPY_NEXT,
    PUT_GLYPH_8BIT_OBS,              PUT_SUBS_8BIT_OBS,   PUT_COPY,
    INSERT,         DELETE,
    ASSOC,
    CNTXT_ITEM,

    ATTR_SET,       ATTR_ADD,       ATTR_SUB,
    ATTR_SET_SLOT,
    IATTR_SET_SLOT,
    PUSH_SLOT_ATTR,                 PUSH_GLYPH_ATTR_OBS,
    PUSH_GLYPH_METRIC,              PUSH_FEAT,
    PUSH_ATT_TO_GATTR_OBS,          PUSH_ATT_TO_GLYPH_METRIC,
    PUSH_ISLOT_ATTR,

    PUSH_IGLYPH_ATTR,    // not implemented

    POP_RET,                        RET_ZERO,           RET_TRUE,
    IATTR_SET,                      IATTR_ADD,          IATTR_SUB,
    PUSH_PROC_STATE,                PUSH_VERSION,
    PUT_SUBS,                       PUT_SUBS2,          PUT_SUBS3,
    PUT_GLYPH,                      PUSH_GLYPH_ATTR,    PUSH_ATT_TO_GLYPH_ATTR,
    MAX_OPCODE
};

class Machine
{
public:
    enum status_t {
        finished,
        stack_underflow,
        stack_not_empty,
        stack_overflow
    };
    
    static const opcode_t *   getOpcodeTable() throw();
    static int32              run(const instr * program, const byte * data,
                                   int32 * stack_base, const size_t length,
                                   Segment & seg, int & islot_idx,
                                   status_t &status) HOT;

protected:
    static bool check_stack(const int32 * const sp, 
                     const int32 * const base,
                     const int32 * const limit) REGPARM(3);

    static bool check_final_stack(const int32 * const sp, 
                           const int32 * const base,
                           const int32 * const limit,
                           status_t &status);
    
};

inline bool Machine::check_stack(const int32 * const sp, 
                                 const int32 * const base,
                                 const int32 * const limit) {
    return (sp <= base && sp > limit);
}

inline bool Machine::check_final_stack(const int32 * const sp, 
                                       const int32 * const base,
                                       const int32 * const limit,
                                       status_t & status) {
    if (sp != base) {
        if (sp > base)
            status = stack_underflow;
        else if (sp <= limit)
            status = stack_overflow;
        else 
            status = stack_not_empty;
        return false;
    }
    status = finished;
    return true;
}


} // end of namespace vm

#ifdef ENABLE_DEEP_TRACING
#define STARTTRACE(name,is)     if (XmlTraceLog::get().active()) { \
				                    XmlTraceLog::get().openElement(ElementOpCode); \
				                    XmlTraceLog::get().addAttribute(AttrName, # name); \
				                    XmlTraceLog::get().addAttribute(AttrIndex, is); \
			                    }

#define ENDTRACE                XmlTraceLog::get().closeElement(ElementOpCode);
#else
#define STARTTRACE(name,is)
#define ENDTRACE
#endif


