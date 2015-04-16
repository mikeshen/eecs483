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
#include <string>

#define DEFAULT 1 // should not be used
#define LOADCONSTANT 2 // U
#define LOADSTRINGCONSTANT 3 // U
#define LOADLABEL 4 // U
#define ASSIGN 5 // U
#define LOAD 6 // U
#define STORE 7 // U
#define BINARYOP 8 // U
#define LABEL 9 // E + 1, L
#define GOTO 10 // E
#define IFZ 11 // E - 1
#define BEGINFUNC 12 // L
#define ENDFUNC 13 // E
#define RETURN 14// E
#define PUSHPARAM 15 // U
#define POPPARAMS 16 // U
#define LCALL 17 // E
#define ACALL 18 // E
#define VTABLE 19 // U

using std::string;
    // A Location object is used to identify the operands to the
    // various TAC instructions. A Location is either fp or gp
    // relative (depending on whether livein stack or global segemnt)
    // and has an offset relative to the base of that segment.
    // For example, a declaration for integer num as the first local
    // variable livein a function would be assigned a Location object
    // with name "num", segment fpRelative, and offset -8.


typedef enum {fpRelative, gpRelative} Segment;


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
  virtual int GetType() { return DEFAULT; }
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
    int GetType() { return LOADCONSTANT; }
    void EmitSpecific(Mips *mips);
};

class LoadStringConstant: public Instruction {
    Location *dst;
    char *str;
  public:
    LoadStringConstant(Location *dst, const char *s);
    int GetType() { return LOADSTRINGCONSTANT; }
    void EmitSpecific(Mips *mips);
};

class LoadLabel: public Instruction {
    Location *dst;
    const char *label;
  public:
    int GetType() { return LOADLABEL; }
    LoadLabel(Location *dst, const char *label);
    void EmitSpecific(Mips *mips);
};

class Assign: public Instruction {
    Location *dst, *src;
  public:
    Assign(Location *dst, Location *src);
    int GetType() { return ASSIGN; }
    void EmitSpecific(Mips *mips);
};

class Load: public Instruction {
    Location *dst, *src;
    int offset;
  public:
    Load(Location *dst, Location *src, int offset = 0);
    int GetType() { return LOAD; }
    void EmitSpecific(Mips *mips);
};

class Store: public Instruction {
    Location *dst, *src;
    int offset;
  public:
    Store(Location *d, Location *s, int offset = 0);
    int GetType() { return STORE; }
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
    int GetType() { return BINARYOP; }
    void EmitSpecific(Mips *mips);
};

class Label: public Instruction {
    const char *label;
  public:
    Label(const char *label);
    void Print();
    int GetType() { return LABEL; }
    void EmitSpecific(Mips *mips);
    const char *GetLabel() { return label; }
};

class Goto: public Instruction {
    const char *label;
  public:
    Goto(const char *label);
    int GetType() { return GOTO; }
    void EmitSpecific(Mips *mips);
    const char *GetLabel() { return label; }
};

class IfZ: public Instruction {
    Location *test;
    const char *label;
  public:
    IfZ(Location *test, const char *label);
    int GetType() { return IFZ; }
    void EmitSpecific(Mips *mips);
    const char *GetLabel() { return label; }
};

class BeginFunc: public Instruction {
    int frameSize;
  public:
    BeginFunc();
    int GetType() { return BEGINFUNC; }
    // used to backpatch the instruction with frame size once known
    void SetFrameSize(int numBytesForAllLocalsAndTemps);
    void EmitSpecific(Mips *mips);
};

class EndFunc: public Instruction {
  public:
    EndFunc();
    int GetType() { return ENDFUNC; }
    void EmitSpecific(Mips *mips);
};

class Return: public Instruction {
    Location *val;
  public:
    Return(Location *val);
    int GetType() { return RETURN; }
    void EmitSpecific(Mips *mips);
};

class PushParam: public Instruction {
    Location *param;
  public:
    PushParam(Location *param);
    int GetType() { return PUSHPARAM; }
    void EmitSpecific(Mips *mips);
};

class PopParams: public Instruction {
    int numBytes;
  public:
    PopParams(int numBytesOfParamsToRemove);
    int GetType() {return POPPARAMS; }
    void EmitSpecific(Mips *mips);
};

class LCall: public Instruction {
    const char *label;
    Location *dst;
  public:
    LCall(const char *label, Location *result);
    int GetType() { return LCALL; }
    void EmitSpecific(Mips *mips);
};

class ACall: public Instruction {
    Location *dst, *methodAddr;
  public:
    ACall(Location *meth, Location *result);
    int GetType() { return ACALL; }
    void EmitSpecific(Mips *mips);
};

class VTable: public Instruction {
    List<const char *> *methodLabels;
    const char *label;
 public:
    VTable(const char *labelForTable, List<const char *> *methodLabels);
    void Print();
    int GetType() { return VTABLE; }
    void EmitSpecific(Mips *mips);
};


class BasicBlock {
private:
  bool isCreated;
  List<Instruction*>* instr;
  int start, end;
  List<string>* gen, *kill, *alive, *dead;
  BasicBlock* nextBlock;
  BasicBlock* gotoBlock; // null if no goto's

public:
  BasicBlock() : isCreated(false), start(-1), end(-1), nextBlock(NULL), gotoBlock(NULL)
  {
    instr = new List<Instruction*>();
    gen = new List<string>();
    kill = new List<string>();
    alive = new List<string>();
    dead = new List<string>();
  }

  void fillBlock(List<Instruction*>* code, int s, int e, BasicBlock* nextB,
    BasicBlock* gotoB)
  {
    Assert(!isCreated);
    start = s;
    end = e;
    nextBlock = nextB;
    gotoBlock = gotoB;
    for(int i = start; i < end; i++)
      instr->Append(code->Nth(i));
  }

  List<string>* getGen() { return gen; }

  List<string>* getAlive() { return alive; }

  List<string>* getKill() { return kill; }

  List<string>* getDead() { return dead; }

  void anaylsis() {
    //Insert Analysis here
  }

};


#endif
