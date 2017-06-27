/* This file is part of the GNU C Library.
   Copyright (C) 2017 Free Software Foundation, Inc.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */


#include <link.h>
#include <ldsodefs.h>

attribute_hidden
void
_dl_check_cet (const ElfW(Phdr) *phdr, size_t phnum,
	       const ElfW(Addr) addr, bool is_executable)
{
  if (phdr == NULL)
    return;

  struct cpu_features *cpu_features = &GLRO(dl_x86_cpu_features);
  size_t i;
  bool ibt_enabled = false;
  bool shstk_enabled = false;

  for (i = 0; i < phnum; i++)
    {
      if (phdr[i].p_type == PT_NOTE)
	{
	  const ElfW(Addr) start = phdr[i].p_vaddr + addr;
	  const ElfW(Nhdr) *note = (const void *) start;

	  while ((ElfW(Addr)) (note + 1) - start < phdr[i].p_memsz)
	    {
	      /* Find the NT_GNU_PROPERTY_TYPE_0 note.  */
	      if (note->n_namesz == 4
		  && note->n_type == NT_GNU_PROPERTY_TYPE_0
		  && memcmp (note + 1, "GNU", 4) == 0)
		{
#define ROUND(len) (((len) + sizeof (ElfW(Addr)) - 1) & -sizeof (ElfW(Addr)))
		  unsigned int *ptr
		    = (unsigned int *) ((char *) &note->n_type
					+ ROUND (note->n_namesz));
		  if (ptr[0] == GNU_PROPERTY_X86_FEATURE_1_AND)
		    {
		      if (ptr[1] == 4)
			{
			  unsigned int pr_data = ptr[2];
			  ibt_enabled
			    = !!(pr_data & GNU_PROPERTY_X86_FEATURE_1_IBT);
			  shstk_enabled
			    = !!(pr_data & GNU_PROPERTY_X86_FEATURE_1_SHSTK);
			}
		      break;
		    }
#undef ROUND
		}
/* Note sections like .note.ABI-tag and .note.gnu.build-id are aligned
   to 4 bytes in 64-bit ELF objects.  */
#define ROUND(len) (((len) + sizeof note->n_type - 1) & -sizeof note->n_type)
	      note = ((const void *) (note + 1)
		      + ROUND (note->n_namesz) + ROUND (note->n_descsz));
#undef ROUND
	    }
	}
    }

  /* If IBT isn't enabled on executable, disable IBT.  */
  if (is_executable && !ibt_enabled)
    cpu_features->feature[index_arch_IBT_Usable]
      &= ~bit_arch_IBT_Usable;

  /* If SHSTK isn't enabled, disable SHSTK.  */
  if (!shstk_enabled)
    cpu_features->feature[index_arch_SHSTK_Usable]
      &= ~bit_arch_SHSTK_Usable;
}
