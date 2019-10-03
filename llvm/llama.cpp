/* Copyright (C) 2018, Derrick Greenspan and 		*
 * the University of Central Florida 			*
 * Licensed under the terms of the MIT License. 	*
 * Please see the LICENSE file for more information 	 */

/*  Skeleton Author: Adrian Sampson 			*
 * Portions (C) 2015 and Licensed under the MIT License */

#include <llvm/Pass.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include <llvm-c/Core.h> 
#include <llvm/PassSupport.h>
#include <llvm/Transforms/Scalar/LoopRotation.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/LoopPassManager.h>
#include <llvm/Transforms/Utils/LoopUtils.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Scalar/LoopUnrollPass.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/Analysis/MemoryLocation.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Transforms/Scalar/IndVarSimplify.h>

using namespace llvm;

namespace {
	int iterator = 0;
	struct funcValue
	{
		Value *func;
		Value *memValue;
		int uses = 0;
		bool visited = false;
    int id;
		IRBuilderBase::InsertPoint IP;
		Instruction *declaredOp;

	};
	int findFunc(funcValue *ptr, Value *func);

	Instruction * iterateInsertPoints(funcValue *ptr, int position)
	{
		return ptr[position].declaredOp;
	}

	funcValue *addFunc(funcValue *ptr, Value *func, Instruction *declaredOp)
	{
		funcValue *retFunc = (funcValue *) realloc(ptr, sizeof(funcValue) * (iterator + 1));
		retFunc[iterator].func = func; 
		retFunc[iterator].declaredOp = declaredOp;
		retFunc[iterator].uses = 0;
    retFunc[iterator].id = iterator;
		iterator++;
		return retFunc;
	}

	bool isInUsers(Instruction *Bi, Instruction *whichOne,  Value *PtrOp)
	{
		/*  If Instruction is part of the user list of vals */
		for(auto U : PtrOp->users())
		{
			if(auto *I = dyn_cast<Instruction>(U))
			{
				if(I == Bi)
					return true;
			}
		}
		return false;

	}

	int funcValExists(funcValue *ptr, Value *func)
	{
		int ret = findFunc(ptr, func);
		return ret;
	}

	int findFunc(funcValue *ptr, Value *func)
	{
		int i;
		Value *compFunc = func;
		for(i = 0; i < iterator; i++)
		{

			if(isa<BitCastInst>(func))
				compFunc = cast<BitCastInst>(func)->getOperand(0);

			if(ptr[i].func == compFunc)
			{
				return i;
			}
		}
		/*  Value was not found */
		return -1;
	}



  struct llamaPass : public FunctionPass {
    static char ID;
    llamaPass() : FunctionPass(ID) {}


    virtual bool runOnFunction(llvm::Function &F) {
	    LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
	    
	    LLVMContext &context = F.getContext();
	    funcValue * values = (funcValue *) calloc(sizeof(funcValue), 1);
	    funcValue value;
	    IRBuilder<> builder();
	    Instruction *theOp;

      for (auto &B : F) 
      {
	        for (auto &I : B) 
		{
			auto op = &I;
			theOp = op;
			Value *retValue;
			if(isa<CallInst>(I))
			{
				if((cast<CallInst>(I).getCalledFunction()->getName()).equals("_internal_calloc")
				|| (cast<CallInst>(I).getCalledFunction()->getName()).equals("_internal_malloc")
				|| (cast<CallInst>(I).getCalledFunction()->getName()).equals("_internal_realloc")
				|| (cast<CallInst>(I).getCalledFunction()->getName()).equals("_internal__mm_malloc"))
				{
					IRBuilder<> builder(op);
					/*  For each allocation via calloc, malloc, realloc, get the 
					 *  insert point, the value (for the store ptr later),  */
					values = addFunc(values, &(cast<Value>(I)), op);
				}
			} /* After a call inst, a store inst is performed. */
			if(isa<StoreInst>(I))
			{
				Value *ValOp = cast<StoreInst>(I).getValueOperand();
				Value *ptrOp = cast<StoreInst>(I).getPointerOperand();
					
				int pos = funcValExists(values, ValOp);
				int val = 0;
				if(pos != -1)
				{
					for (auto U : ptrOp->users())
				        {   
						values[pos].uses++;
						if(auto *I = dyn_cast<Instruction>(U))
				                {   
							int differential = 0, loopDepth = 0;
							if(int depth = LI.getLoopDepth(I->getParent()))
							{
								loopDepth = depth;
								differential = 15;
								bool activate = false;
								BasicBlock *BB = I->getParent();
								for(auto &in : *BB)
								{
									if(!activate && in.isIdenticalTo(I))
										activate = true;
									else if(activate && in.isIdenticalTo(I))
										activate = false;
									else if(activate)
									{
										differential--;
									}
								}
								int not_used = 0, used = 0;
							}
							val += pow(std::max(differential,0), loopDepth * 2);
				                }   
				        }   

					/*  If we allocated the value referenced by the operand via calloc, malloc, or realloc,
					 *  count the number of times the pointer operand is used */
					values[pos].uses += val;
					
				}

			}

      		}
      /*  Now we insert the uses.. */

      	int i;
      	for(i=0; i < iterator; i++)
	{

    fprintf(stderr, "Site: %d\n", values[i].id);
		IRBuilder<> builder(theOp);
		builder.SetInsertPoint(values[i].declaredOp);
		std::vector<Type*> paramTypes = {Type::getInt64Ty(context), Type::getInt32Ty(context)};
		Type *retType = Type::getVoidTy(context);
		FunctionType *LLVMScoreType = FunctionType::get(retType, paramTypes, false);
		Constant *LLVMScoreFunc = F.getParent()->getOrInsertFunction("setLLVMScore",  LLVMScoreType);
		Value *args_LLVMScore[] = {ConstantInt::get(Type::getInt64Ty(context), values[i].uses),
                               ConstantInt::get(Type::getInt32Ty(context), i)};
		builder.CreateCall(LLVMScoreFunc, args_LLVMScore);
      }
	iterator = 0;
      	free(values);
	values = (funcValue *) calloc(sizeof(funcValue), 1);
      }

      	return true;

}
	void getAnalysisUsage(AnalysisUsage &AU) const 
{ 
		AU.addRequired<LoopInfoWrapperPass>(); 
		AU.setPreservesAll (); 
	} 
};
}


// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerllamaPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
	auto LR = createLoopRotatePass();
  PM.add(LR);
  PM.add(new llamaPass());
}

static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerllamaPass);


char llamaPass::ID = 0;
static RegisterPass<llamaPass> X("llama", "llama llama",
		false, false);
