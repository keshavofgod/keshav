/*
 *         Standard Dependable Vehicle Operating System
 *
 * Copyright (C) 2015 Ye Li (liye@sdvos.org)
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file   src/arch/linux/mcu.c
 * @author Ye Li (liye@sdvos.org)
 * @brief  Linux MCU Initialization
 */
#include <arch/linux/mcu.h>
#include <arch/linux/task.h>
#include <ucontext.h>
#include <stdlib.h>
#include <task.h>
#include <signal.h>
#include <debug.h>

void * linux_stack_pool = NULL;

void
McuInit ()
{
  int i = 0, total_stk = 0;
  sigset_t sigset;

  /* Disable all signals */
  sigfillset (&sigset);
  sigprocmask (SIG_SETMASK, &sigset, NULL);

  /* Set up stack pool */
  for (i = 0; i < NUM_TASKS; i++) {
    total_stk += (tasks[i].sp - tasks[i].sp_end);
  }
  /* 4K align stack pool */
  linux_stack_pool = malloc ((total_stk + 0xFFF) & 0xFFFFF000);
  for (i = 0; i < NUM_TASKS; i++) {
    tasks[i].sp = (data_addr_t) linux_stack_pool +
                  ((total_stk + 0xFFF) & 0xFFFFF000) -
                  (SRAM_END - tasks[i].sp);
    tasks[i].bp = tasks[i].sp;
    tasks[i].sp_end = (data_addr_t) linux_stack_pool +
                      ((total_stk + 0xFFF) & 0xFFFFF000) -
                      (SRAM_END - tasks[i].sp_end);
  }
}

/* vi: set et ai sw=2 sts=2: */
