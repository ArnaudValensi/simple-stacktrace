#ifndef SIMPLE_STATCKTRACE_H
#define SIMPLE_STATCKTRACE_H

#include <stdio.h>
#define UNW_LOCAL_ONLY
#include <dlfcn.h>
#include <libunwind.h>

void *stacktrace_image_load_address() {
  Dl_info info;

  dladdr((const void *)&stacktrace_image_load_address, &info);
  return info.dli_fbase;
}

int stacktrace_address_to_line(const void *load_address, const void *address,
                               char *line_string) {
  FILE *fp;
  char atos_command[66];

  sprintf(atos_command, "atos -o main -fullPath -l %p %p\n", load_address,
          address);

  fp = popen(atos_command, "r");
  if (fp == NULL) {
    printf("Failed to run command\n");
    return 1;
  }

  if (fgets(line_string, 1024, fp) == NULL) {
    return 1;
  }

  pclose(fp);

  return 0;
}

void stacktrace() {
  char line_string[1024];
  // Get the load address which corresponds to the image slide caused by ASLR.
  void *load_address = stacktrace_image_load_address();

  unw_cursor_t cursor;
  unw_context_t context;

  // Initialize cursor to current frame for local unwinding.
  unw_getcontext(&context);
  unw_init_local(&cursor, &context);

  // Unwind frames one by one, going up the frame stack.
  while (unw_step(&cursor) > 0) {
    unw_word_t pc;
    unw_get_reg(&cursor, UNW_REG_IP, &pc);
    if (pc == 0) {
      break;
    }

    // Use the atos command to convert an address to symbol and line number.
    if (stacktrace_address_to_line(load_address, (const void *)pc,
                                   line_string) == 0) {
      fprintf(stderr, "0x%llx: %s", (uint64_t)pc, line_string);
    } else {
      fprintf(stderr,
              "0x%llx: unable to obtain symbol and line number for this frame",
              (uint64_t)pc);
    }
  }
}

#endif // SIMPLE_STATCKTRACE_H
