/* File: tac.h
 * -----------
 * This module contains the Instruction class (and its various
 * subclasses) that represent Tac instructions and the Location
 * class used for operands to those instructions.
 *
 * Each instruction is mostly just a little struct with a
 * few fields, but each responds polymorphically to the methods
 * Print and Emit, the first is used to print liveout the TAC form of
 * the instruction (helpful when debugging) and the second to
 * convert to the appropriate MIPS assembly.
 *
 * The operands to each instruction are of Location class.
 * A Location object is a simple representation of where a variable
 * exists at runtime, i.e. whether it is on the stack or global
 * segment and at what offset relative to the current fp or gp.
 *
 * You may need to make changes/extensions to these classes
 * if you are working on IR optimization.

 */

#ifndef _H_tac
#define _H_tac

#include "list.h" // for VTable
#include "mips.h"

    // A Location object is used to identify the operands to the
    // various TAC instructions. A Location is either fp or gp
    // relative (depending on whether livein stack or global segemnt)
    // and has an offset relative to the base of that segment.
    // For example, a declaration for integer num as the first local
    // variable livein a function would be assigned a Location object
    // with name "num", segment fpRelative, and offset -8.

typedef enum {fpRelative, gpRelative} Segment;

typedef enum InstructionType
{
  DEFAULT, // should not be used
  LOADCONSTANT,
  LOADSTRINGCONSTANT,
  LOADLABEL,
  ASSIGN,
  LOAD,
  STORE,
  BINARYOP,
  LABEL,
  GOTO,
  IFZ,
  BEGINFUNC,
  ENDFUNC,
  RETURN,
  PUSHPARAM,
  POPPARAMS,
  LCALL,
  ACALL,
  VTABLE
} InstructionType;

class Location
{
  protected:
    const char *variableName;
    Segment segment;
    int offset;
    Location *reference;
    int refOffset;


    // The register allocated to this location.
    // A "zero" indicates that no register has been allocated.
    Mips::Register reg;

  public:
    Location(Segment seg, int offset, const char *name);
    Location(Location *base, int refOff) :
    variableName(base->variableName), segment(base->segment),
    offset(base->offset), reference(base), refOffset(refOff) {}

    const char *GetName()           { return variableName; }
    Segment GetSegment()            { return segment; }
    int GetOffset()                 { return offset; }
    bool IsReference()              { return reference != NULL; }
    Location *GetReference()        { return reference; }
    int GetRefOffset()              { return refOffset; }
    void SetRegister(Mips::Register r)    { reg = r; }
    Mips::Register GetRegister()          { return reg; }
};

  // base class from which all Tac instructions derived
  // has the interface for the 2 polymorphic messages: Print & Emit

class Instruction {
    protected:
        char printed[128];

    public:
  virtual InstructionType GetType() { return DEFAULT; }
  virtual void Print();
  virtual void EmitSpecific(Mips *mips) = 0;
  virtual void Emit(Mips *mips);
};

  // for convenience, the instruction classes are listed here.
  // the interfaces for the classes follows below
  class LoadConstant;
  class LoadStringConstant;
  class LoadLabel;
  class Assign;
  class Load;
  class Store;
  class BinaryOp;
  class Label;
  class Goto;
  class IfZ;
  class BeginFunc;
  class EndFunc;
  class Return;
  class PushParam;
  class PopParams;
  class LCall;
  class ACall;
  class VTable;

class LoadConstant: public Instruction {
    Location *dst;
    int val;
  public:
    LoadConstant(Location *dst, int val);
    InstructionType GetType() { return LOADCONSTANT; }
    void EmitSpecific(Mips *mips);
};

class LoadStringConstant: public Instruction {
    Location *dst;
    char *str;
  public:
    LoadStringConstant(Location *dst, const char *s);
    InstructionType GetType() { return LOADSTRINGCONSTANT; }
    void EmitSpecific(Mips *mips);
};

