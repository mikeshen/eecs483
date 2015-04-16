/* File: codegen.cc
 * ----------------
 * Implementation for the CodeGenerator class. The methods don't do anything
 * too fancy, mostly just create objects of the various Tac instruction
 * classes and append them to the list.
 */

#include "codegen.h"
#include <string.h>
#include "tac.h"
#include "mips.h"
#include "ast_decl.h"
#include "errors.h"

CodeGenerator::CodeGenerator()
{
  code = new List<Instruction*>();
  curGlobalOffset = 0;
}

char *CodeGenerator::NewLabel()
{
  static int nextLabelNum = 0;
  char temp[10];
  sprintf(temp, "_L%d", nextLabelNum++);
  return strdup(temp);
}


Location *CodeGenerator::GenTempVariable()
{
  static int nextTempNum;
  char temp[10];
  Location *result = NULL;
  sprintf(temp, "_tmp%d", nextTempNum++);
  return GenLocalVariable(temp);
}


Location *CodeGenerator::GenLocalVariable(const char *varName)
{
    curStackOffset -= VarSize;
    Location *loc = new Location(fpRelative, curStackOffset+4, varName);
    return loc;
}

Location *CodeGenerator::GenGlobalVariable(const char *varName)
{
    curGlobalOffset += VarSize;
    Location *loc = new Location(gpRelative, curGlobalOffset-4, varName);
    return loc;
}

Location *CodeGenerator::GenParameter(int index, const char *varName)
{
    Location *loc = new Location(fpRelative, OffsetToFirstParam+index*VarSize, varName);
    return loc;
}

Location *CodeGenerator::GenIndirect(Location* base, int offset)
{
    Location *loc = new Location(base, offset);
    return loc;
}


Location *CodeGenerator::GenLoadConstant(int value)
{
  Location *result = GenTempVariable();
  code->Append(new LoadConstant(result, value));
  return result;
}

Location *CodeGenerator::GenLoadConstant(const char *s)
{
  Location *result = GenTempVariable();
  code->Append(new LoadStringConstant(result, s));
  return result;
}

Location *CodeGenerator::GenLoadLabel(const char *label)
{
  Location *result = GenTempVariable();
  code->Append(new LoadLabel(result, label));
  return result;
}


void CodeGenerator::GenAssign(Location *dst, Location *src)
{
  code->Append(new Assign(dst, src));
}


Location *CodeGenerator::GenLoad(Location *ref, int offset)
{
  Location *result = GenTempVariable();
  code->Append(new Load(result, ref, offset));
  return result;
}

void CodeGenerator::GenStore(Location *dst,Location *src, int offset)
{
  code->Append(new Store(dst, src, offset));
}


Location *CodeGenerator::GenBinaryOp(const char *opName, Location *op1,
                 Location *op2)
{
  Location *result = GenTempVariable();
  code->Append(new BinaryOp(BinaryOp::OpCodeForName(opName), result, op1, op2));
  return result;
}


void CodeGenerator::GenLabel(const char *label)
{
  code->Append(new Label(label));
}

void CodeGenerator::GenIfZ(Location *test, const char *label)
{
  code->Append(new IfZ(test, label));
}

void CodeGenerator::GenGoto(const char *label)
{
  code->Append(new Goto(label));
}

void CodeGenerator::GenReturn(Location *val)
{
  code->Append(new Return(val));
}


BeginFunc *CodeGenerator::GenBeginFunc()
{
  BeginFunc *result = new BeginFunc;
  code->Append(result);
  insideFn = code->NumElements() - 1;
  curStackOffset = OffsetToFirstLocal;
  return result;
}

void CodeGenerator::GenEndFunc()
{
  code->Append(new EndFunc());
  BeginFunc *beginFunc = dynamic_cast<BeginFunc*>(code->Nth(insideFn));
  Assert(beginFunc != NULL);
  beginFunc->SetFrameSize(OffsetToFirstLocal-curStackOffset);
}

void CodeGenerator::GenPushParam(Location *param)
{
  code->Append(new PushParam(param));
}

void CodeGenerator::GenPopParams(int numBytesOfParams)
{
  Assert(numBytesOfParams >= 0 && numBytesOfParams % VarSize == 0); // sanity check
  if (numBytesOfParams > 0)
    code->Append(new PopParams(numBytesOfParams));
}

Location *CodeGenerator::GenLCall(const char *label, bool fnHasReturnValue)
{
  Location *result = fnHasReturnValue ? GenTempVariable() : NULL;
  code->Append(new LCall(label, result));
  return result;
}

Location *CodeGenerator::GenFunctionCall(const char *fnLabel, List<Location*> *args, bool hasReturnValue)
{
  for (int i = args->NumElements()-1; i >= 0; i--) // push params right to left
    GenPushParam(args->Nth(i));
  Location *result = GenLCall(fnLabel, hasReturnValue);
  GenPopParams(args->NumElements()*VarSize);
  return result;
}

