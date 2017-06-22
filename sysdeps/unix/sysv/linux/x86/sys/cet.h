/* ELF program property for Intel CET.
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

#ifndef _SYS_CET_H
#define _SYS_CET_H	1

/* Add x86 feature with IBT and/or SHSTK bits to ELF program property
   if they are enabled.  Otherwise, contents in this header file are
   unused.  */

# ifdef __ASSEMBLER__

#  ifdef __LP64__
#   define __PROPERTY_ALIGN 3
#  else
#   define __PROPERTY_ALIGN 2
#  endif

#  ifdef __IBT__
	.pushsection ".note.gnu.property", "a"
	.p2align __PROPERTY_ALIGN
	.long 1f - 0f		/* name length.  */
	.long 4f - 1f		/* data length.  */
	/* NT_GNU_PROPERTY_TYPE_0.   */
	.long 5			/* note type.  */
0:
	.asciz "GNU"		/* vendor name.  */
1:
	.p2align __PROPERTY_ALIGN
	/* GNU_PROPERTY_X86_FEATURE_1_AND.  */
	.long 0xc0000002	/* pr_type.  */
	.long 3f - 2f		/* pr_datasz.  */
2:
	/* GNU_PROPERTY_X86_FEATURE_1_IBT.  */
	.long 0x1
3:
	.p2align __PROPERTY_ALIGN
4:
	.popsection
#  endif

#  ifdef __SHSTK__
	.pushsection ".note.gnu.property", "a"
	.p2align __PROPERTY_ALIGN
	.long 1f - 0f		/* name length.  */
	.long 4f - 1f		/* data length.  */
	/* NT_GNU_PROPERTY_TYPE_0.   */
	.long 5			/* note type.  */
0:
	.asciz "GNU"		/* vendor name.  */
1:
	.p2align __PROPERTY_ALIGN
	/* GNU_PROPERTY_X86_FEATURE_1_AND.  */
	.long 0xc0000002	/* pr_type.  */
	.long 3f - 2f		/* pr_datasz.  */
2:
	/* GNU_PROPERTY_X86_FEATURE_1_SHSTK.  */
	.long 0x2
3:
	.p2align __PROPERTY_ALIGN
4:
	.popsection
#  endif

# endif

#endif	/* _SYS_CET_H */
