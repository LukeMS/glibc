/* Linux/x86 CET inline functions.
   Copyright (C) 2017 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

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

#define DL_INIT	"_dl_cet_init"

#define ROUND_PROPERTY_NOTE(len) \
  (((len) + sizeof (ElfW(Addr)) - 1) & -sizeof (ElfW(Addr)))

/* Note sections like .note.ABI-tag and .note.gnu.build-id are aligned
   to 4 bytes in 64-bit ELF objects.  */
#define ROUND_NOTE(len) \
  (((len) + sizeof (ElfW(Word)) - 1) & -sizeof (ElfW(Word)))

#ifdef ElfW
static inline void __attribute__ ((unused))
dl_process_cet_property_note (struct link_map *l,
			      const ElfW(Nhdr) *note,
			      ElfW(Addr) size)
{
  const ElfW(Addr) start = (ElfW(Addr)) note;

  while ((ElfW(Addr)) (note + 1) - start < size)
    {
      /* Find the NT_GNU_PROPERTY_TYPE_0 note.  */
      if (note->n_namesz == 4
	  && note->n_type == NT_GNU_PROPERTY_TYPE_0
	  && memcmp (note + 1, "GNU", 4) == 0)
	{
	  unsigned int *ptr
	    = (unsigned int *) ((char *) &note->n_type
				+ ROUND_PROPERTY_NOTE (note->n_namesz));
	  if (ptr[0] == GNU_PROPERTY_X86_FEATURE_1_AND)
	    {
	      if (ptr[1] == 4)
		{
		  unsigned int pr_data = ptr[2];
		  if ((pr_data & GNU_PROPERTY_X86_FEATURE_1_IBT))
		    l->l_cet |= lc_ibt;
		  if ((pr_data & GNU_PROPERTY_X86_FEATURE_1_SHSTK))
		    l->l_cet |= lc_shstk;
		}
	      break;
	    }
	}

      /* Note sections like .note.ABI-tag and .note.gnu.build-id are
       * aligned to 4 bytes in 64-bit ELF objects.  */
      note = ((const void *) (note + 1)
	      + ROUND_NOTE (note->n_namesz)
	      + ROUND_NOTE (note->n_descsz));
    }
}

# ifdef FILEBUF_SIZE
#  define DL_PROCESS_PT_NOTE(l, ph, fd, fbp) \
  dl_process_pt_note ((l), (ph), (fd), (fbp))

static inline void __attribute__ ((unused))
dl_process_pt_note (struct link_map *l, const ElfW(Phdr) *ph,
		    int fd, struct filebuf *fbp)
{
  const ElfW(Nhdr) *note;
  ElfW(Addr) size = ph->p_filesz;

  if (ph->p_offset + size <= (size_t) fbp->len)
    note = (const void *) (fbp->buf + ph->p_offset);
  else
    {
      note = alloca (size);
      __lseek (fd, ph->p_offset, SEEK_SET);
      if (__libc_read (fd, (void *) note, size) != size)
	return;
    }

  dl_process_cet_property_note (l, note, size);
}
# else
#  define DL_PROCESS_PT_NOTE(l, ph) dl_process_pt_note ((l), (ph))

static inline void __attribute__ ((unused))
dl_process_pt_note (struct link_map *l, const ElfW(Phdr) *ph)
{
  const ElfW(Nhdr) *note = (const void *) (ph->p_vaddr + l->l_addr);
  dl_process_cet_property_note (l, note, ph->p_memsz);
}
# endif
#endif