Location *CodeGenerator::GenACall(Location *fnAddr, bool fnHasReturnValue)
{
  Location *result = fnHasReturnValue ? GenTempVariable() : NULL;
  code->Append(new ACall(fnAddr, result));
  return result;
}

Location *CodeGenerator::GenMethodCall(Location *rcvr,
           Location *meth, List<Location*> *args, bool fnHasReturnValue)
{
  for (int i = args->NumElements()-1; i >= 0; i--)
    GenPushParam(args->Nth(i));
  GenPushParam(rcvr); // hidden "this" parameter
  Location *result= GenACall(meth, fnHasReturnValue);
  GenPopParams((args->NumElements()+1)*VarSize);
  return result;
}


static struct _builtin {
  const char *label;
  int numArgs;
  bool hasReturn;
} builtins[] =
 {{"_Alloc", 1, true},
  {"_ReadLine", 0, true},
  {"_ReadInteger", 0, true},
  {"_StringEqual", 2, true},
  {"_PrintInt", 1, false},
  {"_PrintString", 1, false},
  {"_PrintBool", 1, false},
  {"_Halt", 0, false}};

Location *CodeGenerator::GenBuiltInCall(BuiltIn bn,Location *arg1, Location *arg2)
{
  Assert(bn >= 0 && bn < NumBuiltIns);
  struct _builtin *b = &builtins[bn];
  Location *result = NULL;

  if (b->hasReturn) result = GenTempVariable();
                // verify appropriate number of non-NULL arguments given
  Assert((b->numArgs == 0 && !arg1 && !arg2)
  || (b->numArgs == 1 && arg1 && !arg2)
  || (b->numArgs == 2 && arg1 && arg2));
  if (arg2) code->Append(new PushParam(arg2));
  if (arg1) code->Append(new PushParam(arg1));
  code->Append(new LCall(b->label, result));
  GenPopParams(VarSize*b->numArgs);
  return result;
}


void CodeGenerator::GenVTable(const char *className, List<const char *> *methodLabels)
{
  code->Append(new VTable(className, methodLabels));
}

void CodeGenerator::DoOptimization()
{
  Hashtable<BasicBlock*>* basicBlocks = new Hashtable<BasicBlock*>();
  Hashtable<BasicBlock*>* functionBlock = new Hashtable<BasicBlock*>();
  int lastEnd = -1;
  const char* blockName = checkLabel(basicBlocks, lastEnd + 1);
  const char* functionName = blockName;
  basicBlocks->Enter(blockName, new BasicBlock());

  for (int i = 1; i < code->NumElements(); i++) {
    bool labelFlag = false;
    const char* temp = NULL;
    BasicBlock* n = NULL;
    BasicBlock* g = NULL;

    switch(code->Nth(i)->GetType()) {
      case RETURN:
        if (i + 1 < code->NumElements()) {
          temp = checkLabel(basicBlocks, i + 1);
          n = basicBlocks->Lookup(temp);
          if (!std::atoi(temp))
            labelFlag = true;
          functionBlock->Enter(temp, n);
        } // only line that is different between LCALL/ACALL and RETURN is this
        genNewBlock(basicBlocks, blockName, functionName, lastEnd + 1, i + 1, NULL, NULL);
        lastEnd = i;
        if (labelFlag) i++;
        blockName = temp;
        break;
      case LABEL:
        temp = checkLabel(basicBlocks, i);
        n = basicBlocks->Lookup(temp);
        functionBlock->Enter(temp, n);
        genNewBlock(basicBlocks, blockName, functionName, lastEnd + 1, i, n, NULL);
        lastEnd = i - 1;
        blockName = temp;
        break;
      case BEGINFUNC:
        functionName = blockName;
        break;
      case ENDFUNC:
        if (i + 1 < code->NumElements()) {
          temp = checkLabel(basicBlocks, i + 1);
          n = basicBlocks->Lookup(temp);
          if (!std::atoi(temp))
            labelFlag = true;
          functionBlock->Enter(temp, n);
        }
        genNewBlock(basicBlocks, blockName, functionName, lastEnd + 1, i + 1, NULL, NULL);
        // TODO call function analysis on old functionBlock here
        delete functionBlock;
        functionBlock = new Hashtable<BasicBlock*>();
        lastEnd = i;
        if (labelFlag) i++;
        blockName = temp;
        break;
      // TODO implement IFZ and GOTO here
      case IFZ:
        Assert(code->Nth(i+1)->GetType() == GOTO);
        // goto label assignment
        temp = checkLabel(basicBlocks, i);
        g = basicBlocks->Lookup(temp);
        functionBlock->Enter(temp, g);
        // next label assignment
        if (i + 1 < code->NumElements()) {
          temp = checkLabel(basicBlocks, i + 1);
          n = basicBlocks->Lookup(temp);
          if (!std::atoi(temp))
            labelFlag = true;
          functionBlock->Enter(temp, n);
        }
        genNewBlock(basicBlocks, blockName, functionName, lastEnd + 1, i + 1, n, g);
        lastEnd = i;
        if (labelFlag) i++;
        blockName = temp;
        break;
      default:
        break;
    }

  }
}

