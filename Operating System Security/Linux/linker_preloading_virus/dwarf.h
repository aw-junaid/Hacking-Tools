#ifndef __LIBELFMASTER_DWARF__
#define __LIBELFMASTER_DWARF__

#define DW_EH_PE_FORMAT_MASK	0x0f	/* format of the encoded value */
#define DW_EH_PE_APPL_MASK	0x70	/* how the value is to be applied */

#define DW_EH_PE_absptr		0x00
#define DW_EH_PE_omit		0xff
#define DW_EH_PE_uleb128	0x01
#define DW_EH_PE_udata2		0x02
#define DW_EH_PE_udata4		0x03
#define DW_EH_PE_udata8		0x04
#define DW_EH_PE_sleb128	0x09
#define DW_EH_PE_sdata2		0x0a
#define DW_EH_PE_sdata4		0x0b
#define DW_EH_PE_sdata8		0x0c
#define DW_EH_PE_signed		0x09
#define DW_EH_PE_pcrel		0x10
#define DW_EH_PE_indirect	0x80
#define DW_EH_PE_textrel	0x20
#define DW_EH_PE_datarel	0x30
#define DW_EH_PE_funcrel	0x40
#define DW_EH_PE_aligned	0x50

#define dw_read(ptr, type, end) ({	\
	type *__p = (type *) ptr;	\
	type  __v;			\
	if ((__p + 1) > (type *) end)	\
		return -EINVAL;		\
	__v = *__p++;			\
	ptr = (typeof(ptr)) __p;	\
	__v;				\
	})

#define DW_EH_PE_FORMAT_MASK	0x0f	/* format of the encoded value */

typedef struct eh_frame_vec {
	uint64_t initial_loc;
	uint64_t fde_entry_offset;
} eh_frame_vec_t;

typedef struct eh_frame_hdr {
	uint8_t version;
	uint8_t eh_frame_ptr_enc;
	uint8_t fde_count_enc;
	uint8_t table_enc;
	uint32_t eh_frame_ptr;
	uint32_t fde_count;
} __attribute__((packed)) eh_frame_hdr_t;
#endif