class LoadLabel: public Instruction {
    Location *dst;
    const char *label;
  public:
    InstructionType GetType() { return LOADLABEL; }
    LoadLabel(Location *dst, const char *label);
    void EmitSpecific(Mips *mips);
};

class Assign: public Instruction {
    Location *dst, *src;
  public:
    Assign(Location *dst, Location *src);
    InstructionType GetType() { return ASSIGN; }
    void EmitSpecific(Mips *mips);
};

class Load: public Instruction {
    Location *dst, *src;
    int offset;
  public:
    Load(Location *dst, Location *src, int offset = 0);
    InstructionType GetType() { return LOAD; }
    void EmitSpecific(Mips *mips);
};

class Store: public Instruction {
    Location *dst, *src;
    int offset;
  public:
    Store(Location *d, Location *s, int offset = 0);
    InstructionType GetType() { return STORE; }
    void EmitSpecific(Mips *mips);
};

class BinaryOp: public Instruction {

  public:
    static const char * const opName[Mips::NumOps];
    static Mips::OpCode OpCodeForName(const char *name);

  protected:
    Mips::OpCode code;
    Location *dst, *op1, *op2;
  public:
    BinaryOp(Mips::OpCode c, Location *dst, Location *op1, Location *op2);
    InstructionType GetType() { return BINARYOP; }
    void EmitSpecific(Mips *mips);
};

class Label: public Instruction {
    const char *label;
  public:
    Label(const char *label);
    void Print();
    InstructionType GetType() { return LABEL; }
    void EmitSpecific(Mips *mips);
    const char *GetLabel() { return label; }
};

class Goto: public Instruction {
    const char *label;
  public:
    Goto(const char *label);
    InstructionType GetType() { return GOTO; }
    void EmitSpecific(Mips *mips);
    const char *GetLabel() { return label; }
};

class IfZ: public Instruction {
    Location *test;
    const char *label;
  public:
    IfZ(Location *test, const char *label);
    InstructionType GetType() { return IFZ; }
    void EmitSpecific(Mips *mips);
    const char *GetLabel() { return label; }
};

class BeginFunc: public Instruction {
    int frameSize;
  public:
    BeginFunc();
    InstructionType GetType() { return BEGINFUNC; }
    // used to backpatch the instruction with frame size once known
    void SetFrameSize(int numBytesForAllLocalsAndTemps);
    void EmitSpecific(Mips *mips);
};

class EndFunc: public Instruction {
  public:
    EndFunc();
    InstructionType GetType() { return ENDFUNC; }
    void EmitSpecific(Mips *mips);
};

class Return: public Instruction {
    Location *val;
  public:
    Return(Location *val);
    InstructionType GetType() { return RETURN; }
    void EmitSpecific(Mips *mips);
};

class PushParam: public Instruction {
    Location *param;
  public:
    PushParam(Location *param);
    InstructionType GetType() { return PUSHPARAM; }
    void EmitSpecific(Mips *mips);
};

class PopParams: public Instruction {
    int numBytes;
  public:
    PopParams(int numBytesOfParamsToRemove);
    InstructionType GetType() {return POPPARAMS; }
    void EmitSpecific(Mips *mips);
};

class LCall: public Instruction {
    const char *label;
    Location *dst;
  public:
    LCall(const char *labe, Location *result);
    InstructionType GetType() { return LCALL; }
    void EmitSpecific(Mips *mips);
};

class ACall: public Instruction {
    Location *dst, *methodAddr;
  public:
    ACall(Location *meth, Location *result);
    InstructionType GetType() { return ACALL; }
    void EmitSpecific(Mips *mips);
};

class VTable: public Instruction {
    List<const char *> *methodLabels;
    const char *label;
 public:
    VTable(const char *labelForTable, List<const char *> *methodLabels);
    void Print();
    InstructionType GetType() { return VTABLE; }
    void EmitSpecific(Mips *mips);
};


#endif
