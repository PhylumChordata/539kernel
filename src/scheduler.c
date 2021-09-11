#include "scheduler.h"
#include "process.h"

int next_sch_pid = 0;
int curr_sch_pid = 0;

process_t *curr_process, *next_process;

process_t *get_next_process()
{
	process_t *next_process = processes[ next_sch_pid ];
	
	curr_sch_pid = next_sch_pid;
	next_sch_pid++;
	next_sch_pid = next_sch_pid % processes_count;
	
	return next_process;
}

void scheduler( int eip, int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax )
{
	print( "EAX = " );
	printi( eax );
	println();

	curr_process = processes[ curr_sch_pid ];
	next_process = get_next_process();
		
	// Copy Context
	if ( curr_process->state == RUNNING )
	{
		curr_process->context.eax = eax;
		curr_process->context.ecx = ecx;
		curr_process->context.edx = edx;
		curr_process->context.ebx = ebx;
		curr_process->context.esp = esp;
		curr_process->context.ebp = ebp;
		curr_process->context.esi = esi;
		curr_process->context.edi = edi;
		curr_process->context.eip = eip;
	}
	
	curr_process->state = READY;
	next_process->state = RUNNING;
	
	curr_process = next_process;
}

void run_next_process()
{
	// No code should reside here at all. Otherwise,
	// we will not be sure that the context of the process
	// to run will be correct.
	
	asm( "	sti;			\
	 		jmp *%0" : : "r" ( curr_process->context.eip ) );
}