void CodeGenerator::DoFinalCodeGen()
{
  if (IsDebugOn("tac")) { // if debug don't translate to mips, just print Tac
    for (int i = 0; i < code->NumElements(); i++)
      code->Nth(i)->Print();
  } else {
    Mips mips;
    mips.EmitPreamble();
    for (int i = 0; i < code->NumElements(); i++)
      code->Nth(i)->Emit(&mips);
  }
}

Location *CodeGenerator::GenArrayLen(Location *array)
{
  return GenLoad(array, -4);
}

Location *CodeGenerator::GenNew(const char *vTableLabel, int instanceSize)
{
  Location *size = GenLoadConstant(instanceSize);
  Location *result = GenBuiltInCall(Alloc, size);
  Location *vt = GenLoadLabel(vTableLabel);
  GenStore(result, vt);
  return result;
}


Location *CodeGenerator::GenDynamicDispatch(Location *rcvr, int vtableOffset, List<Location*> *args, bool hasReturnValue)
{
  Location *vptr = GenLoad(rcvr); // load vptr
  Assert(vtableOffset >= 0);
  Location *m = GenLoad(vptr, vtableOffset*4);
  return GenMethodCall(rcvr, m, args, hasReturnValue);
}

// all variables (ints, bools, ptrs, arrays) are 4 bytes in for code generation
// so this simplifies the math for offsets
Location *CodeGenerator::GenSubscript(Location *array, Location *index)
{
  Location *zero = GenLoadConstant(0);
  Location *isNegative = GenBinaryOp("<", index, zero);
  Location *count = GenLoad(array, -4);
  Location *isWithinRange = GenBinaryOp("<", index, count);
  Location *pastEnd = GenBinaryOp("==", isWithinRange, zero);
  Location *outOfRange = GenBinaryOp("||", isNegative, pastEnd);
  const char *pastError = NewLabel();
  GenIfZ(outOfRange, pastError);
  GenHaltWithMessage(err_arr_out_of_bounds);
  GenLabel(pastError);
  Location *four = GenLoadConstant(VarSize);
  Location *offset = GenBinaryOp("*", four, index);
  Location *elem = GenBinaryOp("+", array, offset);
  return GenIndirect(elem, 0);
}



Location *CodeGenerator::GenNewArray(Location *numElems)
{
  Location *one = GenLoadConstant(1);
  Location *isNonpositive = GenBinaryOp("<", numElems, one);
  const char *pastError = NewLabel();
  GenIfZ(isNonpositive, pastError);
  GenHaltWithMessage(err_arr_bad_size);
  GenLabel(pastError);

  // need (numElems+1)*VarSize total bytes (extra 1 is for length)
  Location *arraySize = GenLoadConstant(1);
  Location *num = GenBinaryOp("+", arraySize, numElems);
  Location *four = GenLoadConstant(VarSize);
  Location *bytes = GenBinaryOp("*", num, four);
  Location *result = GenBuiltInCall(Alloc, bytes);
  GenStore(result, numElems);
  return GenBinaryOp("+", result, four);
}

void CodeGenerator::GenHaltWithMessage(const char *message)
{
   Location *msg = GenLoadConstant(message);
   GenBuiltInCall(PrintString, msg);
   GenBuiltInCall(Halt, NULL);
}

// helper functions

void CodeGenerator::genNewBlock(Hashtable<BasicBlock*>* basicBlocks,
  const char* blockName, const char* functionName, int s, int e,
  BasicBlock* n, BasicBlock* g)
{
  basicBlocks->Lookup(blockName)->fillBlock(code, s, e, n, g);
}

const char* CodeGenerator::checkLabel(Hashtable<BasicBlock*>* basicBlocks, int s) {
  const char* label;
  char* temp = (char*)malloc(127);
  switch (code->Nth(s)->GetType()) {
    case LABEL:
      std::cerr << "called\n";
      label = dynamic_cast<Label*>(code->Nth(s))->GetLabel();
      std::cerr << label << "\n";
      break;
    case GOTO:
      label = dynamic_cast<Goto*>(code->Nth(s))->GetLabel();
      break;
    case IFZ:
      label = dynamic_cast<IfZ*>(code->Nth(s))->GetLabel();
      break;
    default:
      sprintf(temp, "%d", s);
      label = temp;
      return label;
  }
  free(temp);
  if (!basicBlocks->Lookup(label))
    basicBlocks->Enter(label, new BasicBlock());
  return label;
}
