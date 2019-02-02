// thread.cc
//	Routines to manage threads.  These are the main operations:
//
//	Fork -- create a thread to run a procedure concurrently
//		with the caller (this is done in two steps -- first
//		allocate the Thread object, then call Fork on it)
//	Begin -- called when the forked procedure starts up, to turn
//		interrupts on and clean up after last thread
//	Finish -- called when the forked procedure finishes, to clean up
//	Yield -- relinquish control over the CPU to another ready thread
//	Sleep -- relinquish control over the CPU, but thread is now blocked.
//		In other words, it will not run again, until explicitly
//		put back on the ready queue.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "thread.h"
#include "switch.h"
#include "synch.h"
#include "sysdep.h"

// this is put at the top of the execution stack, for detecting stack overflows
const int STACK_FENCEPOST = 0xdedbeef;

//----------------------------------------------------------------------
// Thread::Thread
// 	Initialize a thread control block, so that we can then call
//	Thread::Fork.
//
//	"threadName" is an arbitrary string, useful for debugging.
//----------------------------------------------------------------------

Process::Process(char* name, int priority)
{
    this->priotity = priotity;
    this->status = JUST_CREATED;
    this->scheduler = new Scheduler();
    Thread *t = new Thread("P1 Main thread");

    this->currentThread = t;
    List<Thread*> (threadList = new List<Thread*>();
    //threadList->Append(t);
}

void
Process::Yield ()
{
    Process *nextProcess;
    Thread *nextThread;
    IntStatus oldLevel = kernel->interrupt->SetLevel(IntOff);

    nextProcess = kernel->scheduler->FindNextProcessToRun();
    if (nextProcess != NULL) {
      kernel->scheduler->ReadyToRun(this);
      kernel->scheduler->Run(nextProcess, FALSE);
    }
    }
    (void) kernel->interrupt->SetLevel(oldLevel);
}


void
Process::Fork(VoidFunctionPtr func, void *arg)
{
    Interrupt *interrupt = kernel->interrupt;
    Scheduler *scheduler = kernel->scheduler;
    IntStatus oldLevel;

    DEBUG(dbgThread, "Forking thread: " << name << " f(a): " << (int) func << " " << arg);

    StackAllocate(func, arg);

    oldLevel = interrupt->SetLevel(IntOff);
    scheduler->ReadyToRun(this);	// ReadyToRun assumes that interrupts
					// are disabled!
    (void) interrupt->SetLevel(oldLevel);
}
